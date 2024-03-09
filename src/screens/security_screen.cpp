#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "../common.h"
#include "security_screen.h"
#include "start_screen.h"
#include "clock_screen.h"
#include "area_screen.h"


//static void btn_event_cb(lv_event_t *e);
static void event_handler(lv_event_t * e);
static void btn_event_handler3(lv_event_t* e);

lv_obj_t* time_label_security; // セキュリティスクリーン用の時刻表示ラベル
static lv_obj_t* screen3 = nullptr;

void create_security_screen(void) {

    Serial.println("create_security_screen Start");
    if (screen3 == nullptr) {

        screen3 = lv_obj_create(NULL);  // スクリーンを作成

        #if 1
        // 時刻表示用のラベルを作成
        time_label_security = lv_label_create(screen3);
        lv_obj_align(time_label_security, LV_ALIGN_TOP_MID, 0, 5); // 画面の上中央に配置
        lv_label_set_text(time_label_security, "00:00"); // 初期テキスト



        // ボタンの作成と設定
        lv_obj_t * btn_02 = lv_btn_create(screen3); // ボタンを作成
        lv_obj_set_size(btn_02, 400, 100); // ボタンのサイズ設定
        lv_obj_center(btn_02); // ボタンを中心に配置
        lv_obj_add_event_cb(btn_02, event_handler, LV_EVENT_ALL, NULL);

        // ボタンのラベルを設定
        lv_obj_t * label_02 = lv_label_create(btn_02);
        lv_label_set_text(label_02, "Disarmed");
        lv_obj_center(label_02);

        // 初期スタイル設定
        lv_obj_set_style_bg_color(btn_02, lv_color_make(0, 0, 0), 0); // 黒色に設定
        lv_obj_set_style_text_color(label_02, lv_color_make(255, 255, 255), 0); // テキストを白色に設定
        
        lv_obj_t* btn3 = lv_btn_create(screen3);
        lv_obj_add_event_cb(btn3, btn_event_handler3, LV_EVENT_CLICKED, NULL);
        lv_obj_align(btn3, LV_ALIGN_TOP_LEFT, 10, 10); // ボタンの位置を画面の左上に設定
        lv_obj_t* label3 = lv_label_create(btn3);
        lv_label_set_text(label3, "Go to Screen 4");
        
        #endif
    }
    lv_scr_load(screen3);

    Serial.println("create_security_screen End");
}


// イベントハンドラ関数
static void event_handler(lv_event_t * e) {
    lv_obj_t * btn = lv_event_get_target(e);
    lv_obj_t * label = lv_obj_get_child(btn, 0);

    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        buzzer_beep();
        Serial.println("buzzer_beep_on");

        if(strcmp(lv_label_get_text(label), "Disarmed") == 0) {
            lv_label_set_text(label, "Armed");
            lv_obj_set_style_bg_color(btn, lv_color_make(0, 255, 0), 0); // 緑色に設定
            //ledOn = true; // LEDを点灯させる
            for (size_t i = 0; i < 8; i++)
            {
                gLedState.areaLeds[i] = true;
            }
        } else {
            lv_label_set_text(label, "Disarmed");
            lv_obj_set_style_bg_color(btn, lv_color_make(0, 0, 0), 0); // 黒色に設定
            for (size_t i = 0; i < 8; i++)
            {
                gLedState.areaLeds[i] = false;
            }
            
        }
            xSemaphoreGive(ledSemaphore); // セマフォを解放
    }
}


static void btn_event_handler3(lv_event_t* e) {
    //show_screen4(); // Screen 4へ遷移
    create_area_screen();
}
