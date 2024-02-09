#include <HardwareSerial.h>
#include <Arduino.h>
#include "common.h"

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
      Serial.print("RFID ID: ");
      Serial.println(id);
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