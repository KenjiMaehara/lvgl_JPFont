#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include <WiFi.h>
#include <NTPClient.h>
#include "common.h"
#include <time.h>


// LVGL用のディスプレイバッファを定義
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[320 * 10];
static lv_disp_drv_t disp_drv;



TFT_eSPI tft = TFT_eSPI(); // TFTのインスタンスを作成

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






// NTPクライアントの設定
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600 * 9); // JSTの場合






void lvgl_screen_setup() {

    Serial.begin(115200); // シリアル通信の初期化
    Serial.println("lvgl_screen_setup Start");

    // TFTの初期化
    tft.begin();
    tft.setRotation(1);

    uint16_t calData[5] = { 231, 3567, 344, 3355, 7 };
    tft.setTouch(calData);

    // LVGLの初期化
    lv_init();

    // バッファのサイズを設定
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 320 * 10); // 解像度に基づいてバッファサイズを設定  
    // LVGLのディスプレイドライバー設定 

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 480; // ディスプレイの解像度を設定
    disp_drv.ver_res = 320;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);


    #if 1
    // タッチパッド入力デバイスを初期化して登録
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
    #endif
}



LV_FONT_DECLARE(jpFont04);


// グローバル変数としてタイマーカウンタを宣言
//unsigned long lastNtpUpdateTime = 0;
const unsigned long ntpUpdateInterval = 86400000; // 24時間 = 86400000ミリ秒
unsigned long lastNtpUpdateTime = ULONG_MAX - ntpUpdateInterval; // 初回実行時に必ずNTP更新が行われるように設定


// グローバル変数として時刻を表示するラベルを宣言
lv_obj_t *time_label;

// WiFiアイコンを作成
lv_obj_t *wifi_icon = lv_label_create(lv_scr_act());


void update_time(lv_timer_t *timer) {

    // Wi-Fi接続状態を確認
    if(WiFi.status() == WL_CONNECTED) {
        // 現在の時間をミリ秒単位で取得
        unsigned long currentMillis = millis();

        // 前回のNTP更新から24時間が経過していれば更新
        if(currentMillis - lastNtpUpdateTime >= ntpUpdateInterval) 
        {
            timeClient.update(); // NTPクライアントを更新
            Serial.println("NTP Update!");
            Serial.println("NTPサーバーから時刻を更新しました。");

            // NTPサーバーから取得した時刻を表示
            Serial.print("現在時刻: ");
            Serial.print(timeClient.getHours());
            Serial.print(":");
            Serial.print(timeClient.getMinutes());
            Serial.print(":");
            Serial.println(timeClient.getSeconds());

            // GMTオフセットとサマータイムの設定（例：日本標準時の場合）
            configTime(3600 * 9, 0, "pool.ntp.org", "time.nist.gov");

            lastNtpUpdateTime = currentMillis; // 最終更新時間を更新
        }
    }

    // 現在時刻を取得
    time_t now = time(NULL);
    struct tm *now_tm = localtime(&now);

    // 時刻を文字列に変換
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", now_tm);

    // 時刻を表示するラベルを更新
    lv_label_set_text(time_label, time_str);

    // TFT表示用に時刻をシリアル出力
    Serial.println(time_str);

    // 接続状態に応じてシンボルを更新
    if(WiFi.status() == WL_CONNECTED) {
        lv_label_set_text(wifi_icon, LV_SYMBOL_WIFI);
    } else {
        lv_label_set_text(wifi_icon, LV_SYMBOL_WIFI LV_SYMBOL_CLOSE);
    }
    lv_obj_align(wifi_icon, LV_ALIGN_OUT_TOP_RIGHT, -10, 10);
}



void create_clock_screen(lv_obj_t *scr) {

    Serial.println("create_clock_screen Start");

    #if 0
    // WiFiアイコンを作成
    wifi_icon = lv_img_create(scr);
    lv_img_set_src(wifi_icon, "path/to/disconnected_icon.png"); // 初期アイコン
    lv_obj_align(wifi_icon, LV_ALIGN_OUT_TOP_RIGHT, -10, 10); // 位置を右上に設定
    #endif

    // 時刻を表示するラベルを作成
    time_label = lv_label_create(scr);
    lv_obj_align(time_label, LV_ALIGN_CENTER, 0, 0);

    // ラベルのフォントサイズを大きくする
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_48, 0);

    // タイマーを作成して時刻を定期的に更新
    lv_timer_create(update_time, 1000, NULL);

    // 初期表示用に現在時刻を更新
    update_time(NULL);

    add_navigation_buttons(screen1, screen2, screen6);

    Serial.println("create_clock_screen End");
}


