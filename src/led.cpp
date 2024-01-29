#include <Wire.h>
//#include "Adafruit_MCP23017.h"
#include "Adafruit_MCP23X17.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "common.h"

Adafruit_MCP23X17 mcp_Secur_LED;
Adafruit_MCP23X17 mcp_local_input;
Adafruit_MCP23X17 mcp_emg_LED;

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
                  mcp_Secur_LED.digitalWrite(i, HIGH);
              }
          } else {
              // 全てのLEDを消灯
              for (int i = 0; i < 8; i++) {
                  mcp_Secur_LED.digitalWrite(i, LOW);
              }
          }
        }

    }
}

void led_setup() {
    Wire.begin(); // I2Cの初期化

    // 0x20アドレスのMCP23017のすべてのピンを出力として設定
    for (int i = 0; i < 16; i++) {
        mcp[0x20].pinMode(i, OUTPUT);
    }

    // 0x21アドレスのMCP23017のすべてのピンを出力として設定
    for (int i = 0; i < 16; i++) {
        mcp[0x21].pinMode(i, OUTPUT);
    }

    // 0x23アドレスのMCP23017のすべてのピンを出力として設定
    for (int i = 0; i < 16; i++) {
        mcp[0x23].pinMode(i, OUTPUT);
    }

    // 0x24アドレスのMCP23017のすべてのピンを出力として設定
    for (int i = 0; i < 16; i++) {
        mcp[0x24].pinMode(i, OUTPUT);
    }

  // セマフォの作成
  ledSemaphore = xSemaphoreCreateBinary();
  

  // LED点滅タスクを作成
  xTaskCreate(blinkLedTask, "Blink LED Task", 2048, NULL, 1, NULL);
}

