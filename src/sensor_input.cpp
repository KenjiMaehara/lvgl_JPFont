#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "common.h"


int lastState[8];  // 以前の状態を格納する配列

void pinMonitorTask(void *pvParameters);
void handlePinChange(int pin, int state);


void sensor_input_setup() {

  // GPA0からGPA7を入力として設定し、プルアップ抵抗を有効化
  for (int i = 0; i < 8; i++) {

    lastState[i] = mcp[0x21 - MCP_BASE_ADDR].digitalRead(i);
  }

  // ピン監視タスクの作成
  xTaskCreate(pinMonitorTask, "Pin Monitor", 1000, NULL, 1, NULL);
}


// ピンの状態を監視するタスク
void pinMonitorTask(void *pvParameters) {
  for (;;) {
    for (int i = 0; i < 8; i++) {
      int currentState = mcp[0x21 - MCP_BASE_ADDR].digitalRead(i);
      if (currentState != lastState[i]) {
        handlePinChange(i, currentState);
        lastState[i] = currentState;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100));  // ポーリング間隔
  }
}

// ピンの変化を処理するハンドラ
void handlePinChange(int pin, int state) {
  // ここにピンの変化に応じた処理を記述
}
