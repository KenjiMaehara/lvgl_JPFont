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

// wifi状態のラベル
lv_obj_t *wifiStatus_label;



// WiFiの資格情報
//const char* ssid = "googlemain";
//const char* password = "Fdsa@0813";
const char* ssid = "20230616me_IPv6";
const char* password = "asdf0616";


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


    static lv_style_t style1;
    lv_style_init(&style1);
    lv_style_set_text_font(&style1, &jpFont04);
  
    static lv_style_t style2;
    lv_style_init(&style2);
    lv_style_set_text_font(&style2, &lv_font_montserrat_48);


    // 時刻表示用ラベルの作成
    time_label = lv_label_create(lv_scr_act());
    lv_obj_add_style(time_label, &style2, 0); // スタイルを適用
    lv_label_set_text(time_label, "00:00");
    lv_obj_align(time_label, LV_ALIGN_CENTER, 0, 0);  
   
    // wifi状態表示用ラベルの作成
    wifiStatus_label = lv_label_create(lv_scr_act());
    lv_obj_add_style(wifiStatus_label, &style1, 0); // スタイルを適用
    lv_label_set_text(wifiStatus_label, "WiFi接続中...");
    lv_obj_align(wifiStatus_label, LV_ALIGN_CENTER, 0, 0);

}



// NTP更新用のタスク
void ntpUpdateTask(void *pvParameters) {
  Serial.println("タスク_ntpUpdateTask_start");

  for (;;) {
    Serial.println("test_ntpUpdateTask");
    vTaskDelay(1000); 
    #if 1
    if (WiFi.status() == WL_CONNECTED) {
      timeClient.update();
      String currentTime = timeClient.getFormattedTime();
      if (time_label != NULL) {
        lv_label_set_text(time_label, currentTime.c_str());
      }
      if (wifiStatus_label != NULL)
      {
        lv_label_set_text(wifiStatus_label, " ");
      }
      
    } else {
      Serial.println("WiFiが接続されていません");
      // WiFiに接続できていない場合は警告メッセージを表示
      // 使用前にNULLでないことをチェック
      if (time_label != NULL) {
        lv_label_set_text(wifiStatus_label, "NTP接続不可 確認してください！");
      }
    }
    //vTaskDelay(3600000 / portTICK_PERIOD_MS); // 1時間ごと
    #endif
  }
}
