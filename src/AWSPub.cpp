#include "AWSPub.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "FS.h"
#include "SPIFFS.h"
#include "SPIFFSRead.h"
#include "common.h"
//#include <M5Core2.h>

#define BUZZER_PIN 25 // スピーカーが接続されているGPIOピン番号
#define CHANNEL 0

// AWS IoT設定
//const char* aws_endpoint = "YOUR_AWS_IOT_ENDPOINT"; // AWS IoTエンドポイント
//const char* deviceName = "YOUR_DEVICE_NAME";
String gAwsEndpoint = "";
String gDeviceName = "";
const int aws_port = 8883;

String sanitizeEndpoint(String endpoint);


// AWS IoT設定（エンドポイント、認証情報など）
// 以前の設定をここに移動

WiFiClientSecure net;
PubSubClient client(net);


void callback(char* topic, byte* message, unsigned int length);
void subscDataToAWS(void * parameter);
void reconnect();


void readAWSDeviceName() {
    if(!SPIFFS.begin(true)){
        Serial.println("SPIFFSの初期化に失敗しました。");
        return;
    }

    // AWS設定ファイルを開く
    File configFile = SPIFFS.open("/aws_devicename.conf", "r");
    if (!configFile) {
        Serial.println("AWS設定ファイルを開くことができませんでした。");
        return;
    }

    // ファイルからデバイス名とエンドポイントを読み込む
    //String awsDeviceName, awsEndpoint;
    while(configFile.available()) {
        String line = configFile.readStringUntil('\n');
        line.trim();
        int separatorIndex = line.indexOf(':');
        if (separatorIndex != -1) {
            String key = line.substring(0, separatorIndex);
            String value = line.substring(separatorIndex + 1);

            if (key == "AWS_DEVICE_NAME") {
                gDeviceName = value;
            } else if (key == "AWS_IOT_ENDPOINT") {
                gAwsEndpoint = value;
            }
        }
    }
    configFile.close();

    // 変数に読み込んだ値を設定
    //const char* aws_endpoint = awsEndpoint.c_str();
    //const char* deviceName = awsDeviceName.c_str();
    //awsEndpoint = sanitizeEndpoint(awsEndpoint);

    //aws_endpoint = awsEndpoint.c_str();
    //deviceName = awsDeviceName.c_str();
    Serial.println("AWS IoTエンドポイント: " + gAwsEndpoint);
    Serial.println("デバイス名: " + gDeviceName);
}


String gCaContent = "";
String gCertificateContent = "";
String gPrivateKeyContent = "";


void setupAWSIoT() {

  ledcSetup(CHANNEL, 2000, 8); // チャンネル0、2kHzの周波数、8ビットの解像度で設定
  ledcAttachPin(BUZZER_PIN, CHANNEL); // BUZZER_PINをチャンネル0にアタッチ

   if(!SPIFFS.begin(true)){
    Serial.println("SPIFFSの初期化に失敗しました。");
    return;
  }

  readAWSDeviceName();

   if(!SPIFFS.begin(true)){
    Serial.println("SPIFFSの初期化に失敗しました。");
    return;
  }


  // Amazon Root CA 1証明書を読み込む
  File ca = SPIFFS.open("/key/AmazonRootCA1.pem", "r");
  if (!ca) {
      Serial.println("CA証明書ファイルの読み込みに失敗しました。");
  } else {
      Serial.println("CA証明書ファイルを読み込みました。");
      //String caContent = "";
      while (ca.available()) {
          gCaContent += char(ca.read());
      }
      Serial.print("CA証明書の内容の長さ: ");
      Serial.println(gCaContent.length()); // 読み込んだCA証明書の内容の長さを表示

      net.setCACert(gCaContent.c_str());
      ca.close();
  }

   if(!SPIFFS.begin(true)){
    Serial.println("SPIFFSの初期化に失敗しました。");
    return;
  }


  // 拡張子に基づいて証明書とキーを読み込む
  gCertificateContent = SPIFFSRead::readFirstFileWithExtension(".cert.pem");
  gPrivateKeyContent = SPIFFSRead::readFirstFileWithExtension(".private.key");

  // 読み込んだ内容でセットアップ
  if(gCertificateContent.length() > 0 && gPrivateKeyContent.length() > 0) {
    net.setCertificate(gCertificateContent.c_str());
    net.setPrivateKey(gPrivateKeyContent.c_str());

    // 内容の長さを出力する
    Serial.print("証明書の長さ: ");
    Serial.println(gCertificateContent.length());
    Serial.print("プライベートキーの長さ: ");
    Serial.println(gPrivateKeyContent.length());
  } else {
    Serial.println("証明書またはプライベートキーが見つかりませんでした。");
  }

  //client.setClient(net);
  // AWS IoTエンドポイントの設定
  client.setServer(gAwsEndpoint.c_str(), aws_port);

  //client.setKeepAlive(60);


  // コールバック関数をセット
  client.setCallback(callback);
  // サブスクライブするトピックを指定
  client.subscribe("button/topic/+");


  // タスクの作成と開始
  xTaskCreatePinnedToCore(
    subscDataToAWS, /* タスク関数 */
    "subscDataToAWSTask", /* タスク名 */
    1024 * 8,         /* スタックサイズ */
    NULL,          /* タスクパラメータ */
    1,             /* 優先度 */
    NULL,          /* タスクハンドル */
    0              /* コアID */
  );
}

// AWSへデータを送信するタスク
void subscDataToAWS(void * parameter){

  for(;;){ // 無限ループ
    if (!client.connected() && WiFi.status() == WL_CONNECTED) {
      reconnect();
    }

    if(client.connected()){
      client.loop();
    }

    vTaskDelay(100 / portTICK_PERIOD_MS); // 100ミリ秒の遅延
  }
}


// サーバーへの接続が切れた場合の再接続ロジックに、サブスクライブ処理を追加
void reconnect() {
  // ループして接続を試みる
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // クライアントIDとしてデバイス名を使用して接続試行
    if (client.connect(gDeviceName.c_str())) {
      Serial.println("connected");

      // コールバック関数をセット
      //client.setCallback(callback);
      // ここでトピックを再サブスクライブ
      client.subscribe("button/topic/+");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // 5秒待機して再試行
      delay(5000);
    }
  }
}


// MQTTメッセージを受信したときのコールバック関数
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  Serial.println(messageTemp);
  
  for(int i = 0; i < 5; i++) {
    buzzer_beep();
    delay(100);
    
  }
  

  #if 0
  // ここで受信したメッセージに基づいて何かアクションを行う
  // 受信したメッセージに基づいてM5Core2の画面をランダムに色を変えて5秒間点滅させる
  for (int i = 0; i < 5; i++) { // 5回繰り返し
      uint32_t randomColor = random(0xFFFFFF); // ランダムな色を生成
      M5.Lcd.fillScreen(randomColor); // 画面をランダムな色で塗りつぶす
      delay(1000); // 1秒待つ
  }
  M5.Lcd.fillScreen(BLACK); // 最後に画面を黒でクリア
  #endif

}