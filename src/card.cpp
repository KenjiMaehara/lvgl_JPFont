#include <HardwareSerial.h>
#include <Arduino.h>
#include "common.h"

#define   MAX_CARD_LENGTH 38

/**********************
 *      TYPEDEFS
 **********************/
struct card_data_t{
    uint8_t type;
    uint8_t data[MAX_CARD_LENGTH];
    int     data_length;
} ;

#define MAX_CARD_LENGTH 38 // 最大カードデータ長を定義

#define		CARD_125KHZ		0xfd
#define		CARD_FELICA		0xfc
#define		CARD_14443A		0xfb
#define		CARD_14443B		0xfa
#define		CARD_15693		0xf8
#define		CARD_START		0xfe

size_t read_length;
uint8_t read_buf[MAX_CARD_LENGTH];


HardwareSerial RFIDSerial(1); // UART1を使用

// タスクで使用するためのピン設定
const int RXPin = 16; // RXピンを16番に設定
const int TXPin = 17; // TXピンを17番に設定
const int baudRate = 9600; // ボーレート設定

void readRFIDTask(void *parameter) {

  card_data_t card;

  Serial.println("-------------readRFIDTask Start--------------");
  // RFIDからデータを読み取るタスク

  memset(&card,0,sizeof(card_data_t));  // カードデータの初期化

  while (1) {
    read_length = RFIDSerial.available();
    if (read_length > 0) {
      //String id = RFIDSerial.readStringUntil('\n');
      //int dataLength = id.length(); // 読み取ったデータの長さを取得
      //read_length = RFIDSerial.available();


      //Serial.print("RFID ID: ");
      //Serial.println(id);

      Serial.print("read_length: ");
      Serial.println(read_length);
      // 受信データがMAX_CARD_LENGTH（38バイト）以上かどうかの判定
      if(read_length <= MAX_CARD_LENGTH) {

        RFIDSerial.readBytes(read_buf,read_length);

 
        //Serial.print("RFID ID: ");
        //Serial.println(id);


        if(read_buf[0] == CARD_START
            && ( ( read_length == 38 && read_buf[38 -1] == CARD_FELICA)
                || ( read_length == 12 && read_buf[12 -1] == CARD_14443A)
                || ( read_length == 26 && read_buf[26 -1] == CARD_14443B)
                || ( read_length == 22 && read_buf[22 -1] == CARD_15693)
                )    
          )                    
        {
          if( read_length == 38 && read_buf[38 -1] == CARD_FELICA) {
            Serial.println("FeliCaカード");
          } else if( read_length == 12 && read_buf[12 -1] == CARD_14443A) {
            Serial.println("Mifareカード");
          } else if( read_length == 26 && read_buf[26 -1] == CARD_14443B) {
            Serial.println("Mifareカード");
          } else if( read_length == 22 && read_buf[22 -1] == CARD_15693) {
            Serial.println("ISO15693カード");
          }
        } else {
          delay(100);
          //Serial.println("不明なカード");
          // dummy read
          //for(int i=0; i < read_length ; i++)
          //RFIDSerial.read();
        }
      } else {
        // read_lengthが異常値の場合、バッファをクリア
        while(RFIDSerial.available()) RFIDSerial.read();
      }
    }

    vTaskDelay(10 / portTICK_PERIOD_MS); // タスクのディレイ
  }
}

void readRFID_setup() {
  Serial.begin(115200);
  // UART1の初期化、IO16をRX、IO17をTXとして使用
  RFIDSerial.begin(baudRate, SERIAL_8N1, RXPin, TXPin);

  #ifdef ENABLE_RFID_TASK
  // RFIDデータ読み取りタスクの作成
  xTaskCreate(
    readRFIDTask, /* タスク関数 */
    "ReadRFIDTask", /* タスク名 */
    1024 * 4, /* スタックサイズ */
    NULL, /* タスクパラメータ */
    1, /* タスク優先度 */
    NULL /* タスクハンドル */
  );
  #endif
}