#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"

#define STOP_TASK   // この行をコメントアウトするとタスクが実行されます




void task_setup() {

  #ifdef STOP_TASK
  return;
  #endif

  // WiFi接続タスクの作成
  xTaskCreate(
    task_connectToWiFi,   // タスクで実行される関数
    "task_ConnectToWiFi", // タスク名
    10000,           // スタックサイズ
    NULL,            // パラメータ
    1,               // 優先順位
    NULL             // タスクハンドル
  );



}