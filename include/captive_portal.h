#pragma once

#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

class CaptivePortal {
public:
    CaptivePortal();
    void begin(const char* ssid);
    void update();
    void stop();

private:
    AsyncWebServer server;
    DNSServer dnsServer;
    const byte DNS_PORT = 53;
    bool running = false;

    void setupRoutes();
};
