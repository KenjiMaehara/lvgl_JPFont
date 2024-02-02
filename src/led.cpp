#include <Wire.h>
//#include "Adafruit_MCP23017.h"
#include <Adafruit_MCP23X17.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "common.h"

Adafruit_MCP23X17 mcp_0x20;
Adafruit_MCP23X17 mcp_0x21;
Adafruit_MCP23X17 mcp_0x22;
Adafruit_MCP23X17 mcp_0x23;
Adafruit_MCP23X17 mcp_0x24;


//Adafruit_MCP23X17 mcp_local_input;
//Adafruit_MCP23X17 mcp_emg_LED;

SemaphoreHandle_t ledSemaphore; // セマフォを定義

// led.cpp
bool ledOn = false; // グローバル変数を定義



#define EMG_INPUT_CH1 0
#define EMG_INPUT_CH2 1
#define EMG_INPUT_CH3 2
#define EMG_INPUT_CH4 3
#define EMG_INPUT_CH5 4
#define EMG_INPUT_CH6 5
#define EMG_INPUT_CH7 6
#define EMG_INPUT_CH8 7

uint8_t lastState;  // 以前の状態を格納する配列

// LED点滅タスク
void blinkLedTask(void *parameter) {
    Serial.println("blinkLedTask Start");
    vTaskDelay(pdMS_TO_TICKS(5000));  // 5秒待つ

    unsigned long previousMillis = 0; // 前回のデバッグメッセージを出力した時間
    const long interval = 5000; // デバッグメッセージの間隔（ミリ秒）

    while (true) {

      //出力PIN設定
      if (ledOn) {
          // 全てのLEDを点灯
          for (int i = 0; i < 8; i++) {
              mcp_0x21.digitalWrite(i, HIGH);
          }
      } else {
          // 全てのLEDを消灯
          for (int i = 0; i < 8; i++) {
              mcp_0x21.digitalWrite(i, LOW);
          }
      }


      // 入力PIN設定     
      uint8_t currentState = mcp_0x20.readGPIOA();
      Serial.print("Current State: 0b"); Serial.println(currentState, BIN); // 現在の状態をバイナリ形式で出力
      delay(30); //チャタリング防止
      // 各ビットの状態をチェックし、変更があった場合は処理する
      for (int i = 0; i < 8; i++) {
        bool currentBit = (currentState >> i ) & 1; // i番目のビットの状態を取得
        if (currentBit != ((lastState >> i ) & 1)) {
          Serial.print("Pin Change Detected on Pin: "); Serial.print(i);
          Serial.print(" to "); Serial.println(currentBit);
          handlePinChange(i, currentBit);
          //lastState = (lastState & ~(1 << i)) | (currentBit << i); // lastStateを更新
        }
      }
      Serial.print("Last State before update: 0b"); Serial.println(lastState, BIN); // 更新前のlastStateをバイナリ形式で出力
      lastState = currentState;
      Serial.print("Last State after update: 0b"); Serial.println(lastState, BIN); // 更新後のlastStateをバイナリ形式で出力

      

      // 現在の時間を取得
      unsigned long currentMillis = millis();
      // 前回のデバッグメッセージ出力から10秒経過したか確認
      if (currentMillis - previousMillis >= interval) {
        // 最後のメッセージ出力時間を更新
        previousMillis = currentMillis;
        // デバッグメッセージを出力
        Serial.println("blinkLedTask is alive");
      }

      vTaskDelay(pdMS_TO_TICKS(100));
    }
}




void led_setup() {

 
  Wire.begin(); // ESP32のIO26(SDA)とIO25(SCL)を指定
  Wire.setClock(50000);

  #if 0
  mcp_0x20.begin_I2C(0x20, &Wire); // MCP23017のI2Cアドレスを指定（必要に応じて変更）

  // 入力に設定
  for (int i = 0; i < 8; i++) {
    mcp_0x20.pinMode(i, INPUT);
  }
  

  // 出力に設定
  for (int i = 8; i < 16; i++) {
    mcp_0x20.pinMode(i, OUTPUT);
  }



  mcp_0x21.begin_I2C(0x21, &Wire); // MCP23017のI2Cアドレスを指定（必要に応じて変更）

  // LEDを出力に設定
  for (int i = 0; i < 16; i++) {
    mcp_0x21.pinMode(i, OUTPUT);
  }
  #endif


  mcp_0x22.begin_I2C(0x22, &Wire);  // MCP23017のI2Cアドレスを0x22として初期化

  // GPA0からGPA7までを入力として設定
  for (int i = 0; i < 16; i++) {
    mcp_0x22.pinMode(i, INPUT);
  }

#if 0

  // GPAポートの割り込みを有効にする
  mcp_0x22.setupInterrupts(true, false, LOW);  // ミラーリング無し、オープンドレイン無し、アクティブロー

  // GPA0～GPA7の割り込みを有効にする
  for (int i = 0; i < 8; i++) {
      mcp_0x22.setupInterruptPin(i, CHANGE);  // 状態変化で割り込みを発生
  }

  // GPA0からGPA7を入力として設定し、プルアップ抵抗を有効化
  //for (int i = 0; i < 8; i++) 
  {

    //lastState[i] = mcp[0x20 - MCP_BASE_ADDR].digitalRead(i);
    //lastState = mcp[0x20 - MCP_BASE_ADDR].readGPIOA();
    lastState = mcp_0x20.readGPIOA();
  }


  mcp_0x23.begin_I2C(0x23, &Wire);  // MCP23017のI2Cアドレスを0x23として初期化
  mcp_0x24.begin_I2C(0x24, &Wire);  // MCP23017のI2Cアドレスを0x23として初期化

  // セマフォの作成
  //ledSemaphore = xSemaphoreCreateBinary();
  
  

  // LED点滅タスクを作成
  xTaskCreate(blinkLedTask, "Blink LED Task", 8192, NULL, 1, NULL);

  #endif
}

