#include "Apps/TimeApp.h"
#include "TimeApp.h"

static TimeApp *instance = NULL;

extern "C" void timeSyncCallbackWrapper(struct timeval *tv) {
    instance->timeSyncCallback();
}
extern "C" void updateTimezoneCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->setTimezone();
}
extern "C" void updateTimeCallbackWrapper(lv_timer_t *timer) {
    instance->notifyAboutTime();
}

extern "C" void updateTimeTimerCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->updateTimeTimer();
}

extern "C" void turnOffAlarmCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->turnOffAlarm();
}

extern "C" void snoozeAlarmCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->snoozeAlarm();
}

TimeApp::TimeApp(DigitalClock *digitalClock, AnalogClock *analogClock,
                 AlarmClock *alarmClock, StateApp *stateApp) {

    instance = this;
    this->stateApp = stateApp;
    this->alarmClock = alarmClock;
    this->analogClock = analogClock;
    this->digitalClock = digitalClock;

    sntp_set_time_sync_notification_cb(timeSyncCallbackWrapper);

    timeUpdateTimer =
        lv_timer_create(updateTimeCallbackWrapper, TIME_UPDATE_INTERVAL, NULL);
    lv_timer_pause(this->timeUpdateTimer);

    lv_msg_subscribe(MSG_UPDATE_TZ, updateTimezoneCallbackWrapper, NULL);
    lv_msg_subscribe(MSG_UPDATE_TIME_TIMER, updateTimeTimerCallbackWrapper, NULL);
    lv_msg_subscribe(MSG_ALARM_STOP, turnOffAlarmCallbackWrapper, NULL);
    lv_msg_subscribe(MSG_ALARM_SNOOZE, snoozeAlarmCallbackWrapper, NULL);
}

void TimeApp::setupTime() {
    if (this->stateApp->wifiState->wifiIsConnected) {
        configTime(0, 0, "pool.ntp.org");
        this->setTimezone();
        if (getLocalTime(&timeinfo, 10)) {
            this->stateApp->timeState->timeIsSet = true;
        } else {
            Serial.println("Can't obtain initial time");
        }
    }
}

void TimeApp::setTimezone() {
    setenv("TZ", this->stateApp->timeState->timezonePosix.c_str(), 1);
    tzset();
}

void TimeApp::notifyAboutTime() {
    getLocalTime(&timeinfo, 5);
    this->isNight();
    analogClock->setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    if (current_hour != timeinfo.tm_hour || current_minute != timeinfo.tm_min) {
        current_hour = timeinfo.tm_hour;
        current_minute = timeinfo.tm_min;
        digitalClock->setTime(timeinfo.tm_hour, timeinfo.tm_min);
    }
    digitalClock->setSeconds(timeinfo.tm_sec);
    digitalClock->setDate(timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year,
                          timeinfo.tm_wday);
    this->checkAlarmClocks(timeinfo);
}

void TimeApp::updateTimeTimer() {
    this->setupTime();
    if (this->stateApp->timeState->timeIsSet) {
        lv_timer_resume(this->timeUpdateTimer);
    } else {
        lv_timer_pause(this->timeUpdateTimer);
    }
}

void TimeApp::turnOffAlarm() {
    if (this->currentAlarm == alarmType::oneOffAlarm) {
        lv_obj_clear_state(this->alarmClock->oneOffSwitch, LV_STATE_CHECKED);
    }
    this->currentAlarm = 0;
    for (int i = 0; i < alarmType::item_count; i++) {
        needToSnoozeAlarm[i] = false;
        snoozeAlarmCount[i] = 1;
    }
    this->stopAlarm();
}
void TimeApp::timeSyncCallback() {
    if (!this->stateApp->timeState->timeIsSet) {
        this->updateTimeTimer();
    }
}
void TimeApp::snoozeAlarm() {
    needToSnoozeAlarm[this->currentAlarm] = true;
    snoozeAlarmTime[this->currentAlarm][0] = timeinfo.tm_hour;
    snoozeAlarmTime[this->currentAlarm][1] = timeinfo.tm_min;
    if (this->currentAlarm == alarmType::oneOffAlarm) {
        lv_obj_add_state(this->alarmClock->oneOffSwitch, LV_STATE_CHECKED);
    }
    this->stopAlarm();
}

