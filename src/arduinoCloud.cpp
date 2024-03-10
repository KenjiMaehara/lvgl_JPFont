#include <ArduinoIoTCloud.h>
//#include <WiFiNINA.h>
//#include "arduino_secrets.h"
#include <WiFi.h>
#include "arduinoCloud.h"

#if 0
/////// WiFi設定の入力 ///////
const char WIFI_SSID[] = SECRET_SSID;    // WiFiネットワーク名
const char WIFI_PASSWORD[] = SECRET_PASS; // WiFiパスワード
#endif

/////// Arduino Cloud設定 ///////
const char DEVICE_LOGIN_NAME[] = "your_device_login_name"; // デバイスのログイン名
const char DEVICE_KEY[] = "your_device_key";               // デバイスのキー

void onArduinoCloudReady() {
  Serial.println("Arduino Cloud connection established.");
}

void initArduinoCloud();
void cloudTask(void *pvParameters);

void arduinoCloud_setup() {
  //Serial.begin(9600);
  // WiFi接続の初期化
  //initWiFi();
  // Arduino Cloudの初期化
  initArduinoCloud();

  // 新規タスクの作成
  xTaskCreate(
    cloudTask,   /* タスク関数 */
    "CloudTask", /* タスク名 */
    8192,        /* スタックサイズ */
    NULL,        /* タスクパラメータ */
    1,           /* タスクの優先度 */
    NULL         /* タスクハンドル */
  );
}


void initArduinoCloud() {
  ArduinoCloud.begin(DEVICE_LOGIN_NAME, DEVICE_KEY);
  ArduinoCloud.onReady(onArduinoCloudReady);
}

void cloudTask(void *pvParameters) {
  for (;;) {
    if (ArduinoCloud.ready()) {
      // ここでArduino Cloudへのデータ送信を行う
      Serial.println("Sending data to Arduino Cloud...");
      // 例: ArduinoCloud.updateThing("sensorValue", value);
    }
    delay(10000); // 10秒ごとにループ
  }
}
