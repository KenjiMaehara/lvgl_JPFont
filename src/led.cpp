#include <Wire.h>
//#include "Adafruit_MCP23017.h"
#include <Adafruit_MCP23X17.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "common.h"



SemaphoreHandle_t ledSemaphore; // セマフォを定義
LedState gLedState;

// MCP23017デバイスの静的インスタンス
Adafruit_MCP23X17 mcp_0x21;

// LED点滅タスク
void blinkLedTask(void *parameter) {

  Serial.println("--------------blinkLedTask Start--------------");

  // 静的に割り当てられたインスタンスの初期化
  mcp_0x21.begin_I2C(0x21, &Wire);
  for (int i = 0; i < 16; i++) {
      mcp_0x21.pinMode(i, OUTPUT);
  }

  //ledState.areaLeds[0] = false;
  while (true) {
      if (xSemaphoreTake(ledSemaphore, portMAX_DELAY)) {
      // セマフォを取得できたらAREA_LEDを状態に応じて点灯/消灯
      for (int i = 0; i < 8; i++) {
           mcp_0x21.digitalWrite(i, gLedState.areaLeds[i]);
      }
      // 入力に応じてEMG_LEDを点灯/消灯
       for (int i = 0; i < 8; i++) {
           mcp_0x21.digitalWrite(i+8, gLedState.emgLeds[i]);
      }
    }
  }
}




void led_setup() {
  Wire.begin(26, 25); // ESP32のIO26(SDA)とIO25(SCL)を指定
  //Wire.begin(25, 26); // ESP32のIO26(SDA)とIO25(SCL)を指定

  // セマフォの作成
  ledSemaphore = xSemaphoreCreateBinary();
  
  #ifdef ENABLE_LED_TASK
  // LED点滅タスクを作成
  xTaskCreate(blinkLedTask, "Blink LED Task", 1024 * 4, NULL, 1, NULL);
  #endif
}

