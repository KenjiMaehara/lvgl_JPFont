#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "common.h"


int lastState[8];  // 以前の状態を格納する配列

void pinMonitorTask(void *pvParameters);
void handlePinChange(int pin, int state);


void sensor_input_setup() {



  // ピン監視タスクの作成
  //xTaskCreate(pinMonitorTask, "Pin Monitor", 10000, NULL, 1, NULL);
}


// ピンの状態を監視するタスク
void pinMonitorTask(void *pvParameters) {

  Serial.println("pinMonitorTask Start");
  
  for (;;) {

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
        uint8_t data02 = mcp[0x22 - MCP_BASE_ADDR].digitalRead(i);
        delay(10);  //チャタリング防止
        mcp[0x21 - MCP_BASE_ADDR].digitalWrite(i+8, data02);
      }
      delay(10); 
  }
  Serial.println("handlePinChange End");
}
