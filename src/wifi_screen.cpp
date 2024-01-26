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


void create_wifi_screen(lv_obj_t *scr) {

    Serial.println("create_wifi_screen start");

    // LVGLオブジェクト
    lv_obj_t* label_ssid = lv_label_create(scr);
    lv_obj_t* label_ip = lv_label_create(scr);


    if (WiFi.status() == WL_CONNECTED) {
        lv_label_set_text(label_ssid, WiFi.SSID().c_str());
        lv_label_set_text(label_ip, WiFi.localIP().toString().c_str());
    }

    //add_navigation_buttons(screen5, screen1, screen4);

    Serial.println("create_wifi_screen End");
}