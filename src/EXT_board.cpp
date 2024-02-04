#include <Arduino.h>
#include <Wire.h>
#include <D:\mywork\lvgl_JPFont\.pio\libdeps\esp32dev\SC16IS750\SC16IS750.h> // SC16IS750ライブラリをインクルード
//#include <UnoWiFiDevEd.h>

#define SC16IS750_ADDRESS 0x48 // デバイスのI2Cアドレス
#define SCL_PIN 22
#define SDA_PIN 21

SC16IS750 device = SC16IS750(SC16IS750_ADDRESS, SDA_PIN, SCL_PIN);

// "hello world"を送信するタスク
void EXT_boardTask(void *parameter) {
  for (;;) { // 無限ループ
    device.println("hello world");
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1秒待機
  }
}

void EXT_board_setup() {
  Serial.begin(9600);
  
  Wire.begin(SDA_PIN, SCL_PIN);
  
  if (!device.begin(9600)) {
    Serial.println("SC16IS750 not found. Please check wiring.");
    //while (1);
  }
  
  Serial.println("SC16IS750 initialized successfully.");

  // 新しいタスクを作成
  xTaskCreate(
    sendHelloWorldTask, // タスク関数
    "Send Hello World Task", // タスク名
    10000, // スタックサイズ
    NULL, // タスク入力パラメータ
    1, // タスクの優先度
    NULL // タスクハンドル
  );
}
