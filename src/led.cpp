#include <Wire.h>
//#include "Adafruit_MCP23017.h"
#include <Adafruit_MCP23X17.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "common.h"



SemaphoreHandle_t ledSemaphore; // セマフォを定義


#define EMG_INPUT_CH1 0
#define EMG_INPUT_CH2 1
#define EMG_INPUT_CH3 2
#define EMG_INPUT_CH4 3
#define EMG_INPUT_CH5 4
#define EMG_INPUT_CH6 5
#define EMG_INPUT_CH7 6
#define EMG_INPUT_CH8 7


LedState gLedState;

// LED点滅タスク
void blinkLedTask(void *parameter) {
    Serial.println("--------------blinkLedTask Start--------------");

  //ledState.areaLeds[0] = false;

    while (true) {
        if (xSemaphoreTake(ledSemaphore, portMAX_DELAY)) {
          // MCP23017デバイスのインスタンスを動的に作成
          Adafruit_MCP23X17* mcp_0x21 = new Adafruit_MCP23X17();

          mcp_0x21->begin_I2C(0x21, &Wire);
          for (int i = 0; i < 16; i++) {
            mcp_0x21->pinMode(i, OUTPUT);
          }
          // セマフォを取得できたらAREA_LEDを状態に応じて点灯/消灯
          for (int i = 0; i < 8; i++) {
              mcp_0x21->digitalWrite(i, gLedState.areaLeds[i]);
          }

          // 入力に応じてEMG_LEDを点灯/消灯
          for (int i = 0; i < 8; i++) {
            mcp_0x21->digitalWrite(i+8, gLedState.emgLeds[i]);
          }

          // MCP23017デバイスのインスタンスを解放
          delete mcp_0x21;
        }

    }
}


void I2C_scan();

void led_setup() {
  Wire.begin(26, 25); // ESP32のIO26(SDA)とIO25(SCL)を指定

  I2C_scan();

  // セマフォの作成
  ledSemaphore = xSemaphoreCreateBinary();
  
  #ifdef ENABLE_LED_TASK
  // LED点滅タスクを作成
  xTaskCreate(blinkLedTask, "Blink LED Task", 2048, NULL, 1, NULL);
  #endif
}



void I2C_scan() {
  Wire.begin(26, 25); // SDAを26ピン、SCLを25ピンとしてI2C通信を開始
  Serial.begin(115200); // シリアル通信を開始
  while (!Serial); // シリアルポートが開くまで待機
  Serial.println("\nI2Cデバイススキャン開始...");

  byte error, address;
  int nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    // I2Cデバイスの存在をチェック
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2Cデバイスが見つかりました。アドレス: 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address, HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("不明なエラーがアドレス 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address, HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("デバイスが見つかりませんでした。");
  else
    Serial.println("デバイススキャン完了。");

}
