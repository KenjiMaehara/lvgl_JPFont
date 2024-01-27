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


void displayWiFiInfo(lv_obj_t *label_ssid, lv_obj_t *label_ip);
void scanAndDisplayWiFiNetworks(lv_obj_t *wifi_list_label);

lv_obj_t* label_ssid;
lv_obj_t* label_ip;


void task_connectToWiFi(void * parameter) {
    WiFi.begin(ssid, password);

    // 無限ループでWiFiのステータスを監視
    for (;;) {
        if (WiFi.status() == WL_CONNECTED) {
            // ここに接続済み時の処理を記述
            Serial.println("Connected to WiFi");
            displayWiFiInfo(label_ssid, label_ip); // WiFi情報の表示更新
            vTaskDelay(10000 / portTICK_PERIOD_MS); // 10秒ごとにチェック
        } else {
            // 接続が失われた場合の再接続処理
            Serial.println("Reconnecting to WiFi...");
            WiFi.disconnect();
            WiFi.begin(ssid, password);
            vTaskDelay(5000 / portTICK_PERIOD_MS); // 5秒ごとに再接続試行
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

    // WiFiリストラベルの作成
    lv_obj_t* wifi_list_label = lv_label_create(scr);
    lv_obj_align(wifi_list_label, LV_ALIGN_CENTER, 0, 60); // 位置の調整
    // スキャンして表示
    scanAndDisplayWiFiNetworks(wifi_list_label);

    add_navigation_buttons(screen5, screen1, screen4);

    Serial.println("create_wifi_screen End");
}

struct WiFiNetwork {
    String SSID;
    int32_t RSSI;
};

void scanAndDisplayWiFiNetworks(lv_obj_t *wifi_list_label) {
    const int maxNetworks = 10; // 表示する最大ネットワーク数
    int n = WiFi.scanNetworks();
    if (n == 0) {
        lv_label_set_text(wifi_list_label, "No networks found");
    } else {
        if (n > maxNetworks) {
            n = maxNetworks; // ネットワークの数を制限
        }

        // 動的メモリ割り当て
        WiFiNetwork* networks = new WiFiNetwork[n];
        if (networks == nullptr) {
            lv_label_set_text(wifi_list_label, "Memory allocation failed");
            return;
        }

        // ネットワークの情報を保存
        for (int i = 0; i < n; ++i) {
            networks[i].SSID = WiFi.SSID(i);
            networks[i].RSSI = WiFi.RSSI(i);
        }

        // ネットワークをRSSIでソート
        std::sort(networks, networks + n, [](const WiFiNetwork &a, const WiFiNetwork &b) {
            return a.RSSI > b.RSSI;
        });

        // ソートされたリストを表示
        String wifi_list_str = "Nearby WiFi Networks:\\n";
        for (int i = 0; i < n; ++i) {
            wifi_list_str += String(i + 1) + ": " + networks[i].SSID + " (RSSI: " + networks[i].RSSI + ")\\n";
        }
        lv_label_set_text(wifi_list_label, wifi_list_str.c_str());

        // 割り当てられたメモリを解放
        delete[] networks;
    }
    WiFi.scanDelete(); // スキャン結果をクリア
}
