#include <time.h>
#include "lvgl.h"
#include "common.h"

// グローバル変数または共通クラス内での宣言
lv_obj_t* time_labels[8]; // 最大8つの画面用のラベルポインタ配列
int current_screen = 0; // 現在表示中の画面のインデックス

// 時刻更新関数
void update_time(void) {
    time_t now = time(NULL);
    struct tm *now_tm = localtime(&now);
    char timeString[64];
    strftime(timeString, sizeof(timeString), "%H:%M:%S", now_tm);


    // キーパッド画面の時刻表示用ラベルのテキストを更新
    if (time_label_keypad) {
        lv_label_set_text(time_label_keypad, timeString);
    }

    // セキュリティ画面の時刻表示用ラベルのテキストを更新
    if (time_label_security) {
        lv_label_set_text(time_label_security, timeString);
    }

    if (time_label_clock) {
        lv_label_set_text(time_label_clock, timeString);
    }
    
}

// タイマーコールバック関数
static void timer_update_time(lv_timer_t * timer) {
    update_time();
}

// アプリケーションの初期化部分でタイマーを設定
void setup_time_update() {
    lv_timer_create(timer_update_time, 1000, NULL); // 1秒ごとに更新
}

// 画面切替時に現在アクティブな画面インデックスを設定する関数
void switch_to_screen(int screen_index) {
    current_screen = screen_index;
    // 画面切り替えのロジック...
}
