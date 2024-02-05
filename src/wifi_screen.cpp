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

// グローバル変数の定義
lv_obj_t* wifi_list_label;

lv_obj_t* label_ssid;
lv_obj_t* label_ip;

// グローバル変数としてフラグを定義
volatile bool isScanningWiFi = false;

void task_connectToWiFi(void * parameter) {
    //WiFi.begin(ssid, password);

    // 無限ループでWiFiのステータスを監視
    for (;;) {
        if (isScanningWiFi) {
            // WiFiスキャン中は何もしない
            vTaskDelay(5000 / portTICK_PERIOD_MS); // 一時停止
            Serial.println("wifi_scanning...");
            continue;
        }  

        if (WiFi.status() == WL_CONNECTED) {
            // ここに接続済み時の処理を記述
            Serial.println("Connected to WiFi");
            displayWiFiInfo(label_ssid, label_ip); // WiFi情報の表示更新
            vTaskDelay(10000 / portTICK_PERIOD_MS); // 10秒ごとにチェック
        } else if (WiFi.status() != WL_CONNECTED && isScanningWiFi == true) {
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

// タイマーコールバック関数
void onTimer(TimerHandle_t xTimer) {
    scanAndDisplayWiFiNetworks(wifi_list_label); // wifi_list_labelは適切に定義する
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
    wifi_list_label = lv_label_create(scr);
    lv_obj_align(wifi_list_label, LV_ALIGN_CENTER, 0, 60); // 位置の調整

    // タイマーの設定（10秒ごとに onTimer を呼び出す）
    TimerHandle_t timer = xTimerCreate("WifiScanTimer", pdMS_TO_TICKS(10000), pdTRUE, (void*)0, onTimer);
    xTimerStart(timer, 0);

    add_navigation_buttons(screen5, screen1, screen4);

    Serial.println("create_wifi_screen End");
}

struct WiFiNetwork {
    String SSID;
    int32_t RSSI;
};

void scanAndDisplayWiFiNetworks(lv_obj_t *wifi_list_label) {

    WiFi.disconnect(true);  // 強制的に切断
    delay(1000);  // 切断処理のための短いディレイ    
    // WiFiモジュールをステーションモードに設定
    WiFi.mode(WIFI_STA);
    delay(1000);  // WiFiモジュールのオフ処理のためのディレイ


    isScanningWiFi = true;  // スキャン開始

    if (WiFi.status() != WL_DISCONNECTED) {
        // WiFiが接続されている場合はスキャンを中止
        lv_label_set_text(wifi_list_label, "WiFi is connected, cannot scan");
        Serial.println("WiFi is connected, cannot scan");
        isScanningWiFi = false;
        return;
    }

    const int maxNetworks = 10; // 表示する最大ネットワーク数
    int n = WiFi.scanNetworks();
    Serial.print("Found WiFi networks: ");
    Serial.println(n); // スキャン結果のネットワーク数を表示

    // nが負の場合のエラー処理
    if (n <= 0) {
        lv_label_set_text(wifi_list_label, "Failed to scan networks or no networks found");
        return;
    }

    if (n > maxNetworks) {
        n = maxNetworks;
    }

    if (n == 0) {
        lv_label_set_text(wifi_list_label, "No networks found");
    } else {
        if (n > maxNetworks) {
            n = maxNetworks; // ネットワークの数を制限
        }
        //Serial.println("scanAndDisplayWiFiNetworks_______test_______1");
        // 動的メモリ割り当て
        WiFiNetwork* networks = new WiFiNetwork[n];
        //Serial.println("scanAndDisplayWiFiNetworks_______test_______2");
        if (networks == nullptr) {
            lv_label_set_text(wifi_list_label, "Memory allocation failed");
            return;
        }
        //Serial.println("scanAndDisplayWiFiNetworks_______test_______3");
        // ネットワークの情報を保存
        for (int i = 0; i < n; ++i) {
            networks[i].SSID = WiFi.SSID(i);
            networks[i].RSSI = WiFi.RSSI(i);
        }
        //Serial.println("scanAndDisplayWiFiNetworks_______test_______4");
        // ネットワークをRSSIでソート
        std::sort(networks, networks + n, [](const WiFiNetwork &a, const WiFiNetwork &b) {
            return a.RSSI > b.RSSI;
        });
        //Serial.println("scanAndDisplayWiFiNetworks_______test_______5");
        // ソートされたリストを表示
        String wifi_list_str = "Nearby WiFi Networks:\\n";
        for (int i = 0; i < n; ++i) {
            wifi_list_str += String(i + 1) + ": " + networks[i].SSID + " (RSSI: " + networks[i].RSSI + ")\\n";
        }
        lv_label_set_text(wifi_list_label, wifi_list_str.c_str());
        //Serial.println("scanAndDisplayWiFiNetworks_______test_______6");
        // 割り当てられたメモリを解放
        delete[] networks;
    }
    WiFi.scanDelete(); // スキャン結果をクリア
    isScanningWiFi = false; // スキャン終了
}
