#pragma once
#include "Config/Config.h"
#include "Display/Display.h"
#include "Filesystem.h"
#include "GUI/GuiApp.h"
#include "ServerApp.h"
#include "TimeApp.h"
#include "WeatherApp.h"
#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include <Adafruit_BME280.h>
#include <BH1750.h>

class OEClockApp {
  private:
    // unsigned long time_now = 0;
    String ssid = "ssid";
    String password = "password";

    TimeApp *time_app;
    WeatherApp *weather_app;
    bool _wifi_connected = false;
    lv_anim_t _brightness_anim;
    Adafruit_BME280 _bme_sensor;
    BH1750 _light_sensor;
    lv_timer_t * _bme_timer = NULL;
    lv_timer_t * _light_sensor_timer = NULL;

  public:
    Preferences preferences;
    Display *display;
    GuiApp *gui_app;
    ServerApp *server_app;

    void init_app();
    void connect_to_wifi();
    void bme_timer_cb(lv_timer_t *timer);
    void light_sensor_timer_cb(lv_timer_t *timer);
    void handle_wifi_state(bool wifi_connected);
    void set_display_brightness(u_int32_t brightness);
    void set_auto_brightness_timer(bool auto_brightness);
    void change_brightness_smoothly(int new_light_level, int old_light_level);
    void setup();
    void loop();
    OEClockApp(/* args */);
    ~OEClockApp();
};

