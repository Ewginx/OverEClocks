#pragma once
#include <Arduino.h>
#include "Config/Config.h"
// #include<map>
enum settings_texts {
    AUTO,
    WIFI_PASSWORD,
    WIFI_SSID
};
extern const char *settings_translation[];
// extern std::map<String, String> settings_translation;