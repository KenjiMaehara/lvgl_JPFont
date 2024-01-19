#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"


void task_setup() {

  // NTP更新タスクの作成
  xTaskCreatePinnedToCore(
    ntpUpdateTask, "NTP Update Task", 10000, NULL, 1, NULL, 0
  );
}