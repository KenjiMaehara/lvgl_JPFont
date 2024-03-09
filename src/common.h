#ifndef COMMON_H
#define COMMON_H

#include <TFT_eSPI.h> // ILI9488ドライバを含むライブラリ
#include <lvgl.h>
#include <Adafruit_MCP23X17.h>

// ここにextern宣言を追加
//extern TFT_eSPI tft; // 例えば TFT_eSPI オブジェクトの宣言
extern void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
extern void my_touchpad_read(lv_indev_drv_t *drv, lv_indev_data_t *data);
extern int count;
extern void tenkey_setup();
extern TFT_eSPI tft; // TFTのインスタンスを作成
extern void clock_setup();
extern void start_setup();
extern void task_setup();
extern void ntpUpdateTask(void *pvParameters);
extern void lvgl_screen_setup();

extern void screen_setup();



extern void add_navigation_buttons(lv_obj_t *scr, lv_obj_t *next_screen, lv_obj_t *prev_screen);


//extern lv_obj_t *screen1;
//extern lv_obj_t *screen2;
//extern lv_obj_t *screen3;
//extern lv_obj_t *screen4;
extern lv_obj_t *screen5;
extern lv_obj_t *screen6;
extern lv_obj_t *screen7;
extern lv_obj_t *screen8;



// WiFiの資格情報
extern const char* ssid;
extern const char* password;


extern void buzzer_beep();
extern void buzzer_setup();
extern void led_setup();

extern bool ledOn;
extern SemaphoreHandle_t ledSemaphore; // セマフォを定義

//extern Adafruit_MCP23X17 mcp[5];
extern Adafruit_MCP23X17 mcp_0x21;
extern Adafruit_MCP23X17 mcp_0x22;
#define MCP_BASE_ADDR 0x20
extern void sensor_input_setup();
//extern int value[8];

// LEDの状態を格納するグローバル構造体
struct LedState {
    bool areaLeds[8];
    bool emgLeds[8];
    // 他のLED状態も必要に応じて追加
};

extern LedState gLedState;

extern bool gWifiScan;

void readRFID_setup();

extern int gScanSuccessful;
extern volatile bool isScanningWiFi;

extern void scanAndDisplayWiFiNetworks(lv_obj_t *wifi_list_label);
extern void create_wifiScan_screen(lv_obj_t *scr);

extern bool gApModeOn;

extern void create_apmode_screen(lv_obj_t *scr);

//extern Preferences preferences;




#define ENABLE_WIFI_TASK
#define ENABLE_RFID_TASK
#define ENABLE_BUZZER_TASK
#define ENABLE_LED_TASK
#define ENABLE_SENSOR_INPUT_TASK
#define ENABLE_APMODE_TASK

#define MAX_CARD_LENGTH 38 // 最大カードデータ長を定義

/**********************
 *      TYPEDEFS
 **********************/
struct card_data_t{
    uint8_t type;
    uint8_t data[MAX_CARD_LENGTH];
    int     data_length;
} ;

extern void (*card_callback)(struct card_data_t * card);



typedef void (*card_read_cb_t)(struct card_data_t * card);
//card_read_cb_t  card_callback;




extern void create_cardTest_screen(lv_obj_t *scr);


void card_set_cb(card_read_cb_t card_cb);

extern void tryConnectToKnownNetworks();

extern void update_time(lv_timer_t *timer);


extern void switch_to_screen(int screen_index);

extern lv_obj_t* time_label_keypad;
extern lv_obj_t* time_label_security;
extern lv_obj_t* time_label_clock;
extern lv_obj_t* time_label_area;
extern lv_obj_t* time_label_ssid_IP;
extern lv_obj_t* time_label_wifiScan;
extern lv_obj_t* time_label_apmode;
extern lv_obj_t* time_label_card;
extern void setup_time_update();

extern lv_obj_t* wifi_label_clock;
extern lv_obj_t* wifi_label_tenkey;
extern void setup_wifiCheck_update();

#endif