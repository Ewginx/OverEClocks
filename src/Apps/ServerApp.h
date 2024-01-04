#pragma once
#include "SPIFFS.h"
#include "StateApp.h"
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>


class ServerApp {
  private:
    short int port = 80;
    AsyncWebServer server;


  public:
    StateApp *_state_app;
    void get_settings(AsyncWebServerRequest *request);
    void setup();
    void run();
    ServerApp(StateApp *state_app);
    ~ServerApp();
};
