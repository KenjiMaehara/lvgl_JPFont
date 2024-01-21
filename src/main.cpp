//#pragma execution_character_set("utf-8")

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"
#include <WiFi.h> // WiFiライブラリをインクルード

void setup() {
    start_setup();
    task_setup();

    WiFi.begin(ssid, password);
}



void loop() {
    lv_task_handler();
    delay(5);
}
