#pragma once
#include <Arduino.h>

class Logger {
public:
    static void init();
    static void log(String msg);
    static void log_credential(String user, String pass);
};
