#pragma once
// #include <Arduino.h>


#define DEVICE_NAME "OEClock"


/*hours (need to check cases when its negative)*/
#define GMT_OFFSET 10
/*pass the interval according to your weather api plan*/
#define WEATHER_API_POLLING_INTERVAL_MINUTES 20
#define API_KEY "api_key" // WeatherAPI  api key
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


#define MSG_WEATHER_ENABLED 1


#define WIFI_CONNECTED_SYMBOL "\uE63E"
#define WIFI_DISCONNECTED_SYMBOL "\uE648"
#define PRESSURE_SYMBOL "\uf079"
#define HUMIDITY_SYMBOL "\uf07a"
#define NO_DATA_SYMBOL "\uf07b"
#define MAX_TEMP_SYMBOL "\ue903"
#define LOW_TEMP_SYMBOL "\ue902"
#define TEMP_UP_SYMBOL "\ue901"
#define TEMP_DOWN_SYMBOL "\ue900"
#define WATER_DROP_SYMBOL "\uf078"
#define SNOWFLAKE_SYMBOL "\uf076"
#define WIND_SYMBOL "\uf0cc"
#define UV_SYMBOL "\uf072"
#define W_WIND_SYMBOL "\uf048"
#define E_WIND_SYMBOL "\uf04d"
#define N_WIND_SYMBOL "\uf058"
#define S_WIND_SYMBOL "\uf044"
#define SW_WIND_SYMBOL "\uf043"
#define SE_WIND_SYMBOL "\uf088"
#define NE_WIND_SYMBOL "\uf057"
#define NW_WIND_SYMBOL "\uf087"
