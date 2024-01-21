//#pragma execution_character_set("utf-8")

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"

void setup() {
    start_setup();
    task_setup();
}



void loop() {
    lv_task_handler();
    delay(5);
}
