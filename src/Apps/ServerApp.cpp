#include "ServerApp.h"

ServerApp::ServerApp(StateApp *state_app) : server(port) {
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    //           { request->send(200, "text/plain", "Hi! Welcome to OEClock."); });
}
void ServerApp::setup() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });
    server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });
    server.on("/weather", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });
    server.on("/brightness", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });
    server.on("/theme", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });
    server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });
    server.on("/debug", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    server.on("/weather", HTTP_POST, [](AsyncWebServerRequest *request){
        String message;
        if (request->hasParam("weather", true)) {
            // message = request->getParam("api_key", true)->value();
            message = request->getParam("city", true)->value();
            message = request->getParam("language", true)->value();
        } else {
            message = "No message sent";
        }
        request->send(200);
    });
    server.serveStatic("/", SPIFFS, "/");
    server.begin();
    ElegantOTA.begin(&server);
}
void ServerApp::run() { ElegantOTA.loop(); }
ServerApp::~ServerApp() {}