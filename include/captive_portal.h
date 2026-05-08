#pragma once

#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include "portal_html.h"

class CaptivePortal {
public:
    CaptivePortal();
    void begin(const char* ssid, PortalTemplate selectedTemplate = TEMPLATE_GENERIC);
    void update();
    void stop();

private:
    AsyncWebServer server;
    DNSServer dnsServer;
    const byte DNS_PORT = 53;
    bool running = false;
    PortalTemplate currentTemplate;

    void setupRoutes();
};
