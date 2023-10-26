#pragma once
// #include <Arduino.h>


#define DEVICE_NAME "OEClock"
#define WIFI_SCAN_ITER 5
#define WIFI_CONNECT_TIMEOUT MILLISECONDS * 10


#define GMT_OFFSET 10 // hours (need to check cases when its negative)
#define TIME_UPDATE_INTERVAL 400
#define WEATHER_API_POLLING_INTERVAL_MINUTES 3 //pass the interval according to your weather api plan


#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

#define BAT_MAX_VOLT 4.2
#define BAT_PIN 34
#define BAT_CONV_FACTOR 1.725
#define BAT_READS 24


#define NAMESPACE "OEClock"

#define HOUR_COUNT 24
#define MINUTE_COUNT 60
#define HOUR_LABEL 0
#define MINUTE_LABEL 3

#define SCREEN_CHANGE_ANIM_TIME 200

#define MILLISECONDS 1000

#define WEATHER_API_POLLING_INTERVAL_MILLISECONDS MILLISECONDS * WEATHER_API_POLLING_INTERVAL_MINUTES * 60

#define SECONDS_IN_ONE_HOUR 3600

//C lang legacy
#define SETTINGS_PANEL_HEIGHT 640