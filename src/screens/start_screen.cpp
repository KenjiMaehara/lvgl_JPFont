#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include <WiFi.h>
#include <NTPClient.h>
#include "../common.h"
#include "clock_screen.h"
#include "tenkey_screen.h"


void btn_event_cb(lv_event_t *e);



// 現在の画面を指すポインタ
lv_obj_t* current_lvgl_screen = NULL;


void screen_setup() {
    //lv_init();
    //ili9488_init();

    create_clock_screen();
    //create_keypad_screen();

}




void add_navigation_buttons(lv_obj_t *scr, lv_obj_t *next_screen, lv_obj_t *prev_screen) {
    lv_obj_t *btn_next = lv_btn_create(scr);
    lv_obj_set_size(btn_next, 100, 50);
    lv_obj_align(btn_next, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
    lv_obj_set_user_data(btn_next, next_screen);
    lv_obj_add_event_cb(btn_next, btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *label_next = lv_label_create(btn_next);
    lv_label_set_text(label_next, "Next");

    lv_obj_t *btn_back = lv_btn_create(scr);
    lv_obj_set_size(btn_back, 100, 50);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    lv_obj_set_user_data(btn_back, prev_screen);
    lv_obj_add_event_cb(btn_back, btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Back");
}

#if 0

void create_fourth_screen(lv_obj_t *scr) {
    // 4番目のスクリーンの内容
    // ...
    add_navigation_buttons(scr, screen1, screen3);
}

#endif

void btn_event_cb(lv_event_t *e) {

    Serial.println("btn_event_cb start");

    #if 1
    lv_obj_t *btn = lv_event_get_target(e);

    // デバッグ情報の出力
    if (btn != NULL) {
        Serial.println("Event target (button) is not NULL");
        // さらに、ボタンに関する詳細情報を出力できます
    } else {
        Serial.println("Event target (button) is NULL");
    }
    #endif



    lv_obj_t *next_screen = (lv_obj_t *)lv_obj_get_user_data(btn);
    //lv_obj_t *next_screen = (lv_obj_t *)lv_event_get_target(e);

    // 次の画面のインデックス番号を取得してシリアル出力
    uintptr_t next_screen_index = (uintptr_t)lv_obj_get_user_data(next_screen);
    Serial.print("Next screen index: ");
    Serial.println(next_screen_index);
    switch_to_screen(next_screen_index);

    if (next_screen == screen6) {
        gWifiScan = true;   // Wi-Fi画面へ遷移する場合
        isScanningWiFi = true;  // スキャンを開始
    } else {
        gWifiScan = false;  // それ以外の場合
        //gScanSuccessful = -1;  // スキャンの成功フラグをリセット
        //isScanningWiFi = false;  // スキャンを停止
    }

    // デバッグ情報の出力
    if (next_screen != NULL) {
        Serial.println("next_screen is not NULL");
        lv_scr_load(next_screen);
    } else {
        Serial.println("next_screen is NULL");
    }


    if (next_screen != NULL) {
        buzzer_beep();
        Serial.println("buzzer_beep_on");
        
        Serial.println("next_screen");
        lv_scr_load(next_screen);
    }

    Serial.println("btn_event_cb end");
}

#if 0
void update_clock(lv_timer_t *timer) {
    // 現在時刻を取得し、screen2のラベルを更新
    // ...
}
#endif

