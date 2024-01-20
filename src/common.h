#ifndef COMMON_H
#define COMMON_H

#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
// ここにextern宣言を追加
//extern TFT_eSPI tft; // 例えば TFT_eSPI オブジェクトの宣言
extern void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
extern void my_touchpad_read(lv_indev_drv_t *drv, lv_indev_data_t *data);
extern int count;
extern void tenkey_setup();
extern TFT_eSPI tft; // TFTのインスタンスを作成
extern void clock_setup();
extern void start_setup();
extern const lv_font_t jpFont04;


extern void ntpUpdateTask(void *pvParameters);

#endif