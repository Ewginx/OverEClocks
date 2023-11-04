#pragma once
// #include <Arduino.h>


#define DEVICE_NAME "OEClock"


/*hours (need to check cases when its negative)*/
#define GMT_OFFSET 10
/*pass the interval according to your weather api plan*/
#define WEATHER_API_POLLING_INTERVAL_MINUTES 20

/* 
"0" - english
"1" - русский
*/
#define LANGUAGE 1

#define NAMESPACE "OEClock"

#define BAT_MAX_VOLT 4.2
#define BAT_PIN 34
#define BAT_CONV_FACTOR 1.725
#define BAT_READS 24


#define MILLISECONDS 1000

#define WIFI_SCAN_ITER 5
#define WIFI_CONNECT_TIMEOUT (MILLISECONDS * 10)

#define SCREEN_CHANGE_ANIM_TIME 200
#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320



#define WEATHER_API_POLLING_INTERVAL_MILLISECONDS (MILLISECONDS * WEATHER_API_POLLING_INTERVAL_MINUTES * 60)

#define SECONDS_IN_ONE_HOUR 3600

#define HOUR_COUNT 24
#define MINUTE_COUNT 60
#define HOUR_LABEL 0
#define MINUTE_LABEL 3

#define TIME_UPDATE_INTERVAL 400


#define WIFI_CONNECTED_SYMBOL "\uE63E"
#define WIFI_DISCONNECTED_SYMBOL "\uE648"