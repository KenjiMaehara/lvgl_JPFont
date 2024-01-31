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

    lastState[i] = mcp[0x20 - MCP_BASE_ADDR].digitalRead(i);
  }

  // ピン監視タスクの作成
  xTaskCreate(pinMonitorTask, "Pin Monitor", 10000, NULL, 1, NULL);
}


// ピンの状態を監視するタスク
void pinMonitorTask(void *pvParameters) {

  Serial.println("pinMonitorTask Start");

  unsigned long lastDebugTime = 0;  // 最後にデバッグ出力を行った時刻を記録する変数
  int lastPin7State = -1;  // Pin 7の最後の状態を保存 (-1は未初期化を表す)
  
  for (;;) {

    unsigned long currentTime = millis();

    // 5秒ごとにデバッグメッセージを出力
    if (currentTime - lastDebugTime >= 5000) {
      Serial.println("pinMonitorTask is running");
      lastDebugTime = currentTime;  // 最後のデバッグ出力時刻を更新
    }

    // アドレス0x22のMCP23017のピン7の状態を読み取る
    int currentPin7State = mcp[0x20 - MCP_BASE_ADDR].digitalRead(7);

    // ピン7の状態をモニター
    if (currentPin7State != lastPin7State) {
      Serial.print("Pin 7 changed to: ");
      Serial.println(currentPin7State);
      lastPin7State = currentPin7State;
    }

    // 他のピンの状態をチェック
    for (int i = 0; i < 8; i++) {
      int currentState = mcp[0x20 - MCP_BASE_ADDR].digitalRead(i);
      if (currentState != lastState[i]) {
        handlePinChange(i, currentState);
        lastState[i] = currentState;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100));  // ポーリング間隔
  }
}


int value[8];
// ピンの変化を処理するハンドラ
void handlePinChange(int pin, int state) {
  Serial.println("handlePinChange Start");
  if(pin == 7)
  {
      for (int i = 0; i < 8; i++) {
        mcp[0x21 - MCP_BASE_ADDR].digitalWrite(i+8, mcp[0x22 - MCP_BASE_ADDR].digitalRead(i));
      }
  }
}
