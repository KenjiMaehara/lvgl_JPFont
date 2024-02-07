#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include <WiFi.h>
#include "common.h"


const char* ssid = "20230616me_IPv6";
const char* password = "asdf0616";


void displayWiFiInfo(lv_obj_t *label_ssid, lv_obj_t *label_ip);

lv_obj_t *label_ssid;
lv_obj_t *label_ip;
lv_obj_t *wifi_list_label;


void task_connectToWiFi(void * parameter) {
    //WiFi.begin(ssid, password);

    // 無限ループでWiFiのステータスを監視
    for (;;) {
        if (isScanningWiFi) {
            // WiFiスキャン中は何もしない
            vTaskDelay(1000 / portTICK_PERIOD_MS); // 一時停止
            Serial.println("wifi_scanning...");
            displayWiFiInfo(label_ssid, label_ip); // WiFi情報の表示更新
            scanAndDisplayWiFiNetworks(wifi_list_label); // wifi_list_labelは適切に定義する
            isScanningWiFi = false; // スキャン終了
            continue;
        }  

        if (WiFi.status() == WL_CONNECTED) {
            // ここに接続済み時の処理を記述
            Serial.println("Connected to WiFi");
            //displayWiFiInfo(label_ssid, label_ip); // WiFi情報の表示更新
            vTaskDelay(1000 / portTICK_PERIOD_MS); // 10秒ごとにチェック
        } else if (WiFi.status() != WL_CONNECTED && isScanningWiFi == false) {
            // 接続が失われた場合の再接続処理
            Serial.println("Reconnecting to WiFi...");
            WiFi.disconnect();
            vTaskDelay(1000 / portTICK_PERIOD_MS); // 5秒ごとに再接続試行
            WiFi.begin(ssid, password);
            vTaskDelay(1000 / portTICK_PERIOD_MS); // 5秒ごとに再接続試行
        }
    }
}


// WiFi接続のステータスを確認し、接続が完了している場合にSSIDとIPアドレスを表示する関数
void displayWiFiInfo(lv_obj_t *label_ssid, lv_obj_t *label_ip) {
    if (WiFi.status() == WL_CONNECTED) {
        // SSIDとIPアドレスのテキストに追加の説明を付ける
        String ssid_text = "SSID:      " + WiFi.SSID();
        String ip_text = "IP address:     " + WiFi.localIP().toString();

        lv_label_set_text(label_ssid, ssid_text.c_str());
        lv_label_set_text(label_ip, ip_text.c_str());
    } else {
        lv_label_set_text(label_ssid, "SSID: Not connected");
        lv_label_set_text(label_ip, "IP: Not connected");
    }
}



void create_wifi_screen(lv_obj_t *scr) {

    Serial.println("create_wifi_screen start");

    // LVGLオブジェクト
    label_ssid = lv_label_create(scr);
    label_ip = lv_label_create(scr);

    // ラベルの位置を設定（必要に応じて調整してください）
    lv_obj_align(label_ssid, LV_ALIGN_CENTER, 0, -30);
    lv_obj_align(label_ip, LV_ALIGN_CENTER, 0, 30);

    // WiFi情報を表示
    displayWiFiInfo(label_ssid, label_ip);

    add_navigation_buttons(screen5, screen6, screen4);

    Serial.println("create_wifi_screen End");
}

