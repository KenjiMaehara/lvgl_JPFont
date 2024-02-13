//#pragma execution_character_set("utf-8")

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"
#include <WiFi.h> // WiFiライブラリをインクルード
#include <env.h>
#include <SPIFFS.h>

void setup() {
    
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

    #if 0
    preferences.begin("wifi", false); // Preferencesを開始
    String ssid = preferences.getString("ssid"); // 保存されたSSIDを読み出し
    String password = preferences.getString("password"); // 保存されたパスワードを読み出し
    #endif

    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFSのマウントに失敗しました");
        return;
    }

    File file = SPIFFS.open("/wifi_config.txt", FILE_READ);
    if (!file) {
        Serial.println("設定ファイルを開けませんでした");
        return;
    }

    String ssid = file.readStringUntil('\n');
    ssid.trim(); // 改行コードを削除
    String password = file.readStringUntil('\n');
    password.trim(); // 改行コードを削除
    file.close();    

    //WiFi.begin(ssid, password);
    WiFi.begin(ssid.c_str(), password.c_str()); // WiFi接続を開始

    //env_init();

}



void loop() {
    lv_task_handler();
    delay(5);
}
