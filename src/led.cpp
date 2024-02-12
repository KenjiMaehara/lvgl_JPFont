#include <Wire.h>
//#include "Adafruit_MCP23017.h"
#include <Adafruit_MCP23X17.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "common.h"

TwoWire I2Ctwo = TwoWire(1);

//SemaphoreHandle_t ledSemaphore; // セマフォを定義
SemaphoreHandle_t i2cSemaphore = xSemaphoreCreateMutex();


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
    byte error, address;
    int nDevices;


    while (true) {
        if (xSemaphoreTake(i2cSemaphore, portMAX_DELAY) == pdTRUE) {
          
          Serial.println("Scanning...");
          nDevices = 0;
          for(address = 1; address < 127; address++ ) {
            I2Ctwo.beginTransmission(address);
            error = I2Ctwo.endTransmission();

            if (error == 0) {
              Serial.print("I2C device found at address 0x");
              if (address < 16) Serial.print("0");
              Serial.print(address, HEX);
              Serial.println("  !");

              nDevices++;
            }
            else if (error == 4) {
              Serial.print("Unknown error at address 0x");
              if (address < 16) Serial.print("0");
              Serial.println(address, HEX);
            }    
          }
          vTaskDelay(pdMS_TO_TICKS(2000)); // 2秒待つ
          xSemaphoreGive(i2cSemaphore);
        }

    }
}




void led_setup() {
  I2Ctwo.begin(26, 25); // ESP32のIO26(SDA)とIO25(SCL)を指定


  // セマフォの作成
  //ledSemaphore = xSemaphoreCreateBinary();
  
  #ifdef ENABLE_LED_TASK
  // LED点滅タスクを作成
  xTaskCreate(blinkLedTask, "Blink LED Task", 2048 * 4, NULL, 1, NULL);
  #endif
}

