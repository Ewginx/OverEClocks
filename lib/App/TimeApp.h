#pragma once
#include <Arduino.h>
#include "time.h"
#include "Alarm.h"
#include "DigitalClock.h"
#include "AnalogClock.h"
#include "Config.h"

class TimeApp
{
private:
    DigitalClock *digital_clock;
    AnalogClock *analog_clock;
    Alarm *alarm;
    
    struct tm timeinfo;

    const long gmtOffset_sec = SECONDS_IN_ONE_HOUR * GMT_OFFSET;
    const int daylightOffset_sec = 0;
    const char *ntpServer = "pool.ntp.org";

    char timeSecond[3];
    char fullTime[8];
    char fullDate[25];
    
    unsigned long time_now = 0;

public:
    void notifyAboutTime();
    void config_time();
    TimeApp(DigitalClock *digital_clock, AnalogClock *analog_clock, Alarm *alarm);
    ~TimeApp();
};


