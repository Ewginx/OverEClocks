#pragma once
#include "Config/Config.h"
#include <Arduino.h>
#include <Preferences.h>

class StateApp {
  private:
    Preferences _preferences;

  public:
    bool wifi_connected = false;
    String ssid;
    String password;

    String city;
    String language;
    bool weather_enabled;

    bool auto_brightness;
    unsigned int brightness_level;

    bool dark_theme_enabled;

    bool weekdays_switch_enabled;
    bool weekends_switch_enabled;
    bool oneOff_switch_enabled;
    String weekdays_time;
    String weekends_time;
    String oneOff_time;

    void save_alarm_switches_enabled(bool weekdays_enabled, bool weekends_enabled,
                                     bool oneOff_enabled);
    void save_alarm_time(const char *weekdays_time, const char *weekends_time,
                         const char *oneOff_time);
    void save_dark_theme_enabled(bool enabled);
    void save_auto_brightness_enabled(bool enabled);
    void save_brightness_level(unsigned int brightness_level);
    void save_ssid(const char *ssid);
    void save_password(const char *password);
    void save_city(const char *city);
    void save_language(const char *language);
    void save_weather_enabled(bool enabled);

    StateApp(/* args */);
    ~StateApp();
};
