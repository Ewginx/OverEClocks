#pragma once
#include "Config/Config.h"
#include <Arduino.h>
#include <Preferences.h>

class WiFiState {
  private:
    Preferences &_preferences;

  public:
    bool wifi_connected = false;
    String ssid;
    String password;
    String ap_login;
    String ap_password;
    String ip_address;
    String gateway_address;
    String current_ip_address;

    void save_ssid(const char *ssid);
    void save_password(const char *password);
    void save_ap_login(const char *login);
    void save_ap_password(const char *password);
    void save_ip_and_gateway_addresses(const char *ip_address,
                                       const char *gateway_address);

    WiFiState(Preferences &preferences);
    ~WiFiState();
};

class WeatherState {
  private:
    Preferences &_preferences;

  public:
    bool weather_enabled;

    String city;
    String city_encoded;
    String language;
    String api_key;
    int request_period;

    void save_city(const char *city);
    void save_language(const char *language);
    void save_api_key(const char *api_key);
    void save_request_period(int request_period);
    void save_weather_enabled(bool enabled);

    WeatherState(Preferences &preferences);
    ~WeatherState();
};

class AlarmState {
  private:
    Preferences &_preferences;

  public:
    bool weekdays_switch_enabled;
    bool weekends_switch_enabled;
    bool oneOff_switch_enabled;
    bool alarm_ringing;
    String weekdays_time;
    String weekends_time;
    String oneOff_time;

    void save_alarm_switches_enabled(bool weekdays_enabled, bool weekends_enabled,
                                     bool oneOff_enabled);
    void save_alarm_time(const char *weekdays_time, const char *weekends_time,
                         const char *oneOff_time);

    AlarmState(Preferences &preferences);
    ~AlarmState();
};

struct ThemeStruct {
    int primary_color;
    int second_color;
    int screen_color;
    int card_color;
    int text_color;
    int grey_color;
};

class ThemeState {
  private:
    Preferences &_preferences;

  public:
    bool dark_theme_enabled;

    int light_primary_color;
    int light_second_color;
    int light_screen_color;
    int light_card_color;
    int light_text_color;
    int light_grey_color;

    int dark_primary_color;
    int dark_second_color;
    int dark_screen_color;
    int dark_card_color;
    int dark_text_color;
    int dark_grey_color;

    void save_dark_theme_enabled(bool enabled);
    void save_light_colors(ThemeStruct light_theme);
    void save_dark_colors(ThemeStruct dark_theme);

    ThemeState(Preferences &preferences);
    ~ThemeState();
};

class DisplayState {
  private:
    Preferences &_preferences;

  public:
    bool auto_brightness;
    unsigned int brightness_level;
    bool auto_theme_change;
    int threshold;

    bool digital_main_screen;

    void save_auto_brightness_enabled(bool enabled);
    void save_brightness_level(unsigned int brightness_level);
    void save_brightness_threshold(int threshold);

    void save_auto_theme_change(bool change);
    void save_digital_main_screen(bool digital_main_screen);

    DisplayState(Preferences &preferences);
    ~DisplayState();
};

class TimeState {
  private:
    Preferences &_preferences;

  public:
    String timezone_posix;

    bool time_is_set = false;

    void save_timezone(const char *timezone_posix);

    TimeState(Preferences &preferences);
    ~TimeState();
};

class SoundState {
  private:
    Preferences &_preferences;

  public:
    bool sound_on;

    short int alarm_track_number;

    short int ee_track_number;

    short int volume_level;
    bool random_alarm_track;

    void save_volume_level(short int volume_level);
    void save_alarm_track_number(short int alarm_track_number);
    void save_ee_track_number(short int ee_track_number);
    void save_random_alarm_track(bool random_alarm_track);
    void save_sound_on(bool sound_on);

    SoundState(Preferences &preferences);
    ~SoundState();
};

class RGBState {
  private:
    Preferences &_preferences;

  public:
    bool enabled;
    short int effect;

    int first_rgb_color;
    int second_rgb_color;
    int third_rgb_color;

    int brightness;

    int delay;

    void save_rgb_color(int first_rgb_color, int second_rgb_color, int third_rgb_color);
    void save_rgb_enabled(bool enabled);
    void save_rgb_effect(short int effect);
    void save_delay_effect(int delay);
    void save_brightness(int brightness);

    RGBState(Preferences &preferences);
    ~RGBState();
};

class StateApp {
  private:
    Preferences _preferences;

  public:
    WiFiState *wifi_state;
    WeatherState *weather_state;
    AlarmState *alarm_state;
    ThemeState *theme_state;
    DisplayState *display_state;
    TimeState *time_state;
    SoundState *sound_state;
    RGBState *rgb_state;

    StateApp(/* args */);
    ~StateApp();
};
