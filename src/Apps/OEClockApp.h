#pragma once
#include "Apps/ServerApp.h"
#include "Apps/TimeApp.h"
#include "Apps/WeatherApp.h"
#include "Config/Config.h"
#include "Display/Display.h"
#include "GUI/GuiApp.h"

#include <Arduino.h>
#include <FS.h>
#include <Preferences.h>
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>

class OEClockApp {
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
    void handle_wifi_state(bool wifi_connected);
    void set_display_brightness(u_int32_t brightness);
    void setup();
    void loop();
    OEClockApp(/* args */);
    ~OEClockApp();
};
