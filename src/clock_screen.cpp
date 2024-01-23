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




TFT_eSPI tft = TFT_eSPI(); // TFTのインスタンスを作成

void my_touchpad_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {


    uint16_t touchX, touchY;

    // タッチイベントの確認
    bool touched = tft.getTouch(&touchX, &touchY);

    

    if (touched) {

        data->point.x = touchX;
        data->point.y = touchY;
        data->state = LV_INDEV_STATE_PR; // プレス状態

        #ifdef DEBUG_MODE
        Serial.println("TFT getTouch!");
        // タッチ座標をシリアル出力
        Serial.print("Touch X: ");
        Serial.print(touchX);
        Serial.print(", Touch Y: ");
        Serial.println(touchY);
        #endif
    } else {
        data->state = LV_INDEV_STATE_REL; // リリース状態
    }

    //return false; // データが常に有効であることを示す

}


// ディスプレイフラッシュ関数
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    tft.pushColors((uint16_t *)&color_p->full, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}






// 時刻表示用のラベル
lv_obj_t *time_label;

// wifi状態のラベル
lv_obj_t *wifiStatus_label;

// ボタンの作成
lv_obj_t *settings_btn; 


// WiFiの資格情報
//const char* ssid = "googlemain";
//const char* password = "Fdsa@0813";
const char* ssid = "20230616me_IPv6";
const char* password = "asdf0616";


// NTPクライアントの設定
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600 * 9); // JSTの場合



static void btn_event_setting_cb(lv_event_t *event) {
    
    Serial.println("設定ボタン_イベントハンドラ呼び出し");
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED) {
        Serial.println("設定ボタンがクリックされました");

        lv_obj_del(time_label); // ボタンオブジェクトを削除
        lv_obj_del(wifiStatus_label); // ボタンオブジェクトを削除
        lv_obj_del(settings_btn); // ボタンオブジェクトを削除
        tenkey_setup();
    }
}



LV_FONT_DECLARE(jpFont04);



void clock_setup() {
    Serial.begin(115200); // シリアル通信の初期化
    Serial.println("clock_setup Start");

    // TFTの初期化
    tft.begin();
    tft.setRotation(1);

    uint16_t calData[5] = { 231, 3567, 344, 3355, 7 };
    tft.setTouch(calData);

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


    #if 1
    // タッチパッド入力デバイスを初期化して登録
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
    #endif


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

    // ボタンの作成
    settings_btn = lv_btn_create(lv_scr_act());
    lv_obj_add_style(settings_btn, &style1, 0); // スタイルを適用
    lv_obj_set_size(settings_btn, 100, 50); // ボタンのサイズ設定
    lv_obj_align(settings_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -10); // 画面の右下隅に配置
    lv_obj_add_event_cb(settings_btn, btn_event_setting_cb, LV_EVENT_CLICKED , NULL); // ボタンアクションの新しい設定方法

    // ボタンにラベルを追加
    lv_obj_t *settings_label = lv_label_create(settings_btn);
    lv_label_set_text(settings_label, "設定");

    // ... その他の初期化コード ...
    Serial.println("clock_setup end");
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
