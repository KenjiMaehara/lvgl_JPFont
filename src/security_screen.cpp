#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"



void create_security_screen(lv_obj_t *scr) {
    // 防犯開始・解除のボタン作成
    // ...
    add_navigation_buttons(scr, screen4, screen2);
}



