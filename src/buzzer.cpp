#include <Arduino.h>
#include <lvgl.h>
#include "common.h"
#include <Wire.h>
#include <freertos/semphr.h>

// MCP23017のI2Cアドレスとレジスタ
#define MCP23017_ADDRESS 0x20
#define IODIRB 0x01
#define GPIOB  0x13

void writeRegister(int device, byte address, byte value);

void buzzer_setup() {
    //Wire.begin();
    //writeRegister(MCP23017_ADDRESS, IODIRB, 0x00); // MCP23017のポートBを出力に設定
    // ... 他のセットアップコード ...
}


// MCP23017のレジスタに値を書き込む関数
void writeRegister(int device, byte address, byte value) {
    #if 0
    if (xSemaphoreTake(i2cSemaphore, portMAX_DELAY)) {
        Wire.beginTransmission(device);
        Wire.write(address);
        Wire.write(value);
        Wire.endTransmission();
        xSemaphoreGive(i2cSemaphore);   // セマフォを解放
    }
    #endif
}

void soundBuzzer() {
    //writeRegister(MCP23017_ADDRESS, GPIOB, 0x08); // ブザーをオンにする
    mcp[0x20 - MCP_BASE_ADDR].digitalWrite(11, HIGH);
    delay(100); // 0.1秒待つ
    mcp[0x20 - MCP_BASE_ADDR].digitalWrite(11, LOW);
    writeRegister(MCP23017_ADDRESS, GPIOB, 0x00); // ブザーをオフにする
}

void task_soundBuzzer(void * parameter) {
    if (xSemaphoreTake(i2cSemaphore, portMAX_DELAY) {
        soundBuzzer();
        xSemaphoreGive(i2cSemaphore);   // セマフォを解放
    }
}


void buzzer_beep() {
    xTaskCreate(
        task_soundBuzzer,   // タスクで実行される関数
        "task_soundBuzzer", // タスク名
        5000,           // スタックサイズ
        NULL,            // パラメータ
        1,               // 優先順位
        NULL             // タスクハンドル
    );
}