#include <Arduino.h>
#include <SPI.h>
#include <LittleFS.h>
#include <SD.h>
#include "pins.h"
#include "display.h"
#include "logger.h"
#include "captive_portal.h"

enum AppState {
    STATE_MENU,
    STATE_TEMPLATE_SELECT,
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

PortalTemplate g_selected_template = TEMPLATE_GENERIC;
String g_custom_templates[10];
int g_custom_count = 0;
String g_all_template_names[14];
int g_total_templates = 0;

void scan_sd_templates() {
    g_custom_count = 0;
    if (!SD.begin(SD_CS)) {
        Serial.println("SD Mount Failed");
        return;
    }
    
    if (!SD.exists("/portals")) {
        SD.mkdir("/portals");
    }
    
    File root = SD.open("/portals");
    if (!root || !root.isDirectory()) return;
    
    File file = root.openNextFile();
    while (file && g_custom_count < 10) {
        if (!file.isDirectory()) {
            String name = file.name();
            if (name.endsWith(".html") || name.endsWith(".htm")) {
                g_custom_templates[g_custom_count++] = "/portals/" + name;
            }
        }
        file = root.openNextFile();
    }
    root.close();
}

void setup() {
    Serial.begin(115200);
    
    // Initialize Display
    display_init();
    
    // Initialize Logger
    Logger::init();
    
    // Scan SD for custom portals
    scan_sd_templates();
    
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
                    g_state = STATE_TEMPLATE_SELECT;
                    
                    // Prepare combined list
                    g_total_templates = 0;
                    for (int i = 0; i < 4; i++) {
                        g_all_template_names[g_total_templates++] = TEMPLATE_NAMES[i];
                    }
                    for (int i = 0; i < g_custom_count; i++) {
                        g_all_template_names[g_total_templates++] = "SD: " + g_custom_templates[i].substring(9);
                    }
                    
                    // Convert to const char* array for display
                    const char* names[14];
                    for (int i = 0; i < g_total_templates; i++) {
                        names[i] = g_all_template_names[i].c_str();
                    }
                    
                    display_show_menu(names, g_total_templates, 0);
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

void handle_template_select() {
    TouchPoint p = display_get_touch();
    static bool last_pressed = false;
    
    if (p.pressed && !last_pressed) {
        if (p.y > 30 && p.y < 240) {
            int clicked_item = (p.y - 30) / 25;
            if (clicked_item >= 0 && clicked_item < g_total_templates) {
                if (clicked_item < 4) {
                    // Built-in template
                    g_selected_template = (PortalTemplate)clicked_item;
                    g_state = STATE_CAPTIVE_PORTAL;
                    display_show_status("PORTAL", "Starting AP...", "SSID: Free WiFi");
                    portal.begin("Free WiFi", g_selected_template);
                } else {
                    // SD Card template
                    int sd_idx = clicked_item - 4;
                    g_state = STATE_CAPTIVE_PORTAL;
                    display_show_status("PORTAL", "Starting AP...", "SSID: Free WiFi");
                    portal.begin("Free WiFi", TEMPLATE_GENERIC, g_custom_templates[sd_idx]);
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
        case STATE_TEMPLATE_SELECT:
            handle_template_select();
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
