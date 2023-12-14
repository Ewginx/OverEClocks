#pragma once
#include "BrightnessApp.h"
#include "Config/Config.h"
#include "Display/Display.h"
#include "Filesystem.h"
#include "GUI/GuiApp.h"
#include "MicroclimateApp.h"
#include "ServerApp.h"
#include "TimeApp.h"
#include "WeatherApp.h"
#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>

class OEClockApp {
  private:
    // unsigned long time_now = 0;
    String ssid;
    String password;

    TimeApp *time_app;
    WeatherApp *weather_app;
    BrightnessApp *brightness_app;
    MicroclimateApp *microclimate_app;
    bool _wifi_connected = false;

  public:
    Preferences preferences;
    Display *display;
    GuiApp *gui_app;
    ServerApp *server_app;

    void init_app();
    void connect_to_wifi();
    void handle_wifi_state(bool wifi_connected);

    void setup();
    void loop();
    OEClockApp(/* args */);
    ~OEClockApp();
};
