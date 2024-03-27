#pragma once
// #include <Arduino.h>
#include "MessageCodes.h"
#include "Symbols.h"
#include "Pins.h"

#define DEVICE_NAME "OEClock"


// Posix Timezone String https://www.veron.nl/wp-content/uploads/2022/12/Posix-Timezone-Strings.pdf
#define TIMEZONE "GMT"

/* 
"0" - english
"1" - русский
*/
#define LANGUAGE 1

 // Weather settings
#define API_KEY ""
#define WEATHER_CITY ""
/*
Arabic	ar
Bengali	bn
Bulgarian	bg
Chinese Simplified	zh
Chinese Traditional	zh_tw
Czech	cs
Danish	da
Dutch	nl
English en
Finnish	fi
French	fr
German	de
Greek	el
Hindi	hi
Hungarian	hu
Italian	it
Japanese	ja
Javanese	jv
Korean	ko
Mandarin	zh_cmn
Marathi	mr
Polish	pl
Portuguese	pt
Punjabi	pa
Romanian	ro
Russian	ru
Serbian	sr
Sinhalese	si
Slovak	sk
Spanish	es
Swedish	sv
Tamil	ta
Telugu	te
Turkish	tr
Ukrainian	uk
Urdu	ur
Vietnamese	vi
Wu (Shanghainese)	zh_wuu
Xiang	zh_hsn
Yue (Cantonese)	zh_yue
Zulu	zu
*/
#define WEATHER_LANGUAGE "en"

// WiFi credentials
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define ACCESS_POINT_LOGIN "OEClock"
#define ACCESS_POINT_PASSWORD "admin"

#define IP_ADDRESS ""
#define GATEWAY_ADDRESS ""

// Set false if you want to set Analog Clock as the main screen
#define DIGITAL_CLOCK_MAIN_SCREEN true

// Pass the interval according to your weather api plan
#define WEATHER_API_POLLING_INTERVAL_MINUTES 20


// Time check period, ms
#define TIME_UPDATE_INTERVAL 900


//   BAT+ ---+
//           |
//          R1
//           |
//           +------- SENSE
//           |
//          R2
//           |
//   BAT- ----
// (R1 + R2) / R2  I am using R1=R2=1M Ohms
// for battery charge detection R1=1M Ohms and R2=1M Ohms
#define DIVIDER_RATIO 1.94
#define BAT_MAX_VOLT 4.2
#define BAT_MIN_VOLT 2.6


// Preferences namespace
#define NAMESPACE "OEClock"

#define MILLISECONDS 1000

#define WIFI_SCAN_ITER 5
#define WIFI_CONNECT_TIMEOUT (MILLISECONDS * 10)

#define SCREEN_CHANGE_ANIM_TIME 200
#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320


//period in which active screen will be set back to DigitalClockScreen if no user activity is presented
#define SCREEN_SWAP_PERIOD MILLISECONDS * 30 
#define DOCK_PANEL_HIDE_PERIOD MILLISECONDS * 15

#define CHANGE_THEME_THRESHOLD 26

#define WEATHER_API_POLLING_INTERVAL_MILLISECONDS (MILLISECONDS * WEATHER_API_POLLING_INTERVAL_MINUTES * 60)

#define SECONDS_IN_ONE_HOUR 3600

#define HOUR_COUNT 24
#define MINUTE_COUNT 60
#define HOUR_LABEL 0
#define MINUTE_LABEL 3

#define NUMPIXELS 8




