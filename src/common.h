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


extern void create_keypad_screen(lv_obj_t *scr);
extern void create_clock_screen(lv_obj_t *scr);
extern void create_security_screen(lv_obj_t *scr);
extern void create_area_screen(lv_obj_t *scr);
extern void create_wifi_screen(lv_obj_t *scr);
extern void task_connectToWiFi(void * parameter);


extern void add_navigation_buttons(lv_obj_t *scr, lv_obj_t *next_screen, lv_obj_t *prev_screen);


extern lv_obj_t *screen1;
extern lv_obj_t *screen2;
extern lv_obj_t *screen3;
extern lv_obj_t *screen4;
extern lv_obj_t *screen5;




// WiFiの資格情報
extern const char* ssid;
extern const char* password;


extern void buzzer_beep();
extern void buzzer_setup();
extern void led_setup();

extern bool ledOn;
//extern SemaphoreHandle_t ledSemaphore; // セマフォを定義

extern Adafruit_MCP23X17 mcp[5];
#define MCP_BASE_ADDR 0x20
extern void sensor_input_setup();

extern uint8_t lastState;  // 以前の状態を格納する配列
extern void pinMonitorTask(void *pvParameters);
extern void handlePinChange(int pin, int state);

#endif