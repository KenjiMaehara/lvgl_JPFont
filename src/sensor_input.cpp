#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "common.h"




void pinMonitorTask(void *pvParameters);
void handlePinChange(int pin, int state);

int last_pinValue[8];  // 以前の状態を格納する配列

void sensor_input_setup() {
  // MCP23017デバイスのインスタンスを動的に作成
  Adafruit_MCP23X17* mcp_0x22 = new Adafruit_MCP23X17();
  
  mcp_0x22->begin_I2C(0x22, &Wire);
  // GPA0からGPA7を入力として設定し、プルアップ抵抗を有効化
  for (int i = 0; i < 8; i++) {

    //lastState[i] = mcp[0x20 - MCP_BASE_ADDR].digitalRead(i);
    last_pinValue[i] = mcp_0x22->digitalRead(i);
  }
  delete mcp_0x22;

  // ピン監視タスクの作成
  xTaskCreate(pinMonitorTask, "Pin Monitor", 10000, NULL, 1, NULL);
}



// ピンの状態を監視するタスク
void pinMonitorTask(void *pvParameters) {

  Serial.println("pinMonitorTask Start");
  bool pin_change = false;

  for (;;) {
    // MCP23017デバイスのインスタンスを動的に作成
    Adafruit_MCP23X17* mcp_0x22 = new Adafruit_MCP23X17();
    
    if (mcp_0x22->begin_I2C(0x22, &Wire)) { // アドレス0x22のMCP23017を初期化
      //Serial.println("MCP23017 connection successful");

      for (int i = 0; i < 8; i++) {
        mcp_0x22->pinMode(i, INPUT);

        int pinValue = mcp_0x22->digitalRead(i);
        if (pinValue != last_pinValue[i]) {
          pin_change = true;
          last_pinValue[i] = pinValue;
        }
      }

      if (pin_change == true) {
        Serial.println("pin_change true!!!!!");
        for (int i = 0; i < 8; i++) {
            gLedState.emgLeds[i] = last_pinValue[i];
        }
        xSemaphoreGive(ledSemaphore); // セマフォを解放
        pin_change=false;


      } 
      // MCP23017デバイスのインスタンスを解放
      delete mcp_0x22;
    } else {
      Serial.println("MCP23017 connection failed");
    }
    vTaskDelay(pdMS_TO_TICKS(100));  // ポーリング間隔
  }
}



// ピンの変化を処理するハンドラ
void handlePinChange(int pin, int state) {
  Serial.println("handlePinChange Start");
  //if(pin == 7)
  {
      for (int i = 0; i < 8; i++) {
        //value[i] = mcp_0x22->digitalRead(i);
      }
      //mcp_0x21.digitalWrite(i+8, mcp_0x22.digitalRead(i));
  }
}