bool TimeApp::isNight() {
    if ((timeinfo.tm_hour <= RGB_TIME_MORNING) ||
        (timeinfo.tm_hour >= RGB_TIME_EVENING)) {
        this->stateApp->timeState->isNight = true;
        return true;
    } else {
        this->stateApp->timeState->isNight = false;
        return false;
    }
}

void TimeApp::checkAlarmClocks(struct tm &timeinfo) {
    this->checkWeekendsAlarmClock(timeinfo);
    this->checkWeekdaysAlarmClock(timeinfo);
    this->checkOneOffAlarmClock(timeinfo);
}

void TimeApp::checkWeekendsAlarmClock(tm &timeinfo) {
    if (this->alarmClock->isAlarmDisabled(this->alarmClock->weekendsSwitch)) {
        this->resetAlarmParameters(this->alarmClock->weekendsRingsInLabel,
                                   alarmType::weekendsAlarm);
        return;
    }
    int hourFromLabel =
        this->alarmClock->getHourFromAlarmLabel(this->alarmClock->weekendsButtonLabel);
    int minuteFromLabel =
        this->alarmClock->getMinuteFromAlarmLabel(this->alarmClock->weekendsButtonLabel);
    if (this->isWeekends(timeinfo.tm_wday)) {
        if (needToSnoozeAlarm[alarmType::weekendsAlarm]) {
            this->processSnooze(alarmType::weekendsAlarm, hourFromLabel, minuteFromLabel);
        }
        if (timeinfo.tm_hour == hourFromLabel & timeinfo.tm_min == minuteFromLabel) {
            this->processFireAlarm(alarmType::weekendsAlarm);
        } else {
            alarmAlreadyFired[alarmType::weekendsAlarm] = false;
        }
    }
    this->calculateWeekendsRemainingTime(hourFromLabel, minuteFromLabel, timeinfo);
}

void TimeApp::checkWeekdaysAlarmClock(tm &timeinfo) {
    if (this->alarmClock->isAlarmDisabled(this->alarmClock->weekdaysSwitch)) {
        this->resetAlarmParameters(this->alarmClock->weekdaysRingsInLabel,
                                   alarmType::weekdaysAlarm);
        return;
    }
    int hourFromLabel =
        this->alarmClock->getHourFromAlarmLabel(this->alarmClock->weekdaysButtonLabel);
    int minuteFromLabel =
        this->alarmClock->getMinuteFromAlarmLabel(this->alarmClock->weekdaysButtonLabel);
    if (!this->isWeekends(timeinfo.tm_wday)) {
        if (needToSnoozeAlarm[alarmType::weekdaysAlarm]) {
            this->processSnooze(alarmType::weekdaysAlarm, hourFromLabel, minuteFromLabel);
        }
        if (timeinfo.tm_hour == hourFromLabel & timeinfo.tm_min == minuteFromLabel) {
            this->processFireAlarm(alarmType::weekdaysAlarm);
        } else {
            alarmAlreadyFired[alarmType::weekdaysAlarm] = false;
        }
    }
    this->calculateWeekdaysRemainingTime(hourFromLabel, minuteFromLabel, timeinfo);
}

void TimeApp::checkOneOffAlarmClock(tm &timeinfo) {
    if (this->alarmClock->isAlarmDisabled(this->alarmClock->oneOffSwitch)) {
        this->resetAlarmParameters(this->alarmClock->oneOffRingsInLabel,
                                   alarmType::oneOffAlarm);
        return;
    }
    int hourFromLabel =
        this->alarmClock->getHourFromAlarmLabel(this->alarmClock->oneOffButtonLabel);
    int minuteFromLabel =
        this->alarmClock->getMinuteFromAlarmLabel(this->alarmClock->oneOffButtonLabel);
    if (needToSnoozeAlarm[alarmType::oneOffAlarm]) {
        this->processSnooze(alarmType::oneOffAlarm, hourFromLabel, minuteFromLabel);
    }
    if (timeinfo.tm_hour == hourFromLabel & timeinfo.tm_min == minuteFromLabel) {
        this->processFireAlarm(alarmType::oneOffAlarm);
    } else {
        alarmAlreadyFired[alarmType::oneOffAlarm] = false;
    }
    this->calculateOneOffRemainingTime(hourFromLabel, minuteFromLabel, timeinfo);
}

