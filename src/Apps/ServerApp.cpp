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
    server.serveStatic("/", SPIFFS, "/");
    server.begin();
    ElegantOTA.begin(&server);
}
void ServerApp::run() { ElegantOTA.loop(); }
ServerApp::~ServerApp() {}