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
#include "esp_heap_caps.h"

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
    //readRFID_setup();

    setup_wifiCheck_update();
    setupAWSIoT();

    //tryConnectToKnownNetworks();


}


unsigned long previousMillis = 0;  // 最後のメモリログ出力時刻を保持する変数
const long interval = 5000;        // メモリログ出力の間隔をミリ秒で設定 (5000ミリ秒 = 5秒)


void loop() {
    lv_task_handler();
    delay(5);

    // 現在の時刻を取得
    unsigned long currentMillis = millis();

    // 前回のデバッグ出力から5秒以上経過しているかチェック
    if (currentMillis - previousMillis >= interval) {
        // 最後のメモリログ出力時刻を更新
        previousMillis = currentMillis;

        // 現在のフリーヒープメモリサイズを取得してログ出力
        Serial.print("Free heap size: ");
        Serial.println(esp_get_free_heap_size());
    }

}