void TimeApp::resetAlarmParameters(lv_obj_t *label, int alarmType) {
    needToSnoozeAlarm[alarmType] = false;
    alarmAlreadyFired[alarmType] = false;
    snoozeAlarmCount[alarmType] = 1;
    snoozeAlarmTime[alarmType][0] = 0;
    snoozeAlarmTime[alarmType][1] = 0;
    lv_label_set_text(label, "");
}

void TimeApp::calculateOneOffRemainingTime(int hour, int minute, struct tm &timeinfo) {
    struct tm timeinfoLocal;
    this->copyTimeinfoStruct(timeinfoLocal, timeinfo);
    time_t now = mktime(&timeinfoLocal);
    if ((timeinfoLocal.tm_hour >= hour & timeinfoLocal.tm_min >= minute) ||
        timeinfoLocal.tm_hour > hour) {
        timeinfoLocal.tm_mday += 1;
    }
    timeinfoLocal.tm_hour = hour;
    timeinfoLocal.tm_min = minute;
    time_t nextTime = mktime(&timeinfoLocal);
    double difference = difftime(nextTime, now);
    this->setRingsInLabelText(difference, alarmClock->oneOffRingsInLabel);
}

void TimeApp::calculateWeekendsRemainingTime(int hour, int minute, struct tm &timeinfo) {
    struct tm timeinfoLocal;
    this->copyTimeinfoStruct(timeinfoLocal, timeinfo);
    time_t now = mktime(&timeinfo);
    int weekdays_add[5] = {5, 4, 3, 2, 1};
    if (0 < timeinfoLocal.tm_wday & timeinfoLocal.tm_wday < 6) {
        timeinfoLocal.tm_mday += weekdays_add[timeinfoLocal.tm_wday - 1];
    }
    if (timeinfoLocal.tm_wday == 0) {
        if ((timeinfoLocal.tm_hour >= hour & timeinfoLocal.tm_min >= minute) ||
            timeinfoLocal.tm_hour > hour) {
            timeinfoLocal.tm_mday += 6;
        }
    }
    if (timeinfoLocal.tm_wday == 6) {
        if ((timeinfoLocal.tm_hour >= hour & timeinfoLocal.tm_min >= minute) ||
            timeinfoLocal.tm_hour > hour) {
            timeinfoLocal.tm_mday += 1;
        }
    }
    timeinfoLocal.tm_hour = hour;
    timeinfoLocal.tm_min = minute;
    time_t nextTime = mktime(&timeinfoLocal);
    double difference = difftime(nextTime, now);
    this->setRingsInLabelText(difference, alarmClock->weekendsRingsInLabel);
}

void TimeApp::calculateWeekdaysRemainingTime(int hour, int minute, struct tm &timeinfo) {
    struct tm timeinfoLocal;
    this->copyTimeinfoStruct(timeinfoLocal, timeinfo);
    time_t now = mktime(&timeinfoLocal);
    if (0 < timeinfoLocal.tm_wday < 5) {
        if ((timeinfoLocal.tm_hour >= hour & timeinfoLocal.tm_min >= minute) ||
            timeinfoLocal.tm_hour > hour) {
            timeinfoLocal.tm_mday += 1;
        }
    }
    if (timeinfoLocal.tm_wday == 5) {
        if ((timeinfoLocal.tm_hour >= hour & timeinfoLocal.tm_min >= minute) ||
            timeinfoLocal.tm_hour > hour) {
            timeinfoLocal.tm_mday += 3;
        }
    }
    if (timeinfoLocal.tm_wday == 6) {
        if ((timeinfoLocal.tm_hour >= hour & timeinfoLocal.tm_min >= minute) ||
            timeinfoLocal.tm_hour > hour) {
            timeinfoLocal.tm_mday += 1;
        } else {
            timeinfoLocal.tm_mday += 2;
        }
    }
    if (timeinfoLocal.tm_wday == 0) {
        if (timeinfoLocal.tm_hour >= hour & timeinfoLocal.tm_min >= minute) {
            timeinfoLocal.tm_mday += 2;
        } else {
            timeinfoLocal.tm_mday += 1;
        }
    }
    timeinfoLocal.tm_hour = hour;
    timeinfoLocal.tm_min = minute;
    time_t nextTime = mktime(&timeinfoLocal);
    double difference = difftime(nextTime, now);
    this->setRingsInLabelText(difference, alarmClock->weekdaysRingsInLabel);
}

