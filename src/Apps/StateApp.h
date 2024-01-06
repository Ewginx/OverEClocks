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
    String ap_login;
    String ap_password;

    String city;
    String city_encoded;
    String language;
    String api_key;
    int request_period;

    bool weather_enabled;

    bool auto_brightness;
    unsigned int brightness_level;
    bool auto_theme_change;
    bool dark_theme_enabled;

    bool digital_main_screen;

    int light_primary_color;
    int light_second_color;
    int dark_background_color;
    int dark_second_color;

    String timezone_posix;

    int threshold;

    bool weekdays_switch_enabled;
    bool weekends_switch_enabled;
    bool oneOff_switch_enabled;
    String weekdays_time;
    String weekends_time;
    String oneOff_time;

    void init_state();

    void save_alarm_switches_enabled(bool weekdays_enabled, bool weekends_enabled,
                                     bool oneOff_enabled);
    void save_alarm_time(const char *weekdays_time, const char *weekends_time,
                         const char *oneOff_time);

    void save_dark_theme_enabled(bool enabled);
    void save_auto_brightness_enabled(bool enabled);
    void save_brightness_level(unsigned int brightness_level);
    void save_brightness_threshold(int threshold);
    
    void save_ssid(const char *ssid);
    void save_password(const char *password);
    void save_city(const char *city);
    void save_ap_login(const char *login);
    void save_ap_password(const char *password);

    void save_language(const char *language);
    void save_api_key(const char *api_key);
    void save_request_period(int request_period);
    void save_weather_enabled(bool enabled);

    void save_auto_theme_change(bool change);
    void save_digital_main_screen(bool digital_main_screen);

    void save_light_colors(int light_primary_color, int light_second_color);
    void save_dark_colors(int dark_background_color, int dark_second_color);

    void save_timezone(const char *timezone_posix);

    StateApp(/* args */);
    ~StateApp();
};
