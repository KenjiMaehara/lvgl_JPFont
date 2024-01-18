//#pragma execution_character_set("utf-8")

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ


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

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[320 * 10]; // 仮にディスプレイの垂直解像度を480ピクセルと仮定
static lv_disp_drv_t disp_drv;


int count = 0;


static void btn_event_cb(lv_event_t *event) {
    count++;
    if (count > 1)
    {
        count = 0;
    }
    
    Serial.println("イベントハンドラ呼び出し");
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED) {
        Serial.println("ボタンがクリックされました");

        // 画面全体を赤色に変更
        if(count % 2 == 0){
            lv_obj_set_style_bg_color(lv_scr_act(), lv_color_make(0, 0, 255), LV_PART_MAIN);
        }else{
            lv_obj_set_style_bg_color(lv_scr_act(), lv_color_make(255, 0, 0), LV_PART_MAIN);
        }
    }
}




LV_FONT_DECLARE(jpFont04);

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


    // タッチパッド入力デバイスを初期化して登録
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);           // 基本的な初期化
    indev_drv.type = LV_INDEV_TYPE_POINTER;  // タッチパッドはポインタータイプのデバイス
    indev_drv.read_cb = my_touchpad_read;    // タッチ読み取り関数を設定
    lv_indev_t *my_indev = lv_indev_drv_register(&indev_drv); // デバイスを登録




    // スタイルの作成
    static lv_style_t style;
    lv_style_init(&style);

    // 枠線の色と太さの設定
    lv_style_set_border_color(&style, lv_color_black()); // 枠線の色を黒に設定
    lv_style_set_border_width(&style, 2); // 枠線の太さを2pxに設定

    // ボタンを作成
    lv_obj_t * btn = lv_btn_create(lv_scr_act());     // scr の代わりに lv_scr_act() を使用
    int width = 200;
    int height = 100;
    lv_obj_set_size(btn, width, height);

    // スタイルの適用 (修正)
    lv_obj_add_style(btn, &style, 0); // ボタンにスタイルを適用

    lv_obj_align(btn, LV_ALIGN_CENTER, 0, -20);       // LV_ALIGN_CENTER または他の適切な定数を使用
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED , NULL); // ボタンアクションの新しい設定方法

    // ボタンにラベルを追加
    //lv_obj_t * label = lv_label_create(btn, NULL);


    static lv_style_t style1;

    lv_style_init(&style1);
    lv_style_set_text_font(&style1, &jpFont04);

    lv_obj_t *label = lv_label_create(btn);
    lv_obj_add_style(label, &style1, 0);

    // ラベルのサイズを設定
    //lv_obj_set_size(label, 200, 200);

    // テキストの折り返しを設定
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);

    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    // テキストを設定
    lv_label_set_text(label, "テスト1");

    Serial.println("Setup End");
}




void tenkey_setup() {
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


    // タッチパッド入力デバイスを初期化して登録
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);           // 基本的な初期化
    indev_drv.type = LV_INDEV_TYPE_POINTER;  // タッチパッドはポインタータイプのデバイス
    indev_drv.read_cb = my_touchpad_read;    // タッチ読み取り関数を設定
    lv_indev_t *my_indev = lv_indev_drv_register(&indev_drv); // デバイスを登録




    // スタイルの作成
    static lv_style_t style;
    lv_style_init(&style);

    // 枠線の色と太さの設定
    lv_style_set_border_color(&style, lv_color_black()); // 枠線の色を黒に設定
    lv_style_set_border_width(&style, 2); // 枠線の太さを2pxに設定

    // ボタンを作成
    lv_obj_t * btn = lv_btn_create(lv_scr_act());     // scr の代わりに lv_scr_act() を使用
    int width = 200;
    int height = 100;
    lv_obj_set_size(btn, width, height);

    // スタイルの適用 (修正)
    lv_obj_add_style(btn, &style, 0); // ボタンにスタイルを適用

    lv_obj_align(btn, LV_ALIGN_CENTER, 0, -20);       // LV_ALIGN_CENTER または他の適切な定数を使用
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED , NULL); // ボタンアクションの新しい設定方法

    // ボタンにラベルを追加
    //lv_obj_t * label = lv_label_create(btn, NULL);


    static lv_style_t style1;

    lv_style_init(&style1);
    lv_style_set_text_font(&style1, &jpFont04);

    lv_obj_t *label = lv_label_create(btn);
    lv_obj_add_style(label, &style1, 0);

    // ラベルのサイズを設定
    //lv_obj_set_size(label, 200, 200);

    // テキストの折り返しを設定
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);

    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    // テキストを設定
    lv_label_set_text(label, "テスト2");

    Serial.println("Setup End");
}




void loop() {
    lv_task_handler();
    delay(5);
}
