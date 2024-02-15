#include <time.h>
#include "lvgl.h"
#include "common.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// グローバル変数または共通クラス内での宣言
//lv_obj_t* time_labels[8]; // 最大8つの画面用のラベルポインタ配列
int current_screen = 0; // 現在表示中の画面のインデックス
unsigned long lastSyncTime = 86400000; // Store the last sync time in milliseconds
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600 * 9, 60000); // JSTのタイムゾーンで設定

// 時刻更新関数
void update_time(void) {

    if(WiFi.status() == WL_CONNECTED){
        
        unsigned long currentMillis = millis();
        // Check if 24 hours (86400000 milliseconds) have passed
        if (currentMillis - lastSyncTime >= 86400000){
            // Sync time with NTP server
            bool accessChekc = timeClient.update();
            Serial.println("NTP time updating...");
            // Update the last sync time
            if(accessChekc){
                Serial.println("NTP time updated");
                configTime(3600 * 9, 0, "pool.ntp.org","time.nist.gov");
                lastSyncTime = currentMillis;
            }
            //lastSyncTime = currentMillis;

            // Convert the NTP time to your desired format and update your display
            // This might involve converting the epoch time to a human-readable format
            // and then updating the display accordingly
        }
    }
    time_t now = time(NULL);
    struct tm *now_tm = localtime(&now);
    char timeString[64];
    strftime(timeString, sizeof(timeString), "%H:%M:%S", now_tm);
    Serial.println(timeString);

    // キーパッド画面の時刻表示用ラベルのテキストを更新
    if (time_label_keypad) {
        lv_label_set_text(time_label_keypad, timeString);
    }

    // セキュリティ画面の時刻表示用ラベルのテキストを更新
    if (time_label_security) {
        lv_label_set_text(time_label_security, timeString);
    }

    if (time_label_clock) {
        lv_label_set_text(time_label_clock, timeString);
    }

    if(time_label_area){
        lv_label_set_text(time_label_area, timeString);
    }
    
    if(time_label_ssid_IP){
        lv_label_set_text(time_label_ssid_IP, timeString);
    }

    if(time_label_wifiScan){
        lv_label_set_text(time_label_wifiScan, timeString);
    }

    if(time_label_apmode){
        lv_label_set_text(time_label_apmode, timeString);
    }

    if(time_label_card){
        lv_label_set_text(time_label_card, timeString);
    }

}

// タイマーコールバック関数
static void timer_update_time(lv_timer_t * timer) {
    update_time();
}

// アプリケーションの初期化部分でタイマーを設定
void setup_time_update() {
    lv_timer_create(timer_update_time, 1000, NULL); // 1秒ごとに更新
}

// 画面切替時に現在アクティブな画面インデックスを設定する関数
void switch_to_screen(int screen_index) {
    current_screen = screen_index;
    // 画面切り替えのロジック...
}
