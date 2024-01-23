//#pragma execution_character_set("utf-8")

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"


static void btn_event_cb(lv_event_t *e);

static char number_str[64] = ""; // 数字を格納する文字列



void create_keypad_screen(lv_obj_t *scr) {

    //Serial.begin(115200); // シリアル通信の初期化
    //Serial.println("tenkey_setup Start");
    Serial.println("create_keypad_screen start");

    const int btn_width = 70; // ボタンの幅
    const int btn_height = 50; // ボタンの高さ
    const int cols = 3; // 列数
    const int rows = 4; // 行数

    // テンキーボタンのラベル
    const char *btn_labels[rows][cols] = {
        {"1", "2", "3"},
        {"4", "5", "6"},
        {"7", "8", "9"},
        {"C", "0", "E"}
    };

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            // ボタンの作成
            lv_obj_t *btn = lv_btn_create(scr);
            lv_obj_set_size(btn, btn_width, btn_height);
            lv_obj_set_pos(btn, col * btn_width, row * btn_height);

            // ボタンのラベルの作成
            lv_obj_t *label = lv_label_create(btn);
            lv_label_set_text(label, btn_labels[row][col]);

            // ボタンにイベントハンドラを追加（必要に応じて）
            // lv_obj_add_event_cb(btn, keypad_btn_event_cb, LV_EVENT_CLICKED, NULL);
        }
    }

    add_navigation_buttons(scr, screen2, screen4);
    Serial.println("create_keypad_screen End");
}


static void btn_event_cb(lv_event_t *e) {
    lv_obj_t *btn = lv_event_get_target(e);
    const char *txt = lv_label_get_text(lv_obj_get_child(btn, 0));
    strncat(number_str, txt, sizeof(number_str) - strlen(number_str) - 1);
    lv_obj_t *number_label = lv_obj_get_child(lv_scr_act(), -1);
    lv_label_set_text(number_label, number_str);
}