#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include <WiFi.h>
#include <NTPClient.h>
#include "common.h"


static void btn_event_cb(lv_event_t *e);



void create_area_screen(lv_obj_t *scr) {

    Serial.println("create_area_screen Start");
    /* アクティブなスクリーンを取得 */
    //lv_obj_t * scr = lv_scr_act();

    /* タイトルラベルを作成 */
    lv_obj_t * title_label = lv_label_create(scr);
    lv_label_set_text(title_label, "Area map");
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10);

    /* キーパッドコンテナを作成 */
    lv_obj_t * keypad_cont = lv_obj_create(scr);
    lv_obj_set_size(keypad_cont, 320, 240);
    lv_obj_align(keypad_cont, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_layout(keypad_cont, LV_LAYOUT_GRID);

    /* ボタンマトリックスを作成 */
    static const char * btn_map[] = {"1", "2", "3", "4", "\n", "5", "6", "7", "8", ""};
    lv_obj_t * btnm = lv_btnmatrix_create(keypad_cont);
    lv_btnmatrix_set_map(btnm, btn_map);
    lv_obj_set_size(btnm, 300, 200);
    lv_obj_align(btnm, LV_ALIGN_CENTER, 0, 0);




    // NEXTボタンを作成
    lv_obj_t *btn = lv_btn_create(scr);
    lv_obj_set_pos(btn, 360, 240); // ボタンの位置を設定
    lv_obj_set_size(btn, 100, 50); // ボタンのサイズを設定

    // ボタンのラベルを作成
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "next");

    // ボタンのイベントハンドラを設定
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);

    Serial.println("create_area_screen End");

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
