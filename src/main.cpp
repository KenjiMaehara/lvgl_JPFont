//#pragma execution_character_set("utf-8")

#include <Arduino.h>
//#include <lvgl.h>
#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include "common.h"
//#include <WiFi.h> // WiFiライブラリをインクルード

TFT_eSPI tft = TFT_eSPI();  // TFTインスタンスを作成

int currentScreen = 0;  // 現在のスクリーン番号


void setup() {
    //start_setup();
    tft.init();  // TFTの初期化
    tft.setRotation(1);  // 画面の向きを設定
    drawScreen(currentScreen);  // 最初のスクリーンを描画

    //WiFi.begin(ssid, password);
}



void loop() {

}


// 画面を描画する関数
void drawScreen(int screenNumber) {
  tft.fillScreen(TFT_BLACK);  // 画面をクリア

  // 画面に応じたコンテンツを描画
  String message = "これは画面" + String(screenNumber + 1) + "です";
  tft.setCursor(10, 50);  // テキストの位置を設定
  tft.setTextColor(TFT_WHITE);  // テキストの色を設定
  tft.setTextSize(2);  // テキストのサイズを設定
  tft.println(message);  // テキストを描画

  // "next"と"back"ボタンを描画
  drawButton("Next", 120, 200);  // "next"ボタン
  drawButton("Back", 0, 200);    // "back"ボタン
}

// ボタンを描画する関数
void drawButton(String title, int x, int y) {
  tft.drawRect(x, y, 100, 40, TFT_WHITE);  // ボタンの枠を描画
  tft.setCursor(x + 10, y + 10);          // テキストの位置を設定
  tft.setTextColor(TFT_WHITE);            // テキストの色を設定
  tft.setTextSize(2);                     // テキストのサイズを設定
  tft.print(title);                       // テキストを描画
}

// 画面遷移のための関数 (実際のボタン操作に合わせて調整する必要があります)
void nextScreen() {
  currentScreen = (currentScreen + 1) % 4;  // 次の画面番号
  drawScreen(currentScreen);                // 画面を描画
}

void previousScreen() {
  if (currentScreen == 0) currentScreen = 3;
  else currentScreen--;
  drawScreen(currentScreen);  // 画面を描画
}