#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

#include "GUI/GuiApp.h"
#include "Display/Display.h"
#include "Config/Config.h"
#include "Apps/TimeApp.h"
#include "Apps/WeatherApp.h"
#include "Apps/ServerApp.h"

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

    void setup_display_brightness();
    
    void setup();
    void loop();
    OEClockApp(/* args */);
    ~OEClockApp();
};


