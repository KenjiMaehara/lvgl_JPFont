//#pragma execution_character_set("utf-8")

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"
#include <WiFi.h> // WiFiライブラリをインクルード
#include <env.h>

void setup() {
    //start_setup();
    lvgl_screen_setup();
    //clock_setup();
    task_setup();
    screen_setup();
    buzzer_setup();
    sensor_input_setup();
    led_setup();
    readRFID_setup();

    preferences.begin("wifi", false); // Preferencesを開始
    String ssid = preferences.getString("ssid"); // 保存されたSSIDを読み出し
    String password = preferences.getString("password"); // 保存されたパスワードを読み出し

    //WiFi.begin(ssid, password);
    WiFi.begin(ssid.c_str(), password.c_str()); // WiFi接続を開始

    env_init();

}



void loop() {
    lv_task_handler();
    delay(5);
}
