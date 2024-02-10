#include <Wire.h>
#include <SC16IS7X0.h>

TwoWire I2Cone = TwoWire(0);

// SC16IS740のI2Cアドレスを定義
#define SC16IS740_ADDRESS 0x49

// SC16IS740を使用するためのオブジェクトを作成
SC16IS7X0 device(SC16IS740_ADDRESS);

// 送信タスクのプロトタイプ宣言
void sendTask(void *parameter);

void device_setup() {
  // シリアル通信を開始（デバッグ用）
  Serial.begin(115200);
  while (!Serial); // シリアルポートが開くのを待つ

  // I2C通信を開始
  I2Cone.begin(21,22);

  #if 0
  // SC16IS740デバイスの初期化
  if (!device.begin_UART(9600)) {
    Serial.println("Device not found");
    while (1); // デバイスが見つからない場合はここで停止
  }
  #endif

  // SC16IS740デバイスの初期化。戻り値のチェックは不要な場合。
  device.begin_UART(9600);
  Serial.println("SC16IS740 ready");


  // UART設定: 9600 baud, 8N1
  device.updateBaudRate(9600); // 仮のメソッド名、正しいメソッド名に置き換える必要があります。


  Serial.println("SC16IS740 ready");

  // "hello world"送信タスクを作成
  xTaskCreate(
    sendTask,          // タスク関数
    "Send Hello Task", // タスク名
    2048 * 4,              // スタックサイズ
    NULL,              // タスクパラメータ
    1,                 // タスク優先度
    NULL               // タスクハンドル
  );
}


// "hello world"を送信するタスク
void sendTask(void *parameter) {
  for (;;) { // 無限ループ
    //device.write("hello world\n");
    vTaskDelay(pdMS_TO_TICKS(2000)); // 2秒待つ
  }
}