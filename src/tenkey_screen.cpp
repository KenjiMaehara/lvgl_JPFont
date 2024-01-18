//#pragma execution_character_set("utf-8")

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ


void tenkey_setup() {
    Serial.begin(115200); // シリアル通信の初期化
    Serial.println("Setup Start");

    lv_init();
    tft.begin();
    tft.setRotation(1); // ディスプレイの向きに合わせて調整

    // バッファのサイズを設定（解像度に基づいて）


    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 320 * 3);

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 480; // ディスプレイの解像度を設定
    disp_drv.ver_res = 320;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);


    // タッチパッド入力デバイスを初期化して登録
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);           // 基本的な初期化
    indev_drv.type = LV_INDEV_TYPE_POINTER;  // タッチパッドはポインタータイプのデバイス
    indev_drv.read_cb = my_touchpad_read;    // タッチ読み取り関数を設定
    lv_indev_t *my_indev = lv_indev_drv_register(&indev_drv); // デバイスを登録




    // スタイルの作成
    static lv_style_t style;
    lv_style_init(&style);

    // 枠線の色と太さの設定
    lv_style_set_border_color(&style, lv_color_black()); // 枠線の色を黒に設定
    lv_style_set_border_width(&style, 2); // 枠線の太さを2pxに設定

    // ボタンを作成
    lv_obj_t * btn = lv_btn_create(lv_scr_act());     // scr の代わりに lv_scr_act() を使用
    int width = 200;
    int height = 100;
    lv_obj_set_size(btn, width, height);

    // スタイルの適用 (修正)
    lv_obj_add_style(btn, &style, 0); // ボタンにスタイルを適用

    lv_obj_align(btn, LV_ALIGN_CENTER, 0, -20);       // LV_ALIGN_CENTER または他の適切な定数を使用
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED , NULL); // ボタンアクションの新しい設定方法

    // ボタンにラベルを追加
    //lv_obj_t * label = lv_label_create(btn, NULL);


    static lv_style_t style1;

    lv_style_init(&style1);
    lv_style_set_text_font(&style1, &jpFont04);

    lv_obj_t *label = lv_label_create(btn);
    lv_obj_add_style(label, &style1, 0);

    // ラベルのサイズを設定
    //lv_obj_set_size(label, 200, 200);

    // テキストの折り返しを設定
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);

    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    // テキストを設定
    lv_label_set_text(label, "テスト2");

    Serial.println("Setup End");
}


