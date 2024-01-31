#include <Arduino.h>
#include <lvgl.h>
#include "common.h"
#include <Wire.h>

// MCP23017のI2Cアドレスとレジスタ
#define MCP23017_ADDRESS 0x20
#define IODIRB 0x01
#define GPIOB  0x13

void writeRegister(int device, byte address, byte value);

void buzzer_setup() {
    Wire.begin();
    writeRegister(MCP23017_ADDRESS, IODIRB, 0x00); // MCP23017のポートBを出力に設定
    // ... 他のセットアップコード ...
}


// MCP23017のレジスタに値を書き込む関数
void writeRegister(int device, byte address, byte value) {
    Wire.beginTransmission(device);
    Wire.write(address);
    Wire.write(value);
    Wire.endTransmission();
}

void soundBuzzer() {
    writeRegister(MCP23017_ADDRESS, GPIOB, 0x08); // ブザーをオンにする
    delay(100); // 0.1秒待つ
    writeRegister(MCP23017_ADDRESS, GPIOB, 0x00); // ブザーをオフにする
}

void task_soundBuzzer(void * parameter) {
    soundBuzzer();
    vTaskDelete(NULL); // タスク終了
}


void buzzer_beep() {

    #if 0
    xTaskCreate(
        task_soundBuzzer,   // タスクで実行される関数
        "task_soundBuzzer", // タスク名
        5000,           // スタックサイズ
        NULL,            // パラメータ
        3,               // 優先順位
        NULL             // タスクハンドル
    );
    #endif
}