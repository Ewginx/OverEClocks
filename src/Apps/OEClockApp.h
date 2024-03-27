#pragma once
#include "BrightnessApp.h"
#include "Config/Config.h"
#include "Display/Display.h"
#include "Filesystem/lv_fs_littlefs.h"
#include "Filesystem/lv_fs_sdcard.h"
#include "GUI/GuiApp.h"
#include "MicroclimateApp.h"
#include "ServerApp.h"
#include "StateApp.h"
#include "TimeApp.h"
#include "WeatherApp.h"
#include "WiFiApp.h"
#include "SoundApp.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>
#include "RGBApp.h"
#include "ButtonApp.h"
#include "BatteryApp.h"

class OEClockApp {
  private:
    TimeApp *time_app;
    WeatherApp *weather_app;
    BrightnessApp *brightness_app;
    MicroclimateApp *microclimate_app;
    SoundApp *sound_app;
    RGBApp *rgb_app;
    ButtonApp *button_app;
    BatteryApp *battery_app;

  public:
    StateApp *state_app;
    Display *display;
    GuiApp *gui_app;
    ServerApp *server_app;
    WiFiApp *wifi_app;

    void init_i2c_apps();
    void init_gui();

    void setup();
    void loop();
    OEClockApp(/* args */);
    ~OEClockApp();
};
