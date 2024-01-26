#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include <WiFi.h>
#include "common.h"


// WiFiの資格情報
//const char* ssid = "googlemain";
//const char* password = "Fdsa@0813";
const char* ssid = "20230616me_IPv6";
const char* password = "asdf0616";



void task_connectToWiFi(void * parameter) {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");


  // このタスクの終了
  vTaskDelete(NULL);
}


// WiFi接続のステータスを確認し、接続が完了している場合にSSIDとIPアドレスを表示する関数
void displayWiFiInfo(lv_obj_t *label_ssid, lv_obj_t *label_ip) {
    if (WiFi.status() == WL_CONNECTED) {
        lv_label_set_text(label_ssid, WiFi.SSID().c_str());
        lv_label_set_text(label_ip, WiFi.localIP().toString().c_str());
    } else {
        lv_label_set_text(label_ssid, "SSID: Not connected");
        lv_label_set_text(label_ip, "IP: Not connected");
    }
}



void create_wifi_screen(lv_obj_t *scr) {

    Serial.println("create_wifi_screen start");

    // LVGLオブジェクト
    lv_obj_t* label_ssid = lv_label_create(scr);
    lv_obj_t* label_ip = lv_label_create(scr);

    // ラベルの位置を設定（必要に応じて調整してください）
    lv_obj_align(label_ssid, LV_ALIGN_CENTER, 0, -30);
    lv_obj_align(label_ip, LV_ALIGN_CENTER, 0, 30);

    // WiFi情報を表示
    displayWiFiInfo(label_ssid, label_ip);

    add_navigation_buttons(screen5, screen1, screen4);

    Serial.println("create_wifi_screen End");
}