#pragma once
#include <Arduino.h>
#include "Config/Config.h"
// #include<map>
enum settings_texts {
    THEME,
    AUTO,
    CITY_PLACEHOLDER,
    CITY,
    WIFI_SSID_PLACEHOLDER,
    WIFI_SSID,
    WIFI_PASSWORD_PLACEHOLDER,
    WIFI_PASSWORD,
    ACCESS_POINT_IP,

};
enum alarm_texts {
    WORKING_DAY_LABEL,
    WEEKEND_DAY_LABEL,
    ONEOFF_DAY_LABEL,
    OK_BUTTON,
    CANCEL_BUTTON,
    RINGS_IN,
    DAY_SHORT,
    HOUR_SHORT,
    MINUTE_SHORT,
};
enum weather_texts {
    FEELS_LIKE,
};
extern const char *settings_translation[];
extern const char *digital_clock_translation[];
extern const char *alarm_translation[];
extern const char *weather_translation[];
// extern std::map<String, String> settings_translation;