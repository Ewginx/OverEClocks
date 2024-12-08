#pragma once
#include "Config/Config.h"
#include "GUI/AlarmClock.h"
#include "GUI/AnalogClock.h"
#include "GUI/DigitalClock.h"
#include "StateApp.h"
#include "esp_sntp.h"
#include <Arduino.h>
#include <time.h>


class TimeApp {
  public:
    void notifyAboutTime();
    void setTimezone();
    void snoozeAlarm();
    void updateTimeTimer();
    void turnOffAlarm();

    void timeSyncCallback();

    TimeApp(DigitalClock *digitalClock, AnalogClock *analogClock, AlarmClock *alarmClock,
            StateApp *stateApp);
    ~TimeApp();

  private:
    StateApp *stateApp;
    DigitalClock *digitalClock;
    AnalogClock *analogClock;
    AlarmClock *alarmClock;

    lv_timer_t *timeUpdateTimer = NULL;

    struct tm timeinfo;

    const char *ntpServer = "pool.ntp.org";

    enum alarmType {
        weekdaysAlarm = 0,
        weekendsAlarm,
        oneOffAlarm,
        item_count,
    };

    short int currentAlarm = 0;
    bool alarmAlreadyFired[3] = {false, false, false};

    bool needToSnoozeAlarm[3] = {false, false, false};

    int snoozeAlarmTime[3][2];

    short int snoozeAlarmCount[3] = {1, 1, 1};

    void setupTime();

    void fireAlarm(int hour, int minute);
    void stopAlarm();
    void processFireAlarm(int alarmType);
    void checkAlarmClocks(tm &timeinfo);
    void checkWeekendsAlarmClock(tm &timeinfo);
    void checkWeekdaysAlarmClock(tm &timeinfo);
    void checkOneOffAlarmClock(tm &timeinfo);
    void resetAlarmParameters(lv_obj_t *label, int alarmType);
    void calculateOneOffRemainingTime(int hour, int minute, struct tm &timeinfo);
    void calculateWeekendsRemainingTime(int hour, int minute, struct tm &timeinfo);
    void calculateWeekdaysRemainingTime(int hour, int minute, struct tm &timeinfo);

    static bool isWeekends(int weekDay);
    bool isNight();

    void processSnooze(int alarmType, int &hours, int &minutes);
    void calculateSnoozeTime(int &hours, int &minutes);

    static void copyTimeinfoStruct(struct tm &newTm, struct tm &oldTm);

    void setRingsInLabelText(double &differenceInSeconds, lv_obj_t *ringsInLabel);
};
