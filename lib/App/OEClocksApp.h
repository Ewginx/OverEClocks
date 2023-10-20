#pragma once
#include "GuiApp.h"
#include <ArduinoHttpClient.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <Preferences.h>
#include "time.h"
#include "Display.h"
#include "GuiApp.h"
#include "Settings.h"
#include "Config.h"

class OEClocksApp
{
private:
    unsigned long time_now = 0;
    const char *ssid = "ssid";
    const char *password = "password";
    short int port = 80;
    const long gmtOffset_sec = 36000;
    const int daylightOffset_sec = 0;
    struct tm timeinfo;
    char timeHour[3];
    char timeMinute[3];
    char timeSecond[3];
    char fullTime[8];
    char fullDate[20];
    int16_t angle;
    int hour;
    int minute;
    int second;
    const char *ntpServer = "pool.ntp.org";
    String weather_url = "jsonplaceholder.typicode.com";
public:

    Settings *settings;
    WiFiClient wifi;
    HttpClient client = HttpClient(wifi, weather_url, port);
    Preferences preferences;
    AsyncWebServer server;
    Display *display;
    GuiApp *gui_app;

    void setup();

    void loop();
    void settings_button_event_cb(lv_event_t *e);
    void darkmode_switch_event_cb(lv_event_t *e);
    OEClocksApp(/* args */);
    ~OEClocksApp();
};


