#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include <WiFi.h>
#include <NTPClient.h>
#include "common.h"


void create_keypad_screen(lv_obj_t *scr);
void create_clock_screen(lv_obj_t *scr);
void create_security_screen(lv_obj_t *scr);
void create_fourth_screen(lv_obj_t *scr);
void update_clock(lv_timer_t *timer);
void btn_event_cb(lv_event_t *e);

lv_obj_t *screen1;
lv_obj_t *screen2;
lv_obj_t *screen3;
lv_obj_t *screen4;


void screen_setup() {
    //lv_init();
    //ili9488_init();

    screen1 = lv_obj_create(NULL);
    create_keypad_screen(screen1);

    screen2 = lv_obj_create(NULL);
    create_clock_screen(screen2);

    screen3 = lv_obj_create(NULL);
    create_security_screen(screen3);

    screen4 = lv_obj_create(NULL);
    create_fourth_screen(screen4);

    lv_scr_load(screen1);

    lv_timer_create(update_clock, 1000, NULL);
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



void create_security_screen(lv_obj_t *scr) {
    // 防犯開始・解除のボタン作成
    // ...
    add_navigation_buttons(scr, screen4, screen2);
}

void create_fourth_screen(lv_obj_t *scr) {
    // 4番目のスクリーンの内容
    // ...
    add_navigation_buttons(scr, screen1, screen3);
}

void btn_event_cb(lv_event_t *e) {

    Serial.println("btn_event_cb start");

    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_t *next_screen = (lv_obj_t *)lv_obj_get_user_data(btn);

    if (next_screen != NULL) {
        Serial.println("next_screen");
        lv_scr_load(next_screen);
    }

    Serial.println("btn_event_cb end");
}

void update_clock(lv_timer_t *timer) {
    // 現在時刻を取得し、screen2のラベルを更新
    // ...
}