#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP23X17.h>

Adafruit_MCP23X17 mcp_addr0x20;
Adafruit_MCP23X17 mcp_addr0x21;
Adafruit_MCP23X17 mcp_addr0x22;

void setup() {

  Wire.begin(); 

  // MCP23017を初期化（アドレス0x21）
  mcp_addr0x21.begin_I2C(0x21);  

  // GPA0～7とGPB0～7を出力として設定
  for (int i = 0; i < 16; i++) {
    mcp_addr0x21.pinMode(i, OUTPUT);
  }
}



void loop() {
  // 全てのピンをHIGHに設定（LED ON）
  for (int i = 0; i < 16; i++) {
    mcp_addr0x21.digitalWrite(i, HIGH);
  }
  delay(500); // 500ミリ秒待機

  // 全てのピンをLOWに設定（LED OFF）
  for (int i = 0; i < 16; i++) {
    mcp_addr0x21.digitalWrite(i, LOW);
  }
  delay(500); // 500ミリ秒待機
}
