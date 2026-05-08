#pragma once

// ─── TFT Display (ILI9341) on HSPI ──────────────────────────────────────────
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST  -1
#define TFT_BL   21

// ─── Touchscreen (XPT2046) on VSPI ──────────────────────────────────────────
#define TOUCH_MISO 39
#define TOUCH_MOSI 32
#define TOUCH_SCLK 25
#define TOUCH_CS   33
#define TOUCH_IRQ  36

// ─── MicroSD Card on VSPI ───────────────────────────────────────────────────
#define SD_MISO 19
#define SD_MOSI 23
#define SD_SCLK 18
#define SD_CS    5

// ─── RGB LED (Active LOW) ───────────────────────────────────────────────────
#define LED_RED   4
#define LED_GREEN 16
#define LED_BLUE  17

// ─── LDR (Light Sensor) ─────────────────────────────────────────────────────
#define LDR_PIN 34

// ─── Speaker ────────────────────────────────────────────────────────────────
#define SPEAKER_PIN 26

// ─── BOOT Button ────────────────────────────────────────────────────────────
#define BOOT_BTN 0
