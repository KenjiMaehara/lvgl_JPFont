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

  // 割り込みの設定
  mcp[2].setupInterrupts(true, false, LOW); // ミラーリング無効, デフォルト値をLOWに設定

  // GPA0～7の各ピンに対して割り込みを有効化
  for (int pin = 0; pin < 8; pin++) {
    mcp[2].setupInterruptPin(pin, CHANGE); // ピンの状態が変化した場合に割り込みが発生
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

  bool lastInterruptState = HIGH; // 前回の割り込み状態を保持する変数
  bool currentState[8]; // 現在の0x22hの入力状態を保持する変数

  for (;;) { // 無限ループ
    bool isLowDetected = false;
    bool interruptState = mcp[0].digitalRead(7); // アドレス0x20hのGPA7の状態を読み取る

    // 割り込み状態のデバッグ出力
    Serial.print("Interrupt State: ");
    Serial.println(interruptState);

    // 割り込みがLOWの場合のみチェック
    if (interruptState == LOW) {
      delay(100); // チャタリング防止のため10ミリ秒待機
      // アドレス0x22hのMCP23017のGPA0～7をスキャンし、状態を保存
      for (int pin = 0; pin < 8; pin++) {
        currentState[pin] = mcp[2].digitalRead(pin);
        // 入力状態のデバッグ出力
        Serial.print("Input Pin ");
        Serial.print(pin);
        Serial.print(": ");
        Serial.println(currentState[pin]);
      }
      isLowDetected = true; // 割り込みがあったのでフラグを設定
    }

    if (isLowDetected) {
      // 状態に変化があった場合、アドレス0x21hのMCP23017のGPB0～7に出力
      for (int pin = 0; pin < 8; pin++) {
        mcp[1].digitalWrite(8 + pin, currentState[pin]);
        // 出力状態のデバッグ出力
        Serial.print("Output Pin ");
        Serial.print(pin);
        Serial.print(": ");
        Serial.println(currentState[pin]);
      }
    }

    delay(100); // 100ミリ秒待機
  }
}



