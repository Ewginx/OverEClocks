#pragma once
#include <Arduino.h>
#include "GuiApp.h"
#include <ArduinoHttpClient.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <Preferences.h>
#include "Display.h"
#include "GuiApp.h"
#include "Config.h"
#include "TimeApp.h"
#include "WeatherApp.h"

class OEClockApp
{
private:
    // unsigned long time_now = 0;
    const char *ssid = "ssid";
    const char *password = "password";
    short int port = 80;

    String weather_url = "jsonplaceholder.typicode.com";

    TimeApp *time_app;
    WeatherApp *weather_app;
    
public:

    Preferences preferences;
    AsyncWebServer server;
    Display *display;
    GuiApp *gui_app;

    void setup();
    void loop();
    
    OEClockApp(/* args */);
    ~OEClockApp();
};


