#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "common.h"


int lastState[8];  // 以前の状態を格納する配列

void pinMonitorTask(void *pvParameters);
void handlePinChange(int pin, int state);


void sensor_input_setup() {

  #if 0
  // GPA0からGPA7を入力として設定し、プルアップ抵抗を有効化
  for (int i = 0; i < 8; i++) {

    lastState[i] = mcp[0x20 - MCP_BASE_ADDR].digitalRead(i);
  }
  #endif

  // ピン監視タスクの作成
  xTaskCreate(pinMonitorTask, "Pin Monitor", 5000, NULL, 3, NULL);
}


void pinMonitorTask(void *pvParameters) {
    Serial.println("pinMonitorTask Start");

    uint8_t lastState = 0;  // 前回の状態を保存する変数

    for (;;) {
        // MCP23017のポートの状態を一括で読み込む
        uint8_t currentState = mcp[0x20 - MCP_BASE_ADDR].readGPIO(0);

        // 変化があったビットを検出
        uint8_t changedBits = currentState ^ lastState;

        // 変化があった各ビットについて処理
        for (int i = 0; i < 8; i++) {
            if (changedBits & (1 << i)) {
                // 変化があったピンについて handlePinChange を呼び出す
                handlePinChange(i, currentState & (1 << i) ? HIGH : LOW);
            }
        }

        lastState = currentState;  // 状態を更新
        vTaskDelay(pdMS_TO_TICKS(100));  // ポーリング間隔
    }
}


void handlePinChange(int pin, int state) {
    Serial.println("handlePinChange Start");
    
    if (pin == 7) {
        // アドレス0x22のMCP23017のGPA0～GPA7の状態を一括で読み取る
        uint8_t portAState = mcp[0x22 - MCP_BASE_ADDR].readGPIO(0);

        // portAStateの値をシリアルポートに出力
        Serial.print("Port A State: 0x");
        Serial.println(portAState, HEX);

        // 読み取った状態をアドレス0x21のMCP23017のGPB0～GPB7に一括で書き込む
        //mcp[0x21 - MCP_BASE_ADDR].writeGPIO(1, portAState);
    }
}
