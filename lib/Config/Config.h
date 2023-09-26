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
