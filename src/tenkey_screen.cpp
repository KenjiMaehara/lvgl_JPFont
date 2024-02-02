//#pragma execution_character_set("utf-8")

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"



static char number_str[64] = ""; // 数字を格納する文字列

// グローバル変数で入力された数字を保持
static char input_str[64] = "";

void keypad_btn_event_cb(lv_event_t *e);

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

    // スタイルを定義
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_border_color(&style, lv_color_black());
    lv_style_set_border_width(&style, 2); // 枠線の幅を2ピクセルに設定

    const int spacing = 10; // ボタン間の余白

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            // ボタンの作成
            lv_obj_t *btn = lv_btn_create(scr);
            lv_obj_set_size(btn, btn_width, btn_height);
            lv_obj_set_pos(btn, 55 + 70 + col * (btn_width + spacing), 70 + row * (btn_height + spacing));

            // ボタンのラベルの作成
            lv_obj_t *label = lv_label_create(btn);
            lv_label_set_text(label, btn_labels[row][col]);

            // ボタンにスタイルを適用
            lv_obj_add_style(btn, &style, 0);

            // ボタンにイベントハンドラを追加（必要に応じて）
            lv_obj_add_event_cb(btn, keypad_btn_event_cb, LV_EVENT_CLICKED, NULL);
        }
    }

    // 数字を表示するラベルを作成
    number_label = lv_label_create(scr);
    lv_obj_align(number_label, LV_ALIGN_TOP_MID, 0, 10);
    lv_label_set_text(number_label, ""); // 初期状態ではテキストなし

    // Set a larger font for the number label
    lv_obj_set_style_text_font(number_label, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);

    add_navigation_buttons(screen2, screen3, screen1);
    Serial.println("create_keypad_screen End");
}



void keypad_btn_event_cb(lv_event_t *e) {
    //buzzer_beep();
    Serial.println("buzzer_beep_on");

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