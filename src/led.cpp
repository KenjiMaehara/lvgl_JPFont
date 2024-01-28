#include <Wire.h>
//#include "Adafruit_MCP23017.h"
#include "Adafruit_MCP23X17.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

Adafruit_MCP23X17 mcp;

// LED点滅タスク
void blinkLedTask(void *parameter) {
  for (int i = 0; i < 8; i++) {
    mcp.pinMode(i, OUTPUT);
  }

  while (true) {
    for (int i = 0; i < 8; i++) {
      mcp.digitalWrite(i, HIGH);
      vTaskDelay(1000 / portTICK_PERIOD_MS); // 1000ms待つ
      mcp.digitalWrite(i, LOW);
    }
  }
}

void led_setup() {
  Wire.begin(); // I2Cの初期化
  mcp.begin_I2C();  // MCP23017の初期化

  // LED点滅タスクを作成
  xTaskCreate(blinkLedTask, "Blink LED Task", 2048, NULL, 1, NULL);
}

