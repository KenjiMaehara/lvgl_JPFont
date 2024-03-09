#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include <WiFi.h>
#include <NTPClient.h>
#include "common.h"
#include "clock_screen.h"
#include "area_screen.h"

lv_obj_t* time_label_area;
static lv_obj_t* screen4 = nullptr;
static void go_to_next_screen_event_handler(lv_event_t * e);

void create_area_screen(void) {

    Serial.println("create_area_screen Start");

    if (screen4 == nullptr) {
        screen4 = lv_obj_create(NULL);

        // 時刻表示用のラベルを作成
        time_label_area = lv_label_create(screen4);
        lv_obj_align(time_label_area, LV_ALIGN_TOP_MID, 0, 5); // 例: 画面の上中央に配置
        lv_label_set_text(time_label_area, "00:00"); // 初期テキスト
        /* アクティブなスクリーンを取得 */
        //lv_obj_t * scr = lv_scr_act();

        /* タイトルラベルを作成 */
        lv_obj_t * title_label = lv_label_create(screen4);
        lv_label_set_text(title_label, "Area map");
        lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10);

        /* キーパッドコンテナを作成 */
        lv_obj_t * keypad_cont = lv_obj_create(screen4);
        lv_obj_set_size(keypad_cont, 320, 240);
        lv_obj_align(keypad_cont, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_layout(keypad_cont, LV_LAYOUT_GRID);

        /* ボタンマトリックスを作成 */
        static const char * btn_map[] = {"1", "2", "3", "4", "\n", "5", "6", "7", "8", ""};
        lv_obj_t * btnm = lv_btnmatrix_create(keypad_cont);
        lv_btnmatrix_set_map(btnm, btn_map);
        lv_obj_set_size(btnm, 300, 200);
        lv_obj_align(btnm, LV_ALIGN_CENTER, 0, 0);

        lv_obj_t* btn3 = lv_btn_create(screen4);
        lv_obj_add_event_cb(btn3, go_to_next_screen_event_handler, LV_EVENT_CLICKED, NULL);
        lv_obj_align(btn3, LV_ALIGN_TOP_LEFT, 10, 10); // ボタンの位置を画面の左上に設定
        lv_obj_t* label3 = lv_label_create(btn3);
        lv_label_set_text(label3, "Go to Screen 4");

    }

    //add_navigation_buttons(screen4, screen5, screen3);
    lv_scr_load(screen4);
    Serial.println("create_area_screen End");

}


static void go_to_next_screen_event_handler(lv_event_t * e) {
    delete_previous_screen(&screen4);
    create_clock_screen();
}
