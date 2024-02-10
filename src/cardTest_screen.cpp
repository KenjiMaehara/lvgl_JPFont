#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"


void onCardRead(card_data_t *card);

lv_obj_t *label;

void create_cardTest_screen(lv_obj_t *scr) {

    Serial.println("create_cardTest_screen Start");

    // カードデータ表示用のラベルを作成
    label = lv_label_create(scr); // ラベルオブジェクトを作成
    lv_label_set_text(label, "カードデータがここに表示されます"); // ラベルに初期テキストを設定
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0); // ラベルを画面の中心に配置


    // カード読み取りコールバックを設定
    card_set_cb(onCardRead);


    add_navigation_buttons(screen8, screen1, screen7);

    Serial.println("create_cardTest_screen End");
}


// カード読み取りコールバック関数
void onCardRead(card_data_t *card) {

  Serial.println("-------------onCardRead Start--------------");

  char dataStr[256]; // データを文字列に変換するためのバッファ
  size_t strLen = 0;
  for (size_t i = 0; i < card->data_length && strLen < sizeof(dataStr) - 3; i++) {
    sprintf(&dataStr[strLen], "%02X ", card->data[i]);
    strLen += 3;
  }
  lv_label_set_text(label, dataStr); // LVGLラベルにカードデータを設定
  lv_task_handler(); // LVGLタスクハンドラを呼び出して画面を更新
}