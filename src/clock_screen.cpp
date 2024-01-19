#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include <WiFi.h>
#include <NTPClient.h>
#include "common.h"


// LVGL用のディスプレイバッファを定義
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[320 * 10];
static lv_disp_drv_t disp_drv;

// 時刻表示用のラベル
lv_obj_t *time_label;



// WiFiの資格情報
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// NTPクライアントの設定
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600 * 9); // JSTの場合




void clock_setup() {
    // TFTの初期化
    tft.begin();
    tft.setRotation(1);

    // LVGLの初期化
    lv_init();

    // バッファのサイズを設定
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 320 * 10); // 解像度に基づいてバッファサイズを設定  
    // LVGLのディスプレイドライバー設定 

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 480; // ディスプレイの解像度を設定
    disp_drv.ver_res = 320;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // 時刻表示用ラベルの作成
    time_label = lv_label_create(lv_scr_act());
    lv_label_set_text(time_label, "00:00");
    lv_obj_align(time_label, LV_ALIGN_CENTER, 0, 0);  
    // NTPクライアントの設定など...
}



// NTP更新用のタスク
void ntpUpdateTask(void *pvParameters) {
  for (;;) {
    if (WiFi.status() == WL_CONNECTED) {
      timeClient.update();
      String currentTime = timeClient.getFormattedTime();
      lv_label_set_text(time_label, currentTime.c_str());
    }
    vTaskDelay(3600000 / portTICK_PERIOD_MS); // 1時間ごと
  }
}
