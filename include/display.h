#pragma once
#include <stdint.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

void display_init();
void display_update();
void display_show_menu(const char* const* items, int count, int selected);
void display_show_status(const char* module_name, const char* line1, const char* line2);
void display_show_error(const char* msg);
void display_clear();
void display_show_logs();

// Touch coordinates
struct TouchPoint {
    int16_t x;
    int16_t y;
    bool pressed;
};

TouchPoint display_get_touch();
