#pragma once
#include "BrightnessApp.h"
#include "Config/Config.h"
#include "Display/Display.h"
#include "Filesystem/lv_fs_sdcard.h"
#include "GUI/GuiApp.h"
#include "MicroclimateApp.h"
#include "ServerApp.h"
#include "StateApp.h"
#include "TimeApp.h"
#include "WeatherApp.h"
#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include <LittleFS.h>

class OEClockApp {
  private:
    TimeApp *time_app;
    WeatherApp *weather_app;
    BrightnessApp *brightness_app;
    MicroclimateApp *microclimate_app;

  public:
    StateApp *state_app;
    Display *display;
    GuiApp *gui_app;
    ServerApp *server_app;

    void init_i2c_apps();
    void init_gui();
    void connect_to_wifi();
    void handle_wifi_state(bool wifi_connected);

    void setup();
    void loop();
    OEClockApp(/* args */);
    ~OEClockApp();
};
