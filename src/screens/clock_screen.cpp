#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include <WiFi.h>
//#include <NTPClient.h>
#include "../common.h"
#include <time.h>
#include "start_screen.h"
#include "tenkey_screen.h"
#include "clock_screen.h"


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
//WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600 * 9); // JSTの場合






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

void screen_switch_event_handler(lv_event_t* e);
static lv_obj_t* screen1 = nullptr;

lv_obj_t* time_label_clock; // セキュリティスクリーン用の時刻表示ラベル
lv_obj_t* wifi_label_clock; // セキュリティスクリーン用のWi-Fi接続状態表示ラベル
//lv_obj_t* screen;
lv_obj_t* gScreen = NULL;

static void go_to_screen2_event_handler(lv_event_t * e);

//void create_clock_screen(lv_obj_t *scr) {
void create_clock_screen(void) {

    Serial.println("create_clock_screen Start");
    // 時刻を表示するラベルを作成
    if (screen1 == nullptr) {
        screen1 = lv_obj_create(NULL);  // スクリーンを作成

        // 時刻表示用のラベルを作成
        //time_label_clock = lv_label_create(scr);
        time_label_clock = lv_label_create(screen1);
        lv_obj_align(time_label_clock, LV_ALIGN_CENTER, 0, 0); // 画面の中央に配置
        lv_label_set_text(time_label_clock, "00:00"); // 初期テキスト


        wifi_label_clock = lv_label_create(screen1);
        lv_obj_align(wifi_label_clock, LV_ALIGN_TOP_RIGHT, -10, 10); // 画面の右上に配置
        lv_label_set_text(wifi_label_clock, LV_SYMBOL_WIFI LV_SYMBOL_CLOSE); // 初期テキスト


        // ラベルのフォントサイズを大きくする
        lv_obj_set_style_text_font(time_label_clock, &lv_font_montserrat_48, LV_STATE_DEFAULT); // フォントサイズを変更

        //add_navigation_buttons(screen1, screen2, screen6);


        // 画面1へ「Go to Screen 2」ボタンを追加
        lv_obj_t* btn1 = lv_btn_create(screen1);
        lv_obj_add_event_cb(btn1, go_to_screen2_event_handler, LV_EVENT_CLICKED, NULL);
        lv_obj_align(btn1, LV_ALIGN_TOP_LEFT, 10, 10); // ボタンの位置を画面の左上に設定
        //lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 0);
        lv_obj_t* label1 = lv_label_create(btn1);
        lv_label_set_text(label1, "Go to Screen 2");
    }


    lv_scr_load(screen1);
    Serial.println("create_clock_screen End");
}


static void go_to_screen2_event_handler(lv_event_t * e) {
    delete_previous_screen(&screen1);
    create_keypad_screen();
}


// イベントハンドラ内でのキャスト
void screen_switch_event_handler(lv_event_t* e) {

    Serial.println("-----screen_switch_event_handler start----------");

    void (*create_screen_func)(void) = (void (*)(void))lv_event_get_user_data(e);
    create_screen_func(); // 関数を呼び出す
}


void delete_previous_screen(lv_obj_t** previous_screen) {
    if (*previous_screen != NULL) {
        lv_obj_del(*previous_screen);
        *previous_screen = NULL; // ポインタをNULLに設定
    }
}