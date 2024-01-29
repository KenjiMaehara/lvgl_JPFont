#include <Wire.h>
//#include "Adafruit_MCP23017.h"
#include "Adafruit_MCP23X17.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

Adafruit_MCP23X17 mcp;
SemaphoreHandle_t ledSemaphore; // セマフォを定義

// led.cpp
bool ledOn = false; // グローバル変数を定義

// LED点滅タスク
void blinkLedTask(void *parameter) {
    Serial.println("blinkLedTask Start");

    while (true) {
        if (xSemaphoreTake(ledSemaphore, portMAX_DELAY)) {
          // セマフォを取得できたらLEDを点滅
          if (ledOn) {
              // 全てのLEDを点灯
              for (int i = 0; i < 8; i++) {
                  mcp.digitalWrite(i, HIGH);
              }
          } else {
              // 全てのLEDを消灯
              for (int i = 0; i < 8; i++) {
                  mcp.digitalWrite(i, LOW);
              }
          }
        }

    }
}

void led_setup() {
  Wire.begin(26, 25); // ESP32のIO26(SDA)とIO25(SCL)を指定
  mcp.begin_I2C(0x21, &Wire); // MCP23017のI2Cアドレスを指定（必要に応じて変更）
  
  // LEDを出力に設定
  for (int i = 0; i < 8; i++) {
    mcp.pinMode(i, OUTPUT);
  }

  // セマフォの作成
  ledSemaphore = xSemaphoreCreateBinary();
  

  // LED点滅タスクを作成
  xTaskCreate(blinkLedTask, "Blink LED Task", 2048, NULL, 1, NULL);
}

