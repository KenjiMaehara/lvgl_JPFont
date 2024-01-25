#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"



void create_fourth_screen(lv_obj_t *scr) {
    // 4番目のスクリーンの内容
    // ...
    add_navigation_buttons(scr, screen1, screen3);
}
