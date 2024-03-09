#ifndef CLOCK_SCREEN_H
#define CLOCK_SCREEN_H

extern void create_clock_screen(void);
extern void screen_switch_event_handler(lv_event_t* e);
extern lv_obj_t* gScreen;

#endif