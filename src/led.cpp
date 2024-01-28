#include <Wire.h>
//#include "Adafruit_MCP23017.h"
#include "Adafruit_MCP23X17.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

Adafruit_MCP23X17 mcp;

// led.cpp
bool ledOn = false; // グローバル変数を定義

// LED点滅タスク
void blinkLedTask(void *parameter) {
    Serial.println("blinkLedTask Start");

    // LEDを出力に設定
  for (int i = 0; i < 8; i++) {
    mcp.pinMode(i, OUTPUT);
  }

    while (true) {

        #if 0
        for (int i = 0; i < 8; i++) {
          mcp.digitalWrite(i, HIGH);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 1000ms待つ
        for (int i = 0; i < 8; i++) {
          mcp.digitalWrite(i, LOW);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 1000ms待つ}
        #endif

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
        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }
}

void led_setup() {
    Wire.begin(26, 25); // ESP32のIO26(SDA)とIO25(SCL)を指定
    mcp.begin_I2C(0x21, &Wire); // MCP23017のI2Cアドレスを指定（必要に応じて変更）

    // LED点滅タスクを作成
    xTaskCreate(blinkLedTask, "Blink LED Task", 2048, NULL, 1, NULL);
}

