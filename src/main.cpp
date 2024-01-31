#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP23X17.h>

// 5つのMCP23017デバイス用の配列
Adafruit_MCP23X17 mcp[5];

void blinkTask(void *pvParameters);


void setup() {

  Wire.begin();
  Serial.begin(115200); // シリアル通信の初期化

  Wire.begin();

  // 5つのデバイスを初期化
  for (int addr = 0; addr < 5; addr++) {
    mcp[addr].begin_I2C(0x20 + addr);
  }

  // アドレス0x20と0x22のデバイスのGPA0～7を入力として設定
  for (int pin = 0; pin < 8; pin++) {
    mcp[0].pinMode(pin, INPUT);  // 0x20のGPA0～7
    mcp[2].pinMode(pin, INPUT);  // 0x22のGPA0～7
  }

  // すべてのデバイスのGPB0～7と、アドレス0x21、0x23、0x24のGPA0～7を出力として設定
  // アドレス0x20のGPB3と、アドレス0x22のGPB0～3を除外
  for (int addr = 0; addr < 5; addr++) {
    for (int pin = 8; pin < 16; pin++) {
      if (!(addr == 0 && pin == 11) && !(addr == 2 && pin >= 8 && pin <= 11)) {
        mcp[addr].pinMode(pin, OUTPUT);
      }
    }
    if (addr != 0 && addr != 2) {  // 0x20と0x22を除く
      for (int pin = 0; pin < 8; pin++) {
        mcp[addr].pinMode(pin, OUTPUT);
      }
    }
  }

  // タスクの作成とスケジューリング
  xTaskCreate(
    blinkTask,   // タスク関数
    "BlinkTask", // タスク名
    10000,       // スタックサイズ
    NULL,        // タスクパラメータ
    1,           // タスク優先度
    NULL         // タスクハンドル
  );

}


void loop() {
  // 何もしない
  delay(1000);
}


void blinkTask(void *pvParameters) {
  Serial.println("blinkTask start"); // タスク開始の通知

  bool lastState[8] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH}; // 前回の状態を保持する変数
  bool currentState[8]; // 現在の状態を保持する変数

  for (;;) { // 無限ループ
    bool isLowDetected = false;
    
    // アドレス0x22hのMCP23017のGPA0～7をスキャンし、状態を保存
    for (int pin = 0; pin < 8; pin++) {
      currentState[pin] = mcp[2].digitalRead(pin);
      if (currentState[pin] != lastState[pin]) {
        isLowDetected = true; // 状態が変わったらフラグを設定
      }
      lastState[pin] = currentState[pin]; // 現在の状態を保存
    }

    if (isLowDetected) {
      // 状態に変化があった場合、アドレス0x21hのMCP23017のGPB0～7に出力
      for (int pin = 0; pin < 8; pin++) {
        mcp[1].digitalWrite(8 + pin, currentState[pin]);
      }
    }

    delay(500); // 500ミリ秒待機
  }
}


