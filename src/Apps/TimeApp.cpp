#include "Apps/TimeApp.h"

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
        strftime(fullDate, 12, "%d.%m.%Y,", &timeinfo);
        digital_clock->set_date(fullDate, timeinfo.tm_wday);
        alarm->check_alarm_time(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_wday);
        // timeinfo.tm_mday = timeinfo.tm_mday+7;
        // time_t time = mktime(&timeinfo);
        // struct tm *tm;
        // tm = gmtime(&time);
        // Serial.println(timeinfo.tm_mday);
    }
}
void TimeApp::config_time(){
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    getLocalTime(&timeinfo);
}
TimeApp::~TimeApp()
{
}