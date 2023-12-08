#include "ServerApp.h"

ServerApp::ServerApp(/* args */) : server(port) {

    // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    //           { request->send(200, "text/plain", "Hi! Welcome to OEClock."); });
}
void ServerApp::setup() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SD, "/index.html", "text/html");
    });
    server.serveStatic("/", SD, "/");
    server.begin();
    ElegantOTA.begin(&server);
}
void ServerApp::run() {

    ElegantOTA.loop();
}
ServerApp::~ServerApp() {}