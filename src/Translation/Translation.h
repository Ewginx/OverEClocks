#pragma once
#include <Arduino.h>
#include "Config/Config.h"
// #include<map>
enum settings_texts {
    theme,
    auto_brightness,
    city_placeholder,
    language_placeholder,
    city,
    wifi_ssid_placeholder,
    wifi_ssid,
    wifi_password_placeholder,
    wifi_password,
    access_point_ip,
    reconnect_wifi,
    update_weather,

};
enum alarm_texts {
    working_day_label,
    weekend_day_label,
    oneoff_day_label,
    ok_button,
    cancel_button,
    rings_in,
    day_short,
    hour_short,
    minute_short,
};
enum weather_texts {
    wind_speed_uom,
    feels_like,
    pressure_uom,
    night,
    morning,
    afternoon,
    evening,
    no_data,
};
extern const char *settings_translation[];
extern const char *digital_clock_translation[];
extern const char *alarm_translation[];
extern const char *weather_translation[];
// extern std::map<String, String> settings_translation;