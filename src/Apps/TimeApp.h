#pragma once
#include "Config/Config.h"
#include "GUI/AlarmClock.h"
#include "GUI/AnalogClock.h"
#include "GUI/DigitalClock.h"
#include "StateApp.h"
#include <Arduino.h>
#include <time.h>

class TimeApp {
  public:
    void notifyAboutTime();
    void setTimezone();
    void snooze();
    void updateTimeTimer();
    void turnOff();

    TimeApp(DigitalClock *digitalClock, AnalogClock *analogClock, AlarmClock *alarmClock,
            StateApp *stateApp);
    ~TimeApp();

  private:
    StateApp *stateApp;

    DigitalClock *digitalClock;
    AnalogClock *analogClock;
    AlarmClock *alarmClock;

    lv_timer_t *timeUpdateTimer = NULL;

    bool weekdaysAlarmAlreadyFired = false;
    bool weekendsAlarmAlreadyFired = false;
    bool oneOffAlarmAlreadyFired = false;
    short int currentAlarm = 0;
    short int snoozePeriodInMinutes = 10;
    short int snoozeMaxCount = 6;

    bool snoozeWeekendsAlarm = false;
    bool snoozeWeekdaysAlarm = false;
    bool snoozeOneOffAlarm = false;

    int snoozeWeekdaysTime[2];
    int snoozeWeekendsTime[2];
    int snoozeOneOffTime[2];

    short int snoozeWeekendsCount = 1;
    short int snoozeWeekdaysCount = 1;
    short int snoozeOneOffCount = 1;

    struct tm timeinfo;

    const char *ntpServer = "pool.ntp.org";

    void setupTime();

    void fire(int hour, int minute);
    void stop();

    void checkAlarmClocks(tm &timeinfo);

    static void copyTimeinfoStruct(struct tm &newTm, struct tm &oldTm);

    static bool isWeekends(int weekDay);
    bool isNight();

    void checkWeekendsAlarmClock(tm &timeinfo);
    void checkWeekdaysAlarmClock(tm &timeinfo);
    void checkOneOffAlarmClock(tm &timeinfo);

    void calculateOneOffRemainingTime(int hour, int minute, struct tm &timeinfo);
    void calculateWeekendsRemainingTime(int hour, int minute, struct tm &timeinfo);
    void calculateWeekdaysRemainingTime(int hour, int minute, struct tm &timeinfo);

    void calculateSnoozeTime(int &hours, int &minutes);

    void setRingsInLabelText(double &differenceInSeconds, lv_obj_t *ringsInLabel);
};
