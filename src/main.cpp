#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP23X17.h>

// 5つのMCP23017デバイス用の配列
Adafruit_MCP23X17 mcp[5];

void setup() {
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
  for (int addr = 0; addr < 5; addr++) {
    for (int pin = 8; pin < 16; pin++) {
      if (!(addr == 0 && pin == 11)) { // アドレス0x20のGPB3を除外
        mcp[addr].pinMode(pin, OUTPUT);
      }
    }
    if (addr != 0 && addr != 2) {  // 0x20と0x22を除く
      for (int pin = 0; pin < 8; pin++) {
        mcp[addr].pinMode(pin, OUTPUT);
      }
    }
  }
}

void loop() {
  // すべてのデバイスのすべての出力ピンをHIGHに設定（LED ON）
  for (int addr = 0; addr < 5; addr++) {
    for (int pin = 8; pin < 16; pin++) {
      if (!(addr == 0 && pin == 11)) { // アドレス0x20のGPB3を除外
        mcp[addr].digitalWrite(pin, HIGH);
      }
    }
    if (addr != 0 && addr != 2) {  // 0x20と0x22を除く
      for (int pin = 0; pin < 8; pin++) {
        mcp[addr].digitalWrite(pin, HIGH);
      }
    }
  }
  delay(500); // 500ミリ秒待機

  // すべてのデバイスのすべての出力ピンをLOWに設定（LED OFF）
  for (int addr = 0; addr < 5; addr++) {
    for (int pin = 8; pin < 16; pin++) {
      if (!(addr == 0 && pin == 11)) { // アドレス0x20のGPB3を除外
        mcp[addr].digitalWrite(pin, LOW);
      }
    }
    if (addr != 0 && addr != 2) {  // 0x20と0x22を除く
      for (int pin = 0; pin < 8; pin++) {
        mcp[addr].digitalWrite(pin, LOW);
      }
    }
  }
  delay(500); // 500ミリ秒待機
}
