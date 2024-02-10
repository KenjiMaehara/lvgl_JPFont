#include <HardwareSerial.h>
#include <Arduino.h>
#include "common.h"


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

  Serial.println("-------------readRFIDTask Start--------------");
  // RFIDからデータを読み取るタスク
  while (1) {
    if (RFIDSerial.available()) {
      String id = RFIDSerial.readStringUntil('\n');
      int dataLength = id.length(); // 読み取ったデータの長さを取得

      Serial.print("RFID ID: ");
      Serial.println(id);
      Serial.print("Data Length: ");
      Serial.println(dataLength);

      // 受信データがMAX_CARD_LENGTH（38バイト）以上かどうかの判定
      if(dataLength <= MAX_CARD_LENGTH) {
        Serial.println("受信データはMAX_CARD_LENGTHバイト以上です。");
      } else {
        Serial.println("受信データはMAX_CARD_LENGTHバイト未満です。");
      }

      RFIDSerial.readBytes(read_buf,read_length);

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
        Serial.println("不明なカード");
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
    1024 * 2, /* スタックサイズ */
    NULL, /* タスクパラメータ */
    1, /* タスク優先度 */
    NULL /* タスクハンドル */
  );
  #endif
}