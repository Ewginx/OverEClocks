#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

#include "GuiApp.h"
#include "Display.h"
#include "GuiApp.h"
#include "Config.h"
#include "TimeApp.h"
#include "WeatherApp.h"
#include "ServerApp.h"

class OEClockApp
{
private:
    // unsigned long time_now = 0;
    String ssid = "ssid";
    String password = "password";

    TimeApp *time_app;
    WeatherApp *weather_app;
    
public:

    Preferences preferences;
    Display *display;
    GuiApp *gui_app;
    ServerApp *server_app;

    void connect_wifi();

    void setup();
    void loop();
    OEClockApp(/* args */);
    ~OEClockApp();
};


