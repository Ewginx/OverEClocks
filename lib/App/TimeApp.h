#pragma once
#include "time.h"
#include "Alarm.h"
#include "DigitalClock.h"
#include "AnalogClock.h"

class TimeApp
{
private:
    struct tm timeinfo;
    DigitalClock *digital_clock;
    AnalogClock *analog_clock;
    Alarm *alarm;
public:
    void notifyAboutTime();
    TimeApp(/* args */);
    ~TimeApp();
};


