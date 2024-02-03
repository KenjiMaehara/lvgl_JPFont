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

    //lastState[i] = mcp[0x20 - MCP_BASE_ADDR].digitalRead(i);
    lastState[i] = mcp_0x22.digitalRead(i);
  }

  // ピン監視タスクの作成
  xTaskCreate(pinMonitorTask, "Pin Monitor", 10000, NULL, 1, NULL);
}


// ピンの状態を監視するタスク
void pinMonitorTask(void *pvParameters) {

  Serial.println("pinMonitorTask Start");
  
  for (;;) {
    for (int i = 0; i < 8; i++) {
      int currentState = mcp_0x22.digitalRead(i);
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
  //if(pin == 7)
  {
      for (int i = 0; i < 8; i++) {
        mcp_0x21.digitalWrite(i+8, mcp_0x22.digitalRead(i));
      }
  }
}
