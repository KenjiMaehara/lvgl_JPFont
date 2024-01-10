#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ

TFT_eSPI tft = TFT_eSPI(); // TFTのインスタンスを作成

// ディスプレイフラッシュ関数
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    tft.pushColors((uint16_t *)&color_p->full, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[320 * 10]; // 仮にディスプレイの垂直解像度を480ピクセルと仮定
static lv_disp_drv_t disp_drv;

void setup() {
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

    // LVGLラベルの作成
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello, LVGL World02!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    Serial.println("Setup End");
}

void loop() {
    lv_task_handler();
    delay(5);
}
