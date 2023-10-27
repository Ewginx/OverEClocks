#include "TimeApp.h"

TimeApp::TimeApp(DigitalClock *digital_clock, AnalogClock *analog_clock, Alarm *alarm)
{
    this->alarm = alarm;
    this->analog_clock = analog_clock;
    this->digital_clock = digital_clock;
}

void TimeApp::notifyAboutTime()
{
    if ((unsigned long)millis() - time_now > TIME_UPDATE_INTERVAL)
    {
        time_now = millis();
        getLocalTime(&timeinfo);
        analog_clock->set_time(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        strftime(fullTime, 6, "%H"
                              ":"
                              "%M",
                 &timeinfo);
        strftime(timeSecond, 3, "%S", &timeinfo);
        digital_clock->set_time(fullTime, timeSecond);
        strftime(fullDate, 25, "%d.%m.%Y, %A", &timeinfo);
        digital_clock->set_date(fullDate);
    }
}
void TimeApp::config_time(){
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    getLocalTime(&timeinfo);
}
TimeApp::~TimeApp()
{
}