#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"






void task_setup() {

  // WiFi接続タスクの作成
  xTaskCreate(
    task_connectToWiFi,   // タスクで実行される関数
    "task_ConnectToWiFi", // タスク名
    10000,           // スタックサイズ
    NULL,            // パラメータ
    3,               // 優先順位
    NULL             // タスクハンドル
  );


  // RFIDデータ読み取りタスクの作成
  xTaskCreate(
    readRFIDTask, /* タスク関数 */
    "ReadRFIDTask", /* タスク名 */
    1024 * 2, /* スタックサイズ */
    NULL, /* タスクパラメータ */
    1, /* タスク優先度 */
    NULL /* タスクハンドル */
  );


  // LED点滅タスクを作成
  xTaskCreate(
    blinkLedTask, 
    "Blink LED Task",
    2048, 
    NULL, 
    1, 
    NULL
  );

  // ピン監視タスクの作成
  xTaskCreate(
    pinMonitorTask, 
    "Pin Monitor", 
    10000, 
    NULL, 
    1, 
    NULL
  );

  // タスクの作成と実行
  xTaskCreate(
    handleClientTask, /* タスク関数 */
    "HandleClientTask", /* タスク名 */
    10000, /* スタックサイズ */
    NULL, /* タスクパラメータ */
    1, /* タスク優先度 */
    NULL /* タスクハンドル */
  );

}