void TimeApp::processSnooze(int alarmType, int &hours, int &minutes) {
    hours = snoozeAlarmTime[alarmType][0];
    minutes = snoozeAlarmTime[alarmType][1];
    this->calculateSnoozeTime(hours, minutes);
    if (snoozeAlarmCount[alarmType] >= SNOOZE_RETRY) {
        needToSnoozeAlarm[alarmType] = false;
        if (alarmType == alarmType::oneOffAlarm) {
            lv_obj_clear_state(this->alarmClock->oneOffSwitch, LV_STATE_CHECKED);
        }
    }
}

void TimeApp::calculateSnoozeTime(int &hours, int &minutes) {
    minutes = minutes + SNOOZE_PERIOD_MINUTES;
    if (minutes >= 60) {
        minutes = minutes - 60;
        hours++;
    }
    if (hours >= 24) {
        hours = 1;
    }
}

void TimeApp::setRingsInLabelText(double &differenceInSeconds, lv_obj_t *ringsInLabel) {
    String time;
    time.reserve(28);
    time += alarm_translation[rings_in];
    if (differenceInSeconds > 86400) {
        time += (int)differenceInSeconds / 86400;
        time += alarm_translation[day_short];
    }
    time += (int)differenceInSeconds % 86400 / 3600;
    time += alarm_translation[hour_short];
    time += (int)differenceInSeconds % 86400 % 3600 / 60;
    time += alarm_translation[minute_short];
    lv_label_set_text(ringsInLabel, time.c_str());
}

bool TimeApp::isWeekends(int weekDay) {
    return (weekDay == 0 || weekDay == 6) ? true : false;
}
void TimeApp::copyTimeinfoStruct(tm &newTm, tm &oldTm) {
    newTm.tm_hour = oldTm.tm_hour;
    newTm.tm_min = oldTm.tm_min;
    newTm.tm_mday = oldTm.tm_mday;
    newTm.tm_sec = oldTm.tm_sec;
    newTm.tm_mon = oldTm.tm_mon;
    newTm.tm_year = oldTm.tm_year;
    newTm.tm_wday = oldTm.tm_wday;
    newTm.tm_yday = oldTm.tm_yday;
    newTm.tm_isdst = oldTm.tm_isdst;
}

void TimeApp::fireAlarm(int hour, int minute) {
    alarmClock->showAlarm(hour, minute);
    this->stateApp->alarmState->alarmIsRinging = true;
    lv_msg_send(MSG_ALARM_PLAY, NULL);
}
void TimeApp::processFireAlarm(int alarmType) {
    if (!alarmAlreadyFired[alarmType]) {
        this->fireAlarm(timeinfo.tm_hour, timeinfo.tm_min);
        alarmAlreadyFired[alarmType] = true;
        this->currentAlarm = alarmType;
        if (needToSnoozeAlarm[alarmType]) {
            snoozeAlarmCount[alarmType]++;
        }
        if (alarmType == alarmType::oneOffAlarm) {
            lv_obj_clear_state(this->alarmClock->oneOffSwitch, LV_STATE_CHECKED);
        }
    }
}
void TimeApp::stopAlarm() {
    this->alarmClock->deleteAlarmModalPanel();
    lv_msg_send(MSG_SOUND_STOP, NULL);
    this->stateApp->alarmState->alarmIsRinging = false;
}

TimeApp::~TimeApp() {}