#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include <WiFi.h>
#include <NTPClient.h>
#include "common.h"

lv_obj_t* time_label_area;

void create_area_screen(lv_obj_t *scr) {

    Serial.println("create_area_screen Start");

    // 時刻表示用のラベルを作成
    time_label_area = lv_label_create(scr);
    lv_obj_align(time_label_area, LV_ALIGN_TOP_MID, 0, 5); // 例: 画面の上中央に配置
    lv_label_set_text(time_label_area, "00:00"); // 初期テキスト
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


    //add_navigation_buttons(screen4, screen5, screen3);

    Serial.println("create_area_screen End");

}

