#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "common.h"




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


uint8_t value;
// ピンの変化を処理するハンドラ
void handlePinChange(int pin, int state) {
  Serial.println("handlePinChange Start");
  //if(pin == 7)
  {
      value = mcp_0x22.readGPIOA();
      // GPB0～7にvalueの値を出力
      mcp_0x21.writeGPIOB(value);

  }
  Serial.println("handlePinChange End");
}
