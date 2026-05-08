#include <Arduino.h>
#include <SPI.h>
#include <LittleFS.h>
#include "pins.h"
#include "display.h"
#include "logger.h"
#include "captive_portal.h"

enum AppState {
    STATE_MENU,
    STATE_CAPTIVE_PORTAL,
    STATE_LOGS
};

AppState g_state = STATE_MENU;
CaptivePortal portal;
int g_cursor = 0;

const char* MENU_ITEMS[] = {
    "START PORTAL",
    "VIEW LOGS",
    "REBOOT"
};
const int MENU_COUNT = 3;

void setup() {
    Serial.begin(115200);
    
    // Initialize Display
    display_init();
    
    // Initialize Logger
    Logger::init();
    
    Logger::log("System Booted");
    
    display_show_menu(MENU_ITEMS, MENU_COUNT, g_cursor);
}

void handle_menu() {
    TouchPoint p = display_get_touch();
    static bool last_pressed = false;
    
    if (p.pressed && !last_pressed) {
        // Simple touch detection for menu items
        if (p.y > 30 && p.y < 240) {
            int clicked_item = (p.y - 30) / 25;
            if (clicked_item >= 0 && clicked_item < MENU_COUNT) {
                g_cursor = clicked_item;
                display_show_menu(MENU_ITEMS, MENU_COUNT, g_cursor);
                
                // Action on click
                if (g_cursor == 0) {
                    g_state = STATE_CAPTIVE_PORTAL;
                    display_show_status("PORTAL", "Starting AP...", "SSID: Free WiFi");
                    portal.begin("Free WiFi");
                } else if (g_cursor == 1) {
                    g_state = STATE_LOGS;
                    display_show_logs();
                } else if (g_cursor == 3) {
                    ESP.restart();
                }
            }
        }
    }
    last_pressed = p.pressed;
}

void loop() {
    switch (g_state) {
        case STATE_MENU:
            handle_menu();
            break;
        case STATE_CAPTIVE_PORTAL:
            {
                portal.update();
                // Check for touch to stop
                TouchPoint p = display_get_touch();
                if (p.pressed && p.y < 30) { // Touch header to exit
                    portal.stop();
                    g_state = STATE_MENU;
                    display_show_menu(MENU_ITEMS, MENU_COUNT, g_cursor);
                }
            }
            break;
        case STATE_LOGS:
            {
                TouchPoint p2 = display_get_touch();
                if (p2.pressed) {
                    g_state = STATE_MENU;
                    display_show_menu(MENU_ITEMS, MENU_COUNT, g_cursor);
                }
            }
            break;
    }
    delay(10);
}
