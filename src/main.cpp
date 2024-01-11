#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ

//#define DEBUG_MODE // デバッグモードを有効にする。デバッグが不要な場合はこの行をコメントアウト


void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p); // ILI9488用のflush関数

TFT_eSPI tft = TFT_eSPI(); // TFTインスタンスを作成

static lv_style_t style_pressed;


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

// 元の背景色を保持するための変数
static lv_color_t original_bg_color;
static bool color_initialized = false;

static lv_timer_t* current_timer = NULL; // 現在動作中のタイマーを追跡するための変数

// タイマーコールバック関数
static void timer_callback(lv_timer_t *timer) {
    lv_obj_set_style_bg_color(lv_scr_act(), original_bg_color, LV_PART_MAIN); // 元の背景色に戻す
    current_timer = NULL; // タイマーが完了したのでリセット
}

// シンプルなボタンイベントハンドラ
static void simple_button_event_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        Serial.println("Simple Button Pressed!");

        // 既存のタイマーが動作中であれば、新しいタイマーを作成しない
        if (!current_timer) {
            // 画面全体の背景色を赤に変更
            lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0xFF0000), LV_PART_MAIN);

            // 新しいタイマーを作成
            current_timer = lv_timer_create(timer_callback, 5000, NULL);
        }
    }
}

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[320 * 10]; // 描画バッファを定義


static lv_disp_drv_t disp_drv;

void setup() {
    Serial.begin(115200);  // シリアル通信を開始
    lv_init(); // LVGLを初期化
    tft.begin(); // TFTを初期化
    tft.setRotation(1); // 必要に応じて画面の向きを設定

    uint16_t calData[5] = { 231, 3567, 344, 3355, 7 };
    tft.setTouch(calData);

    //uint16_t calData[5] = { 275, 3620, 264, 3532, 1 };
    //tft.setTouch(calData);


    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 320 * 10);

    // ディスプレイドライバを設定

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 320;
    disp_drv.flush_cb = my_disp_flush; // ここでILI9488用のflush関数を設定
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /*Register at least one display before you register any input devices*/
    lv_disp_drv_register(&disp_drv);

    // 画面の初期背景色を白色に設定
    original_bg_color = lv_color_white();
    color_initialized = true;


    // タッチパッド入力デバイスを初期化して登録
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);           // 基本的な初期化
    indev_drv.type = LV_INDEV_TYPE_POINTER;  // タッチパッドはポインタータイプのデバイス
    indev_drv.read_cb = my_touchpad_read;    // タッチ読み取り関数を設定
    lv_indev_t *my_indev = lv_indev_drv_register(&indev_drv); // デバイスを登録

    // ボタン用のカスタムスタイルを定義する
    static lv_style_t custom_style;
    lv_style_init(&custom_style);
    lv_style_set_bg_color(&custom_style, lv_color_hex(0x00FF00)); // 背景色を緑に設定
    lv_style_set_border_color(&custom_style, lv_color_black()); // 枠線の色を黒に設定
    lv_style_set_border_width(&custom_style, 4); // 枠線の幅を4ピクセルに設定

    // シンプルなボタンを作成
    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, 200, 100);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(btn, simple_button_event_handler, LV_EVENT_ALL, NULL);

    // カスタムスタイルをボタンに適用する
    lv_obj_add_style(btn, &custom_style, 0);

}

void loop() {

    lv_timer_handler(); // LVGLタイマーを処理
    delay(5);
}

// ILI9488用のflush関数（ディスプレイドライバの設定に必要）
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}
