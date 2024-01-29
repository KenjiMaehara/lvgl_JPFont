//#pragma execution_character_set("utf-8")

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"
#include <WiFi.h> // WiFiライブラリをインクルード

void setup() {
    Serial.begin(115200);
    Serial.println("setup Start");
    //start_setup();
    lvgl_screen_setup();
    //clock_setup();
    task_setup();
    screen_setup();
    buzzer_setup();
    led_setup();

    WiFi.begin(ssid, password);
    Serial.println("setup End");
}



void loop() {
    lv_task_handler();
    delay(5);
}
