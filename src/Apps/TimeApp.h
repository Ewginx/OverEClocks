#pragma once
#include "Config/Config.h"
#include "GUI/AlarmClock.h"
#include "GUI/AnalogClock.h"
#include "GUI/DigitalClock.h"
#include <Arduino.h>
#include <time.h>

class TimeApp {

  public:
  private:
    DigitalClock *digital_clock;
    AnalogClock *analog_clock;
    AlarmClock *alarm_clock;

    struct tm timeinfo;

    const long gmtOffset_sec = SECONDS_IN_ONE_HOUR * GMT_OFFSET;
    const int daylightOffset_sec = 0;
    const char *ntpServer = "pool.ntp.org";

    char timeSecond[3];
    char fullTime[8];
    char fullDate[12];
    unsigned long time_now = 0;

  public:
    void notifyAboutTime();
    void config_time();
    TimeApp(DigitalClock *digital_clock, AnalogClock *analog_clock, AlarmClock *alarm);
    ~TimeApp();
};
