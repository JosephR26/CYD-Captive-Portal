#include "display.h"
#include "pins.h"
#include <SPI.h>
#include <FS.h>
#include <LittleFS.h>

TFT_eSPI tft = TFT_eSPI();
XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ);

// Cyberpunk Colors
#define CP_BLACK   0x0000
#define CP_CYAN    0x07FF
#define CP_MAGENTA 0xF81F
#define CP_YELLOW  0xFFE0
#define CP_RED     0xF800
#define CP_BLUE    0x001F

void draw_glitch_header(const char* title, uint16_t color) {
    tft.fillRect(0, 0, 320, 30, CP_BLACK);
    tft.drawFastHLine(0, 28, 320, color);
    tft.drawFastHLine(0, 29, 320, color);
    
    tft.setTextColor(color);
    tft.setTextSize(2);
    tft.setCursor(10, 5);
    tft.print("> ");
    tft.print(title);
    
    // Glitch effect lines
    tft.drawFastHLine(200, 10, 50, CP_MAGENTA);
    tft.drawFastHLine(210, 15, 30, CP_CYAN);
}

void display_init() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(CP_BLACK);
    
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    SPI.begin(TOUCH_SCLK, TOUCH_MISO, TOUCH_MOSI, TOUCH_CS);
    ts.begin();
    ts.setRotation(1);

    // Boot Screen
    tft.fillScreen(CP_BLACK);
    tft.setTextColor(CP_CYAN);
    tft.setTextSize(3);
    tft.setCursor(40, 80);
    tft.println("MIDAS_OS v2.0");
    tft.setTextColor(CP_MAGENTA);
    tft.setTextSize(1);
    tft.setCursor(40, 110);
    tft.println("INITIALIZING NEURAL LINK...");
    delay(1000);
}

void display_show_menu(const char* const* items, int count, int selected) {
    tft.fillScreen(CP_BLACK);
    draw_glitch_header("SYSTEM_MENU", CP_CYAN);
    
    for (int i = 0; i < count; i++) {
        int y = 45 + i * 35;
        if (i == selected) {
            tft.fillRect(5, y - 5, 310, 30, CP_CYAN);
            tft.setTextColor(CP_BLACK);
        } else {
            tft.drawRect(5, y - 5, 310, 30, CP_CYAN);
            tft.setTextColor(CP_CYAN);
        }
        tft.setCursor(20, y + 2);
        tft.setTextSize(2);
        tft.printf("[%02d] %s", i, items[i]);
    }
}

void display_show_status(const char* module_name, const char* line1, const char* line2) {
    tft.fillScreen(CP_BLACK);
    draw_glitch_header(module_name, CP_MAGENTA);
    
    tft.setTextColor(CP_YELLOW);
    tft.setTextSize(2);
    tft.setCursor(10, 60);
    if (line1) tft.println(line1);
    
    tft.setTextColor(CP_CYAN);
    tft.setCursor(10, 100);
    if (line2) tft.println(line2);
    
    // Decorative scanlines
    for(int i=0; i<240; i+=4) {
        tft.drawFastHLine(0, i, 320, 0x0841); // Dark gray scanlines
    }
}

void display_show_error(const char* msg) {
    tft.fillScreen(CP_BLACK);
    draw_glitch_header("CRITICAL_FAILURE", CP_RED);
    tft.setTextColor(CP_RED);
    tft.setTextSize(2);
    tft.setCursor(10, 100);
    tft.println(msg);
    
    // Red alert effect
    for(int i=0; i<3; i++) {
        tft.drawRect(0, 0, 320, 240, CP_RED);
        delay(100);
        tft.drawRect(0, 0, 320, 240, CP_BLACK);
        delay(100);
    }
}

void display_show_logs() {
    tft.fillScreen(CP_BLACK);
    draw_glitch_header("DATA_STREAM", CP_YELLOW);
    tft.setTextColor(CP_YELLOW);
    tft.setTextSize(1);
    tft.setCursor(0, 40);
    
    fs::File file = LittleFS.open("/logs.txt", "r");
    if (file) {
        if (file.size() > 1500) file.seek(file.size() - 1500);
        while (file.available()) {
            String line = file.readStringUntil('\n');
            if (line.length() > 0) {
                tft.print(">> ");
                tft.println(line);
            }
        }
        file.close();
    } else {
        tft.println("NO DATA FOUND IN BUFFER.");
    }
}

TouchPoint display_get_touch() {
    TouchPoint p = {0, 0, false};
    if (ts.touched()) {
        TS_Point tp = ts.getPoint();
        // CYD Landscape Calibration
        p.x = map(tp.x, 200, 3700, 0, 320);
        p.y = map(tp.y, 240, 3800, 0, 240);
        p.pressed = true;
    }
    return p;
}


