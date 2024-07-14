#include "Apps/TimeApp.h"
#include "TimeApp.h"

static TimeApp *instance = NULL;

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
    instance->turnOff();
}

extern "C" void snoozeAlarmCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->snooze();
}

TimeApp::TimeApp(DigitalClock *digitalClock, AnalogClock *analogClock,
                 AlarmClock *alarmClock, StateApp *stateApp) {

    instance = this;
    this->stateApp = stateApp;
    this->alarmClock = alarmClock;
    this->analogClock = analogClock;
    this->digitalClock = digitalClock;

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
        if (getLocalTime(&timeinfo)) {
            this->stateApp->timeState->timeIsSet = true;
        }
    }
}

void TimeApp::setTimezone() {
    setenv("TZ", this->stateApp->timeState->timezonePosix.c_str(), 1);
    tzset();
}

void TimeApp::notifyAboutTime() {
    getLocalTime(&timeinfo);
    this->isNight();
    analogClock->set_time(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    digitalClock->set_time(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    digitalClock->set_date(timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year,
                           timeinfo.tm_wday);
    this->checkAlarmClocks(timeinfo);
}

void TimeApp::updateTimeTimer() {
    this->setupTime();
    if (this->stateApp->wifiState->wifiIsConnected ||
        this->stateApp->timeState->timeIsSet) {
        lv_timer_resume(this->timeUpdateTimer);
    } else {
        lv_timer_pause(this->timeUpdateTimer);
    }
}

void TimeApp::turnOff() {
    if (this->currentAlarm == 3) {
        lv_obj_clear_state(this->alarmClock->oneOffSwitch, LV_STATE_CHECKED);
    }
    this->currentAlarm = 0;
    this->snoozeWeekendsAlarm = false;
    this->snoozeWeekdaysAlarm = false;
    this->snoozeOneOffAlarm = false;
    this->snoozeWeekendsCount = 1;
    this->snoozeWeekdaysCount = 1;
    this->snoozeOneOffCount = 1;
    this->stop();
}
void TimeApp::snooze() {
    if (this->currentAlarm == 1) {
        this->snoozeWeekdaysAlarm = true;
        this->snoozeWeekdaysTime[0] = timeinfo.tm_hour;
        this->snoozeWeekdaysTime[1] = timeinfo.tm_min;
    }
    if (this->currentAlarm == 2) {
        this->snoozeWeekendsAlarm = true;
        this->snoozeWeekendsTime[0] = timeinfo.tm_hour;
        this->snoozeWeekendsTime[1] = timeinfo.tm_min;
    }
    if (this->currentAlarm == 3) {
        lv_obj_add_state(this->alarmClock->oneOffSwitch, LV_STATE_CHECKED);
        this->snoozeOneOffAlarm = true;
        this->snoozeOneOffTime[0] = timeinfo.tm_hour;
        this->snoozeOneOffTime[1] = timeinfo.tm_min;
    }
    this->stop();
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
    if (lv_obj_has_state(this->alarmClock->weekendsSwitch, LV_STATE_CHECKED)) {
        int hourFromLabel = this->alarmClock->parse_alarm_label(
            lv_label_get_text(this->alarmClock->weekendsButtonLabel), true);
        int minuteFromLabel = this->alarmClock->parse_alarm_label(
            lv_label_get_text(this->alarmClock->weekendsButtonLabel), false);
        if (this->isWeekends(timeinfo.tm_wday)) {
            if (this->snoozeWeekendsAlarm) {
                hourFromLabel = this->snoozeWeekendsTime[0];
                minuteFromLabel = this->snoozeWeekendsTime[1];
                this->calculateSnoozeTime(hourFromLabel, minuteFromLabel);
                if (snoozeWeekendsCount >= snoozeMaxCount) {
                    this->snoozeWeekendsAlarm = false;
                }
            }
            if (timeinfo.tm_hour == hourFromLabel & timeinfo.tm_min == minuteFromLabel) {
                if (!this->weekendsAlarmAlreadyFired) {
                    this->fire(timeinfo.tm_hour, timeinfo.tm_min);
                    this->weekendsAlarmAlreadyFired = true;
                    this->currentAlarm = 2;
                    if (this->snoozeWeekendsAlarm) {
                        this->snoozeWeekendsCount++;
                    }
                }
            } else {
                this->weekendsAlarmAlreadyFired = false;
            }
        }
        this->calculateWeekendsRemainingTime(hourFromLabel, minuteFromLabel, timeinfo);
    } else {
        this->snoozeWeekendsAlarm = false;
        this->weekendsAlarmAlreadyFired = false;
        this->snoozeWeekendsCount = 1;
        this->snoozeWeekendsTime[0] = 0;
        this->snoozeWeekendsTime[1] = 0;
        lv_label_set_text(this->alarmClock->weekendsRingsInLabel, "");
    }
}

void TimeApp::checkWeekdaysAlarmClock(tm &timeinfo) {
    if (lv_obj_has_state(this->alarmClock->weekdaysSwitch, LV_STATE_CHECKED)) {
        int hourFromLabel = this->alarmClock->parse_alarm_label(
            lv_label_get_text(this->alarmClock->weekdaysButtonLabel), true);
        int minuteFromLabel = this->alarmClock->parse_alarm_label(
            lv_label_get_text(this->alarmClock->weekdaysButtonLabel), false);
        if (!this->isWeekends(timeinfo.tm_wday)) {
            if (this->snoozeWeekdaysAlarm) {
                hourFromLabel = this->snoozeWeekdaysTime[0];
                minuteFromLabel = this->snoozeWeekdaysTime[1];
                this->calculateSnoozeTime(hourFromLabel, minuteFromLabel);
                if (snoozeWeekdaysCount >= snoozeMaxCount) {
                    this->snoozeWeekdaysAlarm = false;
                }
            }
            if (timeinfo.tm_hour == hourFromLabel & timeinfo.tm_min == minuteFromLabel) {
                if (!this->weekdaysAlarmAlreadyFired) {
                    this->fire(timeinfo.tm_hour, timeinfo.tm_min);
                    this->weekdaysAlarmAlreadyFired = true;
                    this->currentAlarm = 1;
                    if (this->snoozeWeekdaysAlarm) {
                        this->snoozeWeekdaysCount++;
                    }
                }
            } else {
                this->weekdaysAlarmAlreadyFired = false;
            }
        }
        this->calculateWeekdaysRemainingTime(hourFromLabel, minuteFromLabel, timeinfo);
    } else {
        this->snoozeWeekdaysAlarm = false;
        this->weekdaysAlarmAlreadyFired = false;
        this->snoozeWeekdaysCount = 1;
        this->snoozeWeekdaysTime[0] = 0;
        this->snoozeWeekdaysTime[1] = 0;
        lv_label_set_text(this->alarmClock->weekdaysRingsInLabel, "");
    }
}

void TimeApp::checkOneOffAlarmClock(tm &timeinfo) {
    if (lv_obj_has_state(this->alarmClock->oneOffSwitch, LV_STATE_CHECKED)) {
        int hourFromLabel = this->alarmClock->parse_alarm_label(
            lv_label_get_text(this->alarmClock->oneOffButtonLabel), true);
        int minuteFromLabel = this->alarmClock->parse_alarm_label(
            lv_label_get_text(this->alarmClock->oneOffButtonLabel), false);
        if (this->snoozeOneOffAlarm) {
            hourFromLabel = this->snoozeOneOffTime[0];
            minuteFromLabel = this->snoozeOneOffTime[1];
            this->calculateSnoozeTime(hourFromLabel, minuteFromLabel);
            if (snoozeOneOffCount >= snoozeMaxCount) {
                this->snoozeOneOffAlarm = false;
                lv_obj_clear_state(this->alarmClock->oneOffSwitch, LV_STATE_CHECKED);
            }
        }
        if (timeinfo.tm_hour == hourFromLabel & timeinfo.tm_min == minuteFromLabel) {
            if (!this->oneOffAlarmAlreadyFired) {
                this->fire(timeinfo.tm_hour, timeinfo.tm_min);
                this->oneOffAlarmAlreadyFired = true;
                lv_obj_clear_state(this->alarmClock->oneOffSwitch, LV_STATE_CHECKED);
                this->alarmClock->event_alarmSwitch_cb();
                this->currentAlarm = 3;
                if (snoozeOneOffAlarm) {
                    this->snoozeOneOffCount++;
                }
            }
        } else {
            this->oneOffAlarmAlreadyFired = false;
        }
        this->calculateOneOffRemainingTime(hourFromLabel, minuteFromLabel, timeinfo);
    } else {
        this->snoozeOneOffAlarm = false;
        this->oneOffAlarmAlreadyFired = false;
        this->snoozeWeekdaysCount = 1;
        this->snoozeOneOffTime[0] = 0;
        this->snoozeOneOffTime[1] = 0;
        lv_label_set_text(this->alarmClock->oneOffRingsInLabel, "");
    }
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
    this->setRingsInLabelText(difference, this->alarmClock->oneOffRingsInLabel);
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
    this->setRingsInLabelText(difference, this->alarmClock->weekendsRingsInLabel);
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
    this->setRingsInLabelText(difference, this->alarmClock->weekdaysRingsInLabel);
}

void TimeApp::calculateSnoozeTime(int &hours, int &minutes) {
    minutes = minutes + this->snoozePeriodInMinutes;
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
    return (weekDay == 0 | weekDay == 6) ? true : false;
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

void TimeApp::fire(int hour, int minute) {
    this->alarmClock->show_alarm(hour, minute);
    this->stateApp->alarmState->alarmIsRinging = true;
    lv_msg_send(MSG_ALARM_PLAY, NULL);
}
void TimeApp::stop() {
    this->alarmClock->delete_alarm_modal_panel();
    lv_msg_send(MSG_SOUND_STOP, NULL);
    this->stateApp->alarmState->alarmIsRinging = false;
}

TimeApp::~TimeApp() {}