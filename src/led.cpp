#include <Wire.h>
//#include "Adafruit_MCP23017.h"
#include <Adafruit_MCP23X17.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "common.h"

//Adafruit_MCP23X17 mcp_0x21;
//Adafruit_MCP23X17 mcp_0x22;
//Adafruit_MCP23X17 mcp_local_input;
//Adafruit_MCP23X17 mcp_emg_LED;

SemaphoreHandle_t ledSemaphore; // セマフォを定義

// led.cpp
//bool ledOn = false; // グローバル変数を定義



#define EMG_INPUT_CH1 0
#define EMG_INPUT_CH2 1
#define EMG_INPUT_CH3 2
#define EMG_INPUT_CH4 3
#define EMG_INPUT_CH5 4
#define EMG_INPUT_CH6 5
#define EMG_INPUT_CH7 6
#define EMG_INPUT_CH8 7


LedState gLedState;

// LED点滅タスク
void blinkLedTask(void *parameter) {
    Serial.println("blinkLedTask Start");

  //ledState.areaLeds[0] = false;

    while (true) {
        if (xSemaphoreTake(ledSemaphore, portMAX_DELAY)) {
          // MCP23017デバイスのインスタンスを動的に作成
          Adafruit_MCP23X17* mcp_0x21 = new Adafruit_MCP23X17();

          mcp_0x21->begin_I2C(0x21, &Wire);
          for (int i = 0; i < 16; i++) {
            mcp_0x21->pinMode(i, OUTPUT);
          }
          // セマフォを取得できたらAREA_LEDを状態に応じて点灯/消灯
          for (int i = 0; i < 8; i++) {
              mcp_0x21->digitalWrite(i, gLedState.areaLeds[i]);
          }

          // 入力に応じてEMG_LEDを点灯/消灯
          for (int i = 0; i < 8; i++) {
            mcp_0x21->digitalWrite(i+8, gLedState.emgLeds[i]);
          }

          // MCP23017デバイスのインスタンスを解放
          delete mcp_0x21;
        }

    }
}




void led_setup() {
  Wire.begin(26, 25); // ESP32のIO26(SDA)とIO25(SCL)を指定


  #if 0
  mcp[0x20 - MCP_BASE_ADDR].begin_I2C(0x20, &Wire); // MCP23017のI2Cアドレスを指定（必要に応じて変更）
  
  // 入力に設定
  for (int i = 0; i < 8; i++) {
    mcp[0x20 - MCP_BASE_ADDR].pinMode(i, INPUT);
  }

  // 出力に設定
  for (int i = 8; i < 16; i++) {
    mcp[0x20 - MCP_BASE_ADDR].pinMode(i, OUTPUT);
  }



  mcp_0x21.begin_I2C(0x21, &Wire); // MCP23017のI2Cアドレスを指定（必要に応じて変更）
  
  // LEDを出力に設定
  for (int i = 0; i < 16; i++) {
    mcp_0x21.pinMode(i, OUTPUT);
  }

  mcp_0x22.begin_I2C(0x22, &Wire);  // MCP23017のI2Cアドレスを0x22として初期化

  // GPA0からGPA7までを入力として設定
  for (int i = 0; i < 16; i++) {
    mcp_0x22.pinMode(i, INPUT);
  }

  // GPAポートの割り込みを有効にする
  mcp_0x22.setupInterrupts(true, false, LOW);  // ミラーリング無し、オープンドレイン無し、アクティブロー

  // GPA0～GPA7の割り込みを有効にする
  for (int i = 0; i < 8; i++) {
      mcp_0x22.setupInterruptPin(i, CHANGE);  // 状態変化で割り込みを発生
  }

  #endif
  // セマフォの作成
  ledSemaphore = xSemaphoreCreateBinary();
  

  // LED点滅タスクを作成
  xTaskCreate(blinkLedTask, "Blink LED Task", 4096, NULL, 1, NULL);
}

