#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "../common.h"
#include "security_screen.h"
#include "start_screen.h"
#include "clock_screen.h"


//static void btn_event_cb(lv_event_t *e);
static void event_handler(lv_event_t * e);
static lv_obj_t* screen3 = nullptr;

lv_obj_t* time_label_security; // セキュリティスクリーン用の時刻表示ラベル

void create_security_screen(void) {


    Serial.println("create_security_screen Start");

    if (screen3 == nullptr) {
        screen3 = lv_obj_create(NULL);
        lv_obj_t* btn3 = lv_btn_create(screen3);
        lv_obj_add_event_cb(btn3, event_handler, LV_EVENT_CLICKED, NULL);
        lv_obj_align(btn3, LV_ALIGN_CENTER, 0, 0);
        lv_obj_t* label3 = lv_label_create(btn3);
        lv_label_set_text(label3, "Go to Screen 1");
    }
    lv_scr_load(screen3);
 
    Serial.println("create_security_screen End");
}


// イベントハンドラ関数
static void event_handler(lv_event_t * e) {
    
        create_clock_screen();
}


