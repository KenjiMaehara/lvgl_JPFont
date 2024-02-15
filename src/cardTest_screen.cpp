#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"


void onCardRead(card_data_t *card);

lv_obj_t *label;
lv_obj_t *typeLabel; // カードタイプ表示用の新しいラベル

lv_obj_t* time_label_card;

void create_cardTest_screen(lv_obj_t *scr) {

    Serial.println("create_cardTest_screen Start");

    // 時刻表示用のラベルを作成
    time_label_card = lv_label_create(scr);
    lv_obj_align(time_label_card, LV_ALIGN_TOP_MID, 0, 5); // 画面の上中央に配置
    lv_label_set_text(time_label_card, "00:00"); // 初期テキスト

    // カードデータ表示用のラベルを作成
    label = lv_label_create(scr); // ラベルオブジェクトを作成
    lv_label_set_text(label, "カードデータがここに表示されます"); // ラベルに初期テキストを設定
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -20); // 少し上に配置

    // カードタイプ表示用の新しいラベルを作成
    typeLabel = lv_label_create(scr);
    lv_label_set_text(typeLabel, "カードタイプ: 未識別");
    lv_obj_align(typeLabel, LV_ALIGN_CENTER, 0, 20); // カードデータラベルの少し下に配置

    // カード読み取りコールバックを設定
    card_set_cb(onCardRead);


    add_navigation_buttons(screen8, screen1, screen7);

    Serial.println("create_cardTest_screen End");
}


// カード読み取りコールバック関数
void onCardRead(card_data_t *card) {

  Serial.println("-------------onCardRead Start--------------");
  buzzer_beep(); // ブザーを鳴らす

  char dataStr[256]; // データを文字列に変換するためのバッファ
  size_t strLen = 0;
  for (size_t i = 0; i < card->data_length && strLen < sizeof(dataStr) - 3; i++) {
    sprintf(&dataStr[strLen], "%02X ", card->data[i]);
    strLen += 3;
  }
  lv_label_set_text(label, dataStr); // LVGLラベルにカードデータを設定


  // カードデータの長さからカードの種類を識別
  const char* cardTypeName = "type unknown";
  switch(card->data_length) {
      case 24: cardTypeName = "14443B"; break;
      case 36: cardTypeName = "FeliCa"; break;
      case 10: cardTypeName = "MIFARE(Type A)"; break;
      case 20: cardTypeName = "15693"; break;
      // 他の長さに対するケースも同様に追加
      default: break;
  }  
  // カードタイプラベルにテキストを設定
  lv_label_set_text(typeLabel, cardTypeName);

  lv_task_handler(); // LVGLタスクハンドラを呼び出して画面を更新
}