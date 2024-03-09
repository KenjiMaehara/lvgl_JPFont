//#pragma execution_character_set("utf-8")

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "../common.h"
#include "start_screen.h"
#include "tenkey_screen.h"
#include "clock_screen.h"
#include "security_screen.h"




static char number_str[64] = ""; // 数字を格納する文字列

// グローバル変数で入力された数字を保持
static char input_str[64] = "";

void keypad_btn_event_cb(lv_event_t *e);
void update_time_label(lv_obj_t* label);

lv_obj_t* time_label_keypad;

// グローバル変数として数字を表示するラベルを宣言
lv_obj_t *number_label;

lv_obj_t* wifi_label_tenkey;

static void go_to_next_screen_event_handler(lv_event_t * e);
static lv_obj_t* screen2 = nullptr;

void create_keypad_screen(void) {

    Serial.println("create_keypad_screen start");

    if (screen2 == nullptr) {
        screen2 = lv_obj_create(NULL);
        // 画面2の構成要素を設定
        lv_obj_t* btn2 = lv_btn_create(screen2);
        lv_obj_add_event_cb(btn2, go_to_next_screen_event_handler, LV_EVENT_CLICKED, NULL);
        lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 0);
        lv_obj_t* label2 = lv_label_create(btn2);
        lv_label_set_text(label2, "Go to Screen 3");
    }
    lv_scr_load(screen2);

    Serial.println("create_keypad_screen End");
}


static void go_to_next_screen_event_handler(lv_event_t * e) {

    create_security_screen();

}


void keypad_btn_event_cb(lv_event_t *e) {
    buzzer_beep();
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



// 時刻を更新するための新しい関数
void update_time_label(lv_obj_t* label) {
    time_t now = time(NULL);
    struct tm *now_tm = localtime(&now);

    char timeString[64]; // 時刻を格納するための文字列
    strftime(timeString, sizeof(timeString), "%H:%M:%S", now_tm); // 時刻を文字列にフォーマット

    lv_label_set_text(label, timeString); // ラベルに現在時刻を設定
}