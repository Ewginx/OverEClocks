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
    bool _wifi_connected = false;
    
public:

    Preferences preferences;
    Display *display;
    GuiApp *gui_app;
    ServerApp *server_app;
    

    void init_app();
    void connect_to_wifi();
    void set_display_brightness(u_int32_t brightness);
    void setup();
    void loop();
    OEClockApp(/* args */);
    ~OEClockApp();
};


