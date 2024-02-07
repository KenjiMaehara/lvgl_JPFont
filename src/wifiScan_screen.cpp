#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include <WiFi.h>
#include "common.h"





lv_obj_t *wifi_list;

// グローバル変数としてフラグを定義
volatile bool isScanningWiFi = false;


bool gWifiScan = false; 

// タイマーコールバック関数
void onTimer(TimerHandle_t xTimer) {
    if(gWifiScan == false || isScanningWiFi == true) return;

    isScanningWiFi = true;  // スキャン開始

}


// グローバル変数としてタイマーハンドルを定義
TimerHandle_t timer = NULL;

void create_wifiScan_screen(lv_obj_t *scr) {

    Serial.println("create_wifi_screen start");

    // リストオブジェクトの作成（create_wifi_screen関数内）
    wifi_list = lv_list_create(scr);
    //lv_obj_set_size(wifi_list, 200, 200); // リストのサイズを設定
    lv_obj_set_size(wifi_list, LV_PCT(70), LV_PCT(60));
    lv_obj_align(wifi_list, LV_ALIGN_CENTER, 0, 0); // リストの位置を調整

    // タイマーの設定（10秒ごとに onTimer を呼び出す）
    timer = xTimerCreate("WifiScanTimer", pdMS_TO_TICKS(10000), pdTRUE, (void*)0, onTimer);
    xTimerStart(timer, 0);

    add_navigation_buttons(screen6, screen1, screen5);

    Serial.println("create_wifi_screen End");
}

struct WiFiNetwork {
    String SSID;
    int32_t RSSI;
};


void scanAndDisplayWiFiNetworks(lv_obj_t *wifi_list_label) {

    Serial.println("scanAndDisplayWiFiNetworks_______Start_______");


    WiFi.disconnect(true);  // 強制的に切断
    delay(1000);  // 切断処理のための短いディレイ    
    // WiFiモジュールをステーションモードに設定
    WiFi.mode(WIFI_STA);
    delay(1000);  // WiFiモジュールのオフ処理のためのディレイ
    Serial.println("scanAndDisplayWiFiNetworks_______test_______1");

    //isScanningWiFi = true;  // スキャン開始

    if (WiFi.status() != WL_DISCONNECTED) {
        // WiFiが接続されている場合はスキャンを中止
        lv_label_set_text(wifi_list_label, "WiFi is connected, cannot scan");
        Serial.println("WiFi is connected, cannot scan");
        WiFi.disconnect(true);  // 強制的に切断
        delay(1000);  // 切断処理のための短いディレイ 
        //isScanningWiFi = false;
        return;
    }
    Serial.println("scanAndDisplayWiFiNetworks_______test_______2");

    const int maxNetworks = 10; // 表示する最大ネットワーク数
    int n = WiFi.scanNetworks();
    Serial.print("Found WiFi networks: ");
    Serial.println(n); // スキャン結果のネットワーク数を表示

    Serial.println("scanAndDisplayWiFiNetworks_______test_______3");

    // nが負の場合のエラー処理
    if (n <= 0) {
        lv_label_set_text(wifi_list_label, "Failed to scan networks or no networks found");
        Serial.println("Failed to scan networks or no networks found");
        return;
    }

    if (n > maxNetworks) {
        n = maxNetworks;
    }

    if (n == 0) {
        lv_label_set_text(wifi_list_label, "No networks found");
        Serial.println("No networks found");
    } else {
        if (n > maxNetworks) {
            n = maxNetworks; // ネットワークの数を制限
        }
        Serial.println("scanAndDisplayWiFiNetworks_______test_______4");
        // 動的メモリ割り当て
        WiFiNetwork* networks = new WiFiNetwork[n];
        Serial.println("scanAndDisplayWiFiNetworks_______test_______5");
        if (networks == nullptr) {
            lv_label_set_text(wifi_list_label, "Memory allocation failed");
            Serial.println("Memory allocation failed");
            return;
        }
        Serial.println("scanAndDisplayWiFiNetworks_______test_______6");
        // ネットワークの情報を保存
        for (int i = 0; i < n; ++i) {
            networks[i].SSID = WiFi.SSID(i);
            networks[i].RSSI = WiFi.RSSI(i);
        }
        Serial.println("scanAndDisplayWiFiNetworks_______test_______7");
        // ネットワークをRSSIでソート
        std::sort(networks, networks + n, [](const WiFiNetwork &a, const WiFiNetwork &b) {
            return a.RSSI > b.RSSI;
        });
        Serial.println("scanAndDisplayWiFiNetworks_______test_______8");
        // ソートされたリストを表示

        #if 0
        // リストオブジェクトの作成（create_wifi_screen関数内）
        lv_obj_t *wifi_list = lv_list_create(lv_scr_act());
        Serial.println("scanAndDisplayWiFiNetworks_______test_______9");
        //lv_obj_set_size(wifi_list, 200, 200); // リストのサイズを設定
        lv_obj_set_size(wifi_list, LV_PCT(70), LV_PCT(60));
        lv_obj_align(wifi_list, LV_ALIGN_CENTER, 0, 0); // リストの位置を調整
        Serial.println("scanAndDisplayWiFiNetworks_______test_______10");
        #endif

        //delay(1000);
        lv_obj_clean(wifi_list); // 以前のエントリをクリア
        String wifi_list_str;
        for (int i = 0; i < n; ++i) {
            wifi_list_str = String(i + 1) + ": " + networks[i].SSID + " (RSSI: " + networks[i].RSSI + ")";
            lv_list_add_btn(wifi_list, LV_SYMBOL_WIFI, wifi_list_str.c_str()); // ここで各ネットワーク情報をリストに追加
            Serial.println(wifi_list_str); // ここで各ネットワーク情報をシリアル出力
        }

        Serial.println("scanAndDisplayWiFiNetworks_______test_______11");
        lv_label_set_text(wifi_list_label, wifi_list_str.c_str());
        Serial.println("scanAndDisplayWiFiNetworks_______test_______12");
        // 割り当てられたメモリを解放
        delete[] networks;
        Serial.println("scanAndDisplayWiFiNetworks_______test_______13");
    }
    WiFi.scanDelete(); // スキャン結果をクリア
    Serial.println("scanAndDisplayWiFiNetworks_______test_______14");
    //isScanningWiFi = false; // スキャン終了

}
