#include <Arduino.h>


#define DEVICE_NAME "OverEClock"
#define WIFI_SCAN_ITER 5
#define WIFI_CONNECT_TIMEOUT 10 * 1000

#define TIME_UPDATE_INTERVAL 60 * 1000

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

#define BAT_MAX_VOLT 4.2
#define BAT_PIN 34
#define BAT_CONV_FACTOR 1.725
#define BAT_READS 24



#define HOUR_COUNT 24
#define MINUTE_COUNT 60
#define HOUR_LABEL 0
#define MINUTE_LABEL 3

#define SCREEN_CHANGE_ANIM_TIME 200


//C lang legacy
#define SETTINGS_PANEL_HEIGHT 640