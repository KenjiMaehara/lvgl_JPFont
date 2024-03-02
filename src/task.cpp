#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"
#include "wifi_screen.h"






void task_setup() {

  #ifdef ENABLE_WIFI_TASK
  // WiFi接続タスクの作成
  xTaskCreate(
    task_connectToWiFi,   // タスクで実行される関数
    "task_ConnectToWiFi", // タスク名
    1024 * 4,           // スタックサイズ
    NULL,            // パラメータ
    2,               // 優先順位
    NULL             // タスクハンドル
  );
  #endif
}