//#pragma execution_character_set("utf-8")

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"
#include <WiFi.h> // WiFiライブラリをインクルード
#include <env.h>
#include <SPIFFS.h>
#include <FS.h>
#include "AWSPub.h"

void setup() {
    Serial.begin(115200);
    
    env_init();

    
    lvgl_screen_setup();
    
    task_setup();
    screen_setup();
    setup_time_update();
    buzzer_setup();
    sensor_input_setup();
    led_setup();
    readRFID_setup();

    setup_wifiCheck_update();
    setupAWSIoT();

    //tryConnectToKnownNetworks();


}



void loop() {
    lv_task_handler();
    delay(5);
}
