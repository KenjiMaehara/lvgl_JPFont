//#pragma execution_character_set("utf-8")

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"



static char number_str[64] = ""; // 数字を格納する文字列

// グローバル変数で入力された数字を保持
static char input_str[64] = "";

void keypad_btn_event_cb(lv_event_t *e);

static void btn_event_cb(lv_event_t *e);

// グローバル変数として数字を表示するラベルを宣言
lv_obj_t *number_label;

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
            lv_obj_add_event_cb(btn, keypad_btn_event_cb, LV_EVENT_CLICKED, NULL);
        }
    }

    // 数字を表示するラベルを作成
    number_label = lv_label_create(scr);
    lv_obj_align(number_label, LV_ALIGN_TOP_MID, 0, 10);
    lv_label_set_text(number_label, ""); // 初期状態ではテキストなし

    //add_navigation_buttons(scr, screen2, screen4);


    // 新しいボタンを作成
    lv_obj_t *btn = lv_btn_create(scr);
    lv_obj_set_pos(btn, 360, 240); // ボタンの位置を設定
    lv_obj_set_size(btn, 100, 50); // ボタンのサイズを設定

    // ボタンのラベルを作成
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "next");

    // ボタンのイベントハンドラを設定
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);


    Serial.println("create_keypad_screen End");
}



void keypad_btn_event_cb(lv_event_t *e) {
    lv_obj_t *btn = lv_event_get_target(e);
    const char *btn_text = lv_label_get_text(lv_obj_get_child(btn, 0));

    // 入力された数字をinput_strに追加（'C'でクリア、'E'でエンター）
    if (strcmp(btn_text, "C") == 0) {
        // 'C'が押された場合、入力をクリア
        strcpy(input_str, "");
    } else if (strcmp(btn_text, "E") != 0) {
        // 'E'以外のボタンが押された場合、数字を追加
        if (strlen(input_str) < sizeof(input_str) - 1) {
            strcat(input_str, btn_text);
        }
    }

    // input_strの内容をnumber_labelに表示
    lv_label_set_text(number_label, input_str);
}



// ボタンのイベントハンドラ
static void btn_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        // 時計画面を作成して表示
        lv_obj_t *next_scr = lv_obj_create(NULL);
        create_security_screen(next_scr);
        lv_scr_load(next_scr);
    }
}



