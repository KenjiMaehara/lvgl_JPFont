#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"

static void btn_event_cb(lv_event_t *e);


void create_security_screen(lv_obj_t *scr) {

    Serial.println("create_security_screen Start");

    // 防犯開始・解除のボタン作成
    // ...
    //add_navigation_buttons(scr, screen4, screen2);


   // 新しいボタンを作成
    lv_obj_t *btn = lv_btn_create(scr);
    lv_obj_set_pos(btn, 360, 240); // ボタンの位置を設定
    lv_obj_set_size(btn, 100, 50); // ボタンのサイズを設定

    // ボタンのラベルを作成
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "next");

    // ボタンのイベントハンドラを設定
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);

    Serial.println("create_security_screen End");
}



// ボタンのイベントハンドラ
static void btn_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        // 時計画面を作成して表示
        lv_obj_t *security_scr = lv_obj_create(NULL);
        create_fourth_screen(security_scr);
        //lv_scr_load(security_scr);
    }
}
