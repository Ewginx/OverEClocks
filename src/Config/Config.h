#pragma once
// #include <Arduino.h>
#include "MessageCodes.h"
#include "Symbols.h"
#include "Pins.h"

#define DEVICE_NAME "OEClock"

//-----------------------------TIME SETTINGS-----------------------------
// Posix Timezone String https://www.veron.nl/wp-content/uploads/2022/12/Posix-Timezone-Strings.pdf
#define TIMEZONE "GMT"
// Time check period, ms
#define TIME_UPDATE_INTERVAL 900
#define HOUR_COUNT 24
#define MINUTE_COUNT 60

//-----------------------------LANGUAGE SETTINGS-----------------------------
/* 
"0" - english
"1" - русский
*/
#define LANGUAGE 1

//-----------------------------WEATHER SETTINGS-----------------------------
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

// Pass the interval according to your weather api plan
#define WEATHER_API_POLLING_INTERVAL_MINUTES 20

//-----------------------------WIFI SETTINGS-----------------------------
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define ACCESS_POINT_LOGIN "OEClock"
#define ACCESS_POINT_PASSWORD "admin"

#define IP_ADDRESS ""
#define GATEWAY_ADDRESS ""

/*access clocks web server via MDNS_NAME.local
works on MacOS and IPhone, Android 12 and higher
works on Windows 10 10.0.19042 and higher or need to install Bonjour
or just simply add OECLOCK_IP_ADDRESS YOUR_MDNS_NAME to C:\windows\system32\drivers\etc\hosts (e.g. 192.XXX.X.XX oeclock.local),
don't forget to give OEClock a static IP in router settings
need to install Ahavi on Linux
*/
#define MDNS_NAME "oeclock" 

#define WIFI_CONNECT_ATTEMPTS 20
#define WIFI_CONNECT_DELAY 500

//-----------------------------SCREEN SETTINGS-----------------------------
// Set false if you want to set Analog Clock as the main screen
#define DIGITAL_CLOCK_MAIN_SCREEN true

#define SCREEN_CHANGE_ANIM_TIME 200

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

//period in which active screen will be set back to DigitalClockScreen if no user activity is presented
#define SCREEN_SWAP_PERIOD MILLISECONDS * 30 
#define DOCK_PANEL_HIDE_PERIOD MILLISECONDS * 15

#define DARK_THEME_CHANGE_THRESHOLD 25
#define LIGHT_THEME_CHANGE_THRESHOLD 35

//-----------------------------RGB SETTINGS-----------------------------
#define RGB_TIME_MORNING 9 //Hour
#define RGB_TIME_EVENING 22 //Hour
#define NUMBER_OF_PIXELS 8

//-----------------------------BATTERY SETTINGS-----------------------------
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
#define BAT_MAX_VOLT 4.02
#define BAT_MIN_VOLT 2.6

//-----------------------------TEMPERATURE SENSOR SETTINGS-----------------------------
//offset for temperature calibration
#define TEMPERATURE_OFFSET 4


//-----------------------------MISC-----------------------------
// Preferences namespace
#define NAMESPACE "OEClock"

#define MILLISECONDS 1000

#define WEATHER_API_POLLING_INTERVAL_MILLISECONDS (MILLISECONDS * WEATHER_API_POLLING_INTERVAL_MINUTES * 60)

#define SECONDS_IN_ONE_HOUR 3600

#define HOUR_LABEL 0
#define MINUTE_LABEL 3

#define RGB_BREATHE_STEP 5




