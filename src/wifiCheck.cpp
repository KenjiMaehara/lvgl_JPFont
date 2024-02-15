#include <WiFi.h>
#include <lvgl.h>
#include "common.h"

void wifiCheck(void) {


    if(wifi_label_clock) {
        // 接続状態に応じてシンボルを更新
        if(WiFi.status() == WL_CONNECTED) {
            lv_label_set_text(wifi_label_clock, LV_SYMBOL_WIFI);
        } else {
            lv_label_set_text(wifi_label_clock, LV_SYMBOL_WIFI LV_SYMBOL_CLOSE);
        }
    }


}


// タイマーコールバック関数
static void timer_update_wifiCheck(lv_timer_t * timer) {
    wifiCheck();
}

// アプリケーションの初期化部分でタイマーを設定
void setup_wifiCheck_update() {
    lv_timer_create(timer_update_wifiCheck, 10000, NULL); // 10秒ごとに更新
}
