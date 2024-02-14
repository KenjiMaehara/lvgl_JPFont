//#pragma execution_character_set("utf-8")

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"
#include <WiFi.h> // WiFiライブラリをインクルード
#include <env.h>
#include <SPIFFS.h>
#include <FS.h>

void setup() {
    Serial.begin(115200);
    
    env_init();
    //start_setup();
    lvgl_screen_setup();
    //clock_setup();
    task_setup();
    screen_setup();
    buzzer_setup();
    sensor_input_setup();
    led_setup();
    readRFID_setup();


    tryConnectToKnownNetworks();

}



void loop() {
    lv_task_handler();
    delay(5);
}
