#include <Wire.h>
//#include "Adafruit_MCP23017.h"
#include <Adafruit_MCP23X17.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "common.h"

Adafruit_MCP23X17 mcp[5];
//Adafruit_MCP23X17 mcp_local_input;
//Adafruit_MCP23X17 mcp_emg_LED;

SemaphoreHandle_t ledSemaphore; // セマフォを定義

// led.cpp
bool ledOn = false; // グローバル変数を定義



#define EMG_INPUT_CH1 0
#define EMG_INPUT_CH2 1
#define EMG_INPUT_CH3 2
#define EMG_INPUT_CH4 3
#define EMG_INPUT_CH5 4
#define EMG_INPUT_CH6 5
#define EMG_INPUT_CH7 6
#define EMG_INPUT_CH8 7

// LED点滅タスク
void blinkLedTask(void *parameter) {
    Serial.println("blinkLedTask Start");

    while (true) {

      if (ledOn) {
          // 全てのLEDを点灯
          for (int i = 0; i < 8; i++) {
              mcp[0x21 - MCP_BASE_ADDR].digitalWrite(i, HIGH);
          }
      } else {
          // 全てのLEDを消灯
          for (int i = 0; i < 8; i++) {
              mcp[0x21 - MCP_BASE_ADDR].digitalWrite(i, LOW);
          }
      }

    }
}




void led_setup() {
  Wire.begin(26, 25); // ESP32のIO26(SDA)とIO25(SCL)を指定

  mcp[0x20 - MCP_BASE_ADDR].begin_I2C(0x20, &Wire); // MCP23017のI2Cアドレスを指定（必要に応じて変更）
  
  // 入力に設定
  for (int i = 0; i < 8; i++) {
    mcp[0x20 - MCP_BASE_ADDR].pinMode(i, INPUT);
  }

  // 出力に設定
  for (int i = 8; i < 16; i++) {
    mcp[0x20 - MCP_BASE_ADDR].pinMode(i, OUTPUT);
  }



  mcp[0x21 - MCP_BASE_ADDR].begin_I2C(0x21, &Wire); // MCP23017のI2Cアドレスを指定（必要に応じて変更）
  
  // LEDを出力に設定
  for (int i = 0; i < 16; i++) {
    mcp[0x21 - MCP_BASE_ADDR].pinMode(i, OUTPUT);
  }

  mcp[0x22 - MCP_BASE_ADDR].begin_I2C(0x22, &Wire);  // MCP23017のI2Cアドレスを0x22として初期化

  // GPA0からGPA7までを入力として設定
  for (int i = 0; i < 16; i++) {
    mcp[0x22 - MCP_BASE_ADDR].pinMode(i, INPUT);
  }

  // GPAポートの割り込みを有効にする
  mcp[0x22 - MCP_BASE_ADDR].setupInterrupts(true, false, LOW);  // ミラーリング無し、オープンドレイン無し、アクティブロー

  // GPA0～GPA7の割り込みを有効にする
  for (int i = 0; i < 8; i++) {
      mcp[0x22 - MCP_BASE_ADDR].setupInterruptPin(i, CHANGE);  // 状態変化で割り込みを発生
  }


  // セマフォの作成
  //ledSemaphore = xSemaphoreCreateBinary();
  

  // LED点滅タスクを作成
  xTaskCreate(blinkLedTask, "Blink LED Task", 4096, NULL, 1, NULL);
}

