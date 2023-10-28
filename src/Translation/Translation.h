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
extern const char *settings_translation[];
// extern std::map<String, String> settings_translation;