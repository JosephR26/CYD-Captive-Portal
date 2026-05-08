#pragma once

#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include "portal_html.h"

class CaptivePortal {
public:
    CaptivePortal();
    void begin(const char* ssid, PortalTemplate selectedTemplate = TEMPLATE_GENERIC, String customPath = "");
    void update();
    void stop();

private:
    AsyncWebServer server;
    DNSServer dnsServer;
    const byte DNS_PORT = 53;
    bool running = false;
    PortalTemplate currentTemplate;
    String sdPath;

    void setupRoutes();
};
