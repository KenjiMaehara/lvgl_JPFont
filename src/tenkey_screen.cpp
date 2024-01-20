//#pragma execution_character_set("utf-8")

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[320 * 10]; // 仮にディスプレイの垂直解像度を480ピクセルと仮定
static lv_disp_drv_t disp_drv;


static void btn_event_cb(lv_event_t *e);


static char number_str[64] = ""; // 数字を格納する文字列

LV_FONT_DECLARE(jpFont04);

void tenkey_setup() {

    lv_init();
    tft.begin();
    tft.setRotation(1); // ディスプレイの向きに合わせて調整

    // バッファのサイズを設定
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 320 * 10); // 解像度に基づいてバッファサイズを設定



    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 480; // ディスプレイの解像度を設定
    disp_drv.ver_res = 320;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    #if 0
    // タッチパッド入力デバイスを初期化して登録
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
    #endif

    // シンプルな数値キーパッドを作成
    lv_obj_t *btn;
    lv_obj_t *label;
    lv_obj_t *screen = lv_scr_act();


    for (int i = 0; i < 12; i++) {
        btn = lv_btn_create(screen); // シグネチャを更新
        lv_obj_set_size(btn, 80, 45);
        lv_obj_set_pos(btn, (i % 3) * 90 + 100, (i / 3) * 50 + 100);
        lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);
        label = lv_label_create(btn); // シグネチャを更新

        static lv_style_t button_label_style;
        lv_style_init(&button_label_style);
        lv_style_set_text_font(&button_label_style, &jpFont04);
        lv_obj_add_style(label, &button_label_style, 0);
        lv_obj_set_align(label, LV_ALIGN_CENTER); // テキストを中央に配置


        // 特定のボタンにテキストを設定
        if (i == 9) {
            lv_label_set_text(label, "*");
        } else if (i == 10) {
            lv_label_set_text(label, "0");
        } else if (i == 11) {
            lv_label_set_text(label, "#");
        } else {
            lv_label_set_text_fmt(label, "%d", i + 1);
        }
    }

    // 数字を表示するラベルを作成
    lv_obj_t *number_label = lv_label_create(screen); // シグネチャを更新
    lv_label_set_text(number_label, number_str);
    lv_obj_align(number_label, LV_ALIGN_TOP_MID, 0, 30);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, &jpFont04); // フォントサイズを大きく設定
    lv_obj_add_style(number_label, &style, 0);

    Serial.println("Setup End");
}


static void btn_event_cb(lv_event_t *e) {
    lv_obj_t *btn = lv_event_get_target(e);
    const char *txt = lv_label_get_text(lv_obj_get_child(btn, 0));
    strncat(number_str, txt, sizeof(number_str) - strlen(number_str) - 1);
    lv_obj_t *number_label = lv_obj_get_child(lv_scr_act(), -1);
    lv_label_set_text(number_label, number_str);
}