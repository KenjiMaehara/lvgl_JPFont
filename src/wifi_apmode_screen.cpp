#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include <Preferences.h>
#include "common.h"


//const char* ssid = "ESP32-AP";
//const char* password = "123456789";

WebServer server(80);

String inputPage();
void setupWiFi(String ssid, String password);
static void ap_mode_toggle_handler(lv_event_t *e);
void handleClientTask(void *parameters);

Preferences preferences;

void wifi_apmode() {

  preferences.begin("wifi", false); // "wifi"は名前空間です
  String ssid = preferences.getString("ssid", "ESP32-AP"); // デフォルト値は "ESP32-AP"
  String password = preferences.getString("password", "12345678"); // デフォルト値は "12345678"

  WiFi.softAP("ESP32-AP", "12345678");
  Serial.println("AP Mode Enabled. SSID: ESP32-AP, Password: 12345678");

  //WiFi.softAP(ssid.c_str(), password.c_str());
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // ウェブサーバーのルートで呼び出される関数を設定
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", inputPage());
  });

  // フォームが送信されたときに呼び出される関数を設定
  server.on("/setup", HTTP_POST, []() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);

    server.sendHeader("Connection", "close");
    server.send(200, "text/html", "<h1>Setup complete.</h1><p>Device will now restart.</p>");
    setupWiFi(ssid, password);
  });

  server.begin();
}

String inputPage() {
  return "<!DOCTYPE html><html><head><title>ESP32 WiFi Setup</title></head><body><h1>WiFi Setup</h1><form action='/setup' method='post'><input type='text' name='ssid' placeholder='SSID'><br><input type='password' name='password' placeholder='Password'><br><input type='submit' value='Setup'></form></body></html>";
}


void setupWiFi(String ssid, String password) {
  Serial.println("Setting up WiFi connection...");
  WiFi.begin(ssid.c_str(), password.c_str());

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    counter++;
    if (counter >= 60) { // 30秒後にタイムアウト
      Serial.println("Failed to connect.");
      ESP.restart(); // 再起動
    }
  }

  Serial.println("WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  gApModeOn = false;
  // ここで必要な処理を行う
}


void create_apmode_screen(lv_obj_t *scr) {
    Serial.println("create_wifi_screen start");

    // ここに既存のコードを継続

    // APモード切り替えボタンの作成
    lv_obj_t *ap_mode_btn = lv_btn_create(scr);
    lv_obj_set_pos(ap_mode_btn, 10, 10); // ボタンの位置を設定
    lv_obj_set_size(ap_mode_btn, 100, 50); // ボタンのサイズを設定
    lv_obj_add_event_cb(ap_mode_btn, ap_mode_toggle_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t *label = lv_label_create(ap_mode_btn);
    lv_label_set_text(label, "AP Mode");

    add_navigation_buttons(screen7, screen1, screen6);
    // タスクの作成と実行
    xTaskCreate(
      handleClientTask, /* タスク関数 */
      "HandleClientTask", /* タスク名 */
      10000, /* スタックサイズ */
      NULL, /* タスクパラメータ */
      1, /* タスク優先度 */
      NULL /* タスクハンドル */
    );
    
    Serial.println("create_wifi_screen End");
}

bool gApModeOn = false;

// ボタンのイベントハンドラ
static void ap_mode_toggle_handler(lv_event_t *e) {

    buzzer_beep();
    Serial.println("buzzer_beep_on");
    gApModeOn = !gApModeOn;

    if (gApModeOn) {
        gApModeOn = true;
        WiFi.disconnect(true);  // 強制的に切断
        delay(1000);  // 切断処理のための短いディレイ   
        // APモードをオンにする
        wifi_apmode();
        
    } else {
        gApModeOn = false;
        // APモードをオフにする
        WiFi.softAPdisconnect(true);
        Serial.println("AP Mode Disabled");
        
    }
}


void handleClientTask(void *parameters) {

  Serial.println("handleClientTask start");

  for (;;) {
    server.handleClient();
    delay(10); // CPUリソースを適切に利用
  }
}