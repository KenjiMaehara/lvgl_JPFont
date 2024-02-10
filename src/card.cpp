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

typedef void (*card_read_cb_t)(struct card_data_t * card);

#define MAX_CARD_LENGTH 38 // 最大カードデータ長を定義

#define		CARD_125KHZ		0xfd
#define		CARD_FELICA		0xfc
#define		CARD_14443A		0xfb
#define		CARD_14443B		0xfa
#define		CARD_15693		0xf8
#define		CARD_START		0xfe

void card_set_cb(card_read_cb_t card_cb);

card_read_cb_t  card_callback;

HardwareSerial RFIDSerial(1); // UART1を使用

// タスクで使用するためのピン設定
const int RXPin = 16; // RXピンを16番に設定
const int TXPin = 17; // TXピンを17番に設定
const int baudRate = 9600; // ボーレート設定

void readRFIDTask(void *parameter) {

  size_t read_length;
  uint8_t read_buf[MAX_CARD_LENGTH];
  card_data_t card;
  int   verify_cnt = 0;
  int   loop_cnt = 0;

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
            Serial.println("CARD_14443B");
          } else if( read_length == 22 && read_buf[22 -1] == CARD_15693) {
            Serial.println("ISO15693カード");
          }
        
            // カードデータを16進数文字列に変換
            String cardDataStr = "";
            for(int i = 0; i < read_length; i++) {
              char hexBuf[3];
              sprintf(hexBuf, "%02X", read_buf[i]);
              cardDataStr += String(hexBuf);
            }
            Serial.print("Card Data: ");
            Serial.println(cardDataStr);

            int data_length = read_length - 2;
                    
            if(memcmp(&read_buf[1],card.data,data_length) !=0 || data_length != card.data_length)
            {
                memcpy(card.data,&read_buf[1],data_length);
                card.data_length = data_length;
                verify_cnt = 0;
            }
            else
            {
               verify_cnt++;
                if(verify_cnt == 2)
                {
                    card_callback(&card);
                    //Serial.printf("card %s\n",card.data);
                }
            }


        } else {
          /* card start error */
          memset(read_buf,0,MAX_CARD_LENGTH);    
          verify_cnt = 0;
        }
      } else {

        // dummy read
        for(int i=0; i < read_length ; i++)
          RFIDSerial.read();
        // read_lengthが異常値の場合、バッファをクリア
        //while(RFIDSerial.available()) RFIDSerial.read();
      }
    }
    else
    {
      delay(100);
      if(verify_cnt > 0 )
      {
              loop_cnt++;
          if(loop_cnt  > 15) 
          {
              memset(&card,0,sizeof(card_data_t));
              verify_cnt = 0;
              loop_cnt = 0;
              printf("cada data clear\n");
          }
      }
    }

    //vTaskDelay(10 / portTICK_PERIOD_MS); // タスクのディレイ
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