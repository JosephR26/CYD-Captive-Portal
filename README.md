# CYD-Captive-Portal

Cyberpunk-themed Captive Portal Pentesting Firmware for the ESP32-2432S028 (Cheap Yellow Display).

## Features

*   **Cyberpunk UI**: High-contrast neon aesthetic (cyan, magenta, yellow) with glitch-style headers.
*   **Captive Portal**: Fake Wi-Fi Access Point with DNS hijacking.
*   **Credential Harvesting**: Captures and logs usernames/passwords to LittleFS.
*   **Touch Interface**: Fully controlled via the 2.8" touchscreen (XPT2046).
*   **Live Logs**: View captured data directly on the device screen.

## Hardware

Designed for the **ESP32-2432S028** "Cheap Yellow Display" board.

## Installation

1.  Install [PlatformIO](https://platformio.org/).
2.  Clone this repo: `git clone https://github.com/JosephR26/CYD-Captive-Portal.git`
3.  Build & Upload: `pio run --target upload`
4.  Upload Filesystem: `pio run --target uploadfs`

## Usage

1.  Power on the CYD.
2.  Select **START PORTAL** from the menu.
3.  Victims connecting to "Free WiFi" will be redirected to the login page.
4.  View captured credentials in **VIEW LOGS**.

## License

MIT
