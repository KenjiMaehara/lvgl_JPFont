#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"

static void btn_event_cb(lv_event_t *e);
static void event_handler(lv_event_t * e);


void create_security_screen(lv_obj_t *scr) {

    Serial.println("create_security_screen Start");

    // ボタンの作成と設定
    lv_obj_t * btn_02 = lv_btn_create(lv_scr_act()); // ボタンを作成
    lv_obj_set_size(btn_02, 200, 50); // ボタンのサイズ設定
    lv_obj_center(btn_02); // ボタンを中心に配置
    lv_obj_add_event_cb(btn_02, event_handler, LV_EVENT_ALL, NULL);

    // ボタンのラベルを設定
    lv_obj_t * label_02 = lv_label_create(btn_02);
    lv_label_set_text(label_02, "Disarmed");
    lv_obj_center(label_02);

    // 初期スタイル設定
    lv_obj_set_style_bg_color(btn_02, lv_color_make(0, 0, 0), 0); // 黒色に設定
    lv_obj_set_style_text_color(label_02, lv_color_make(255, 255, 255), 0); // テキストを白色に設定




    // NEXTボタンを作成
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


// イベントハンドラ関数
static void event_handler(lv_event_t * e) {
    lv_obj_t * btn = lv_event_get_target(e);
    lv_obj_t * label = lv_obj_get_child(btn, 0);

    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        if(strcmp(lv_label_get_text(label), "Disarmed") == 0) {
            lv_label_set_text(label, "Armed");
            lv_obj_set_style_bg_color(btn, lv_color_make(0, 255, 0), 0); // 緑色に設定
        } else {
            lv_label_set_text(label, "Disarmed");
            lv_obj_set_style_bg_color(btn, lv_color_make(0, 0, 0), 0); // 黒色に設定
        }
    }
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
