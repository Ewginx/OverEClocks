#pragma once
#include <Arduino.h>
#include "GuiApp.h"
#include <ArduinoHttpClient.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <Preferences.h>
#include "Display.h"
#include "GuiApp.h"
#include "Settings.h"
#include "Config.h"
#include "TimeApp.h"

class OEClocksApp
{
private:
    // unsigned long time_now = 0;
    const char *ssid = "ssid";
    const char *password = "password";
    short int port = 80;

    String weather_url = "jsonplaceholder.typicode.com";

    TimeApp *time_app;
public:

    Settings *settings;
    WiFiClient wifi;
    HttpClient client = HttpClient(wifi, weather_url, port);
    Preferences preferences;
    AsyncWebServer server;
    Display *display;
    GuiApp *gui_app;

    void setup();
    void loop();

    void send_weather_request(void * parameter);
    
    void settings_button_event_cb(lv_event_t *e);
    void darkmode_switch_event_cb(lv_event_t *e);
    OEClocksApp(/* args */);
    ~OEClocksApp();
};


