#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include <WiFi.h>
#include <NTPClient.h>
#include "common.h"
#include <time.h>

static void btn_event_cb(lv_event_t *e);



void my_touchpad_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {


    uint16_t touchX, touchY;

    // タッチイベントの確認
    bool touched = tft.getTouch(&touchX, &touchY);

    

    if (touched) {

        data->point.x = touchX;
        data->point.y = touchY;
        data->state = LV_INDEV_STATE_PR; // プレス状態

        #ifdef DEBUG_MODE
        Serial.println("TFT getTouch!");
        // タッチ座標をシリアル出力
        Serial.print("Touch X: ");
        Serial.print(touchX);
        Serial.print(", Touch Y: ");
        Serial.println(touchY);
        #endif
    } else {
        data->state = LV_INDEV_STATE_REL; // リリース状態
    }

    //return false; // データが常に有効であることを示す

}


// ディスプレイフラッシュ関数
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    tft.pushColors((uint16_t *)&color_p->full, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}





// WiFiの資格情報
//const char* ssid = "googlemain";
//const char* password = "Fdsa@0813";
const char* ssid = "20230616me_IPv6";
const char* password = "asdf0616";


// NTPクライアントの設定
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600 * 9); // JSTの場合






LV_FONT_DECLARE(jpFont04);



// グローバル変数として時刻を表示するラベルを宣言
lv_obj_t *time_label;

void update_time(lv_timer_t *timer) {
    // 現在時刻を取得
    time_t now = time(NULL);
    struct tm *now_tm = localtime(&now);

    // 時刻を文字列に変換
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", now_tm);

    // 時刻を表示するラベルを更新
    lv_label_set_text(time_label, time_str);
}



void create_clock_screen(lv_obj_t *scr) {

    Serial.println("create_clock_screen Start");

    // 時刻を表示するラベルを作成
    time_label = lv_label_create(scr);
    lv_obj_align(time_label, LV_ALIGN_CENTER, 0, 0);

    // タイマーを作成して時刻を定期的に更新
    lv_timer_create(update_time, 1000, NULL);

    // 初期表示用に現在時刻を更新
    update_time(NULL);

    // 新しいボタンを作成
    lv_obj_t *btn = lv_btn_create(scr);
    lv_obj_set_pos(btn, 360, 240); // ボタンの位置を設定
    lv_obj_set_size(btn, 100, 50); // ボタンのサイズを設定

    // ボタンのラベルを作成
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "next");

    // ボタンのイベントハンドラを設定
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);


    Serial.println("create_clock_screen End");
}



// ボタンのイベントハンドラ
static void btn_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        // 時計画面を作成して表示
        lv_obj_t *security_scr = lv_obj_create(NULL);
        create_security_screen(security_scr);
        lv_scr_load(security_scr);
    }
}




