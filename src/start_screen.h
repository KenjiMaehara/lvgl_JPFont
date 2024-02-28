#ifndef START_SCREEN_H
#define START_SCREEN_H

#include <lvgl.h>

extern void load_screen(lv_obj_t* screen);
extern void delete_current_lvgl_screen(void);

extern lv_obj_t* current_lvgl_screen;

#endif