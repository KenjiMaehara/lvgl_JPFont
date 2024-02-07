#include <WiFi.h>
#include <WebServer.h>
#include <lvgl.h>
#include <qrcodegen.h> // QRコード生成ライブラリをインクルード（適切なライブラリに置き換えてください）

// WiFi AP設定
const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";

// Webサーバーのインスタンスを作成
WebServer server(80);

void displayQRCode(const uint8_t *qrcode);


void create_qr_wifi_screen(lv_obj_t *scr) {

  ssid = "ESP32-Access-Point";
  password = "123456789";

  // WiFi APモードの設定
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.println("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // QRコードのデータを生成（SSIDとパスワードを含む）
  const char *qrData = "WIFI:T:WPA;S:ESP32-Access-Point;P:123456789;;";
  uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
  uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
  bool ok = qrcodegen_encodeText(qrData, tempBuffer, qrcode, qrcodegen_Ecc_LOW,
                                 qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
  if (ok) {
    displayQRCode(qrcode); // QRコードをディスプレイに表示する関数（実装が必要）
  }

  // WebサーバーのルートでWiFi設定フォームを提供
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", "<h1>WiFi Settings</h1><form action=\"/set\" method=\"POST\"><input type=\"text\" name=\"ssid\"><input type=\"text\" name=\"password\"><input type=\"submit\" value=\"Connect\"></form>");
  });

  // フォームからのデータを処理
  server.on("/set", HTTP_POST, []() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");
    // WiFi接続ロジック（実装が必要）
    server.send(200, "text/html", "<h1>Settings saved. Rebooting...</h1>");
    // 設定を保存し、ESP32を再起動する処理（実装が必要）
  });

  server.begin();
}


// QRコードを表示するためのバッファサイズを定義（サイズは表示するQRコードに応じて調整）
#define QR_CODE_SIZE 150
#define CANVAS_WIDTH (QR_CODE_SIZE * 2)
#define CANVAS_HEIGHT (QR_CODE_SIZE * 2)

// LVGL canvasを表示するためのバッファ
static lv_color_t canvas_buffer[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT)];

void displayQRCode(const uint8_t *qrcode) {
  // Canvasオブジェクトを作成
  lv_obj_t *canvas = lv_canvas_create(lv_scr_act());
  lv_canvas_set_buffer(canvas, canvas_buffer, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
  lv_obj_align(canvas, LV_ALIGN_CENTER, 0, 0);
  lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);

  int size = qrcodegen_getSize(qrcode);
  int scale = QR_CODE_SIZE / size; // QRコードの各ポイントを表示するためのスケールファクター

  // QRコードの各ポイントをcanvas上に描画
  for (int y = 0; y < size; y++) {
    for (int x = 0; x < size; x++) {
      if (qrcodegen_getModule(qrcode, x, y)) {
        // 描画記述子の初期化
        lv_draw_rect_dsc_t rect_dsc;
        lv_draw_rect_dsc_init(&rect_dsc);
        rect_dsc.bg_color = lv_color_black(); // 背景色を設定
        lv_canvas_draw_rect(canvas, x * scale, y * scale, scale, scale, &rect_dsc);
      }
    }
  }

  add_navigation_buttons(screen7, screen1, screen6);
}