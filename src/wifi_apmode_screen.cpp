#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
//#include <Preferences.h>
#include "common.h"
#include <SPIFFS.h>
#include <FS.h>
#include <ArduinoJson.h> // ArduinoJsonライブラリをインクルード

//const char* ssid = "ESP32-AP";
//const char* password = "123456789";

WebServer server(80);

String inputPage();
void setupWiFi(String ssid, String password);
static void ap_mode_toggle_handler(lv_event_t *e);
void handleClientTask(void *parameters);

void saveWiFiConfig(const String& ssid, const String& password);
void tryConnectToKnownNetworks();


//Preferences preferences;

void wifi_apmode() {

  // SPIFFSの初期化
  if(!SPIFFS.begin(true)){
      Serial.println("SPIFFSのマウントに失敗しました");
      return;
  }
  // 保存されたWiFi設定で接続を試みる
  tryConnectToKnownNetworks();

  // APモードを有効にする
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
    String newSSID = server.arg("ssid");
    String newPassword = server.arg("password");
    
    // 新しいSSIDとパスワードが入力されたら、それを保存
    saveWiFiConfig(newSSID, newPassword);
  
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", "<h1>Setup complete.</h1><p>Device will now restart.</p>");
    setupWiFi(newSSID, newPassword);
  });

  server.begin();
}

String inputPage() {
  return "<!DOCTYPE html><html><head><title>ESP32 WiFi Setup</title></head><body><h1>WiFi Setup</h1><form action='/setup' method='post'><input type='text' name='ssid' placeholder='SSID'><br><input type='password' name='password' placeholder='Password'><br><input type='submit' value='Setup'></form></body></html>";
}


// JSON形式でWiFi設定を保存
void saveWiFiConfig(const String& ssid, const String& password) {
  DynamicJsonDocument doc(1024);
  // 既存の設定を読み込む
  fs::File configFile = SPIFFS.open("/wifi_config.json", FILE_READ);
  if (configFile) {
    deserializeJson(doc, configFile);
    configFile.close();
  }

  // 新しい設定を追加
  JsonArray array = doc.as<JsonArray>();
  JsonObject newObj = array.createNestedObject();
  newObj["ssid"] = ssid;
  newObj["password"] = password;

  // 設定をファイルに保存
  configFile = SPIFFS.open("/wifi_config.json", FILE_WRITE);
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return;
  }
  serializeJson(doc, configFile);
  configFile.close();
}

// 設定を読み込み、接続を試みる
void tryConnectToKnownNetworks() {
  DynamicJsonDocument doc(1024);
  fs::File configFile = SPIFFS.open("/wifi_config.json", FILE_READ);
  if (!configFile) {
    Serial.println("Failed to open config file");
    return;
  }
  deserializeJson(doc, configFile);
  configFile.close();

  JsonArray array = doc.as<JsonArray>();
  for (JsonObject obj : array) {
    String ssid = obj["ssid"].as<String>();
    String password = obj["password"].as<String>();

    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Trying to connect to: ");
    Serial.println(ssid);
    if (WiFi.waitForConnectResult() == WL_CONNECTED) {
      Serial.println("Connected!");
      break; // 接続に成功したらループを抜ける
    }
  }
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
    //lv_obj_set_pos(ap_mode_btn, 10, 10); // ボタンの位置を設定
    //lv_obj_set_size(ap_mode_btn, 100, 50); // ボタンのサイズを設定
    lv_obj_set_size(ap_mode_btn, 300, 100); // ボタンサイズ設定
    lv_obj_center(ap_mode_btn); // ボタンを中央に配置
    lv_obj_add_event_cb(ap_mode_btn, ap_mode_toggle_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label = lv_label_create(ap_mode_btn);
    lv_label_set_text(label, "Switch to AP Mode");
    lv_obj_set_style_text_font(lv_obj_get_child(ap_mode_btn, 0), &lv_font_montserrat_16, 0); // フォントサイズ変更
    lv_obj_center(label);

    add_navigation_buttons(screen7, screen1, screen6);
    #ifdef ENABLE_APMODE_TASK
    // タスクの作成と実行
    xTaskCreate(
      handleClientTask, /* タスク関数 */
      "HandleClientTask", /* タスク名 */
      10000, /* スタックサイズ */
      NULL, /* タスクパラメータ */
      1, /* タスク優先度 */
      NULL /* タスクハンドル */
    );
    #endif

    Serial.println("create_wifi_screen End");
}

bool gApModeOn = false;

// ボタンのイベントハンドラ
static void ap_mode_toggle_handler(lv_event_t *e) {

    lv_obj_t * btn = lv_event_get_target(e);

    buzzer_beep();
    Serial.println("buzzer_beep_on");
    gApModeOn = !gApModeOn;

    if (gApModeOn) {
        // 色を黄色に変更し、テキストを更新
        lv_obj_set_style_bg_color(btn, lv_color_hex(0xFFFF00), LV_PART_MAIN); // 黄色に変更
        lv_obj_set_style_text_color(btn, lv_color_black(), LV_PART_MAIN); // テキスト色を黒に設定
        lv_label_set_text(lv_obj_get_child(btn, 0), "Now in AP Mode\nConnect to SSID: ESP32-AP");
        lv_obj_set_style_text_font(lv_obj_get_child(btn, 0), &lv_font_montserrat_16, 0); // フォントサイズ変更
        
        gApModeOn = true;
        WiFi.disconnect(true);  // 強制的に切断
        delay(1000);  // 切断処理のための短いディレイ   
        // APモードをオンにする
        wifi_apmode();
        
    } else {
        // 元の色とテキストに戻す
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x0000FF), LV_PART_MAIN); // 青色に戻す
        lv_obj_set_style_text_color(btn, lv_color_white(), LV_PART_MAIN); // テキスト色を白に設定
        lv_label_set_text(lv_obj_get_child(btn, 0), "Switch to AP Mode");
        lv_obj_set_style_text_font(lv_obj_get_child(btn, 0), &lv_font_montserrat_16, 0); // フォントサイズ変更
        gApModeOn = false;
        // APモードをオフにする
        WiFi.softAPdisconnect(true);
        Serial.println("AP Mode Disabled");
        
    }
}


void handleClientTask(void *parameters) {

  Serial.println("--------------handleClientTask start--------------");

  for (;;) {
    server.handleClient();
    delay(10); // CPUリソースを適切に利用
  }
}