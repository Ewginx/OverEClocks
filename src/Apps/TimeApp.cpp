#include "Apps/TimeApp.h"
#include "TimeApp.h"

static TimeApp *instance = NULL;

extern "C" void update_tz_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->set_timezone();
}

extern "C" void update_time_cb_wrapper(lv_timer_t *timer) { instance->notifyAboutTime(); }
extern "C" void update_time_timer_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->update_time_timer();
}

TimeApp::TimeApp(DigitalClock *digital_clock, AnalogClock *analog_clock,
                 AlarmClock *alarm_clock, StateApp *state_app) {
    instance = this;
    this->_state_app = state_app;
    this->alarm_clock = alarm_clock;
    this->analog_clock = analog_clock;
    this->digital_clock = digital_clock;

    _time_update_timer =
        lv_timer_create(update_time_cb_wrapper, TIME_UPDATE_INTERVAL, NULL);
    lv_timer_pause(this->_time_update_timer);

    lv_msg_subscribe(MSG_UPDATE_TZ, update_tz_cb_wrapper, NULL);
    lv_msg_subscribe(MSG_UPDATE_TIME_TIMER, update_time_timer_cb_wrapper, NULL);
}

void TimeApp::config_time() {
    if (this->_state_app->wifi_state->wifi_connected) {
        configTime(0, 0, "pool.ntp.org");
        this->set_timezone();
        if (getLocalTime(&timeinfo)) {
            this->_state_app->time_state->time_is_set = true;
        }
    }
}

void TimeApp::notifyAboutTime() {
    getLocalTime(&timeinfo);
    analog_clock->set_time(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    digital_clock->set_time(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    digital_clock->set_date(timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year,
                            timeinfo.tm_wday);
}
void TimeApp::check_alarm_clocks(struct tm &timeinfo) {
    this->check_weekends_alarm_clock(timeinfo);
    this->check_weekdays_alarm_clock(timeinfo);
    this->check_oneOff_alarm_clock(timeinfo);
}

void TimeApp::check_weekends_alarm_clock(tm &timeinfo) {
    if (lv_obj_has_state(this->alarm_clock->weekendsSwitch, LV_STATE_CHECKED)) {
        int hour_from_label = this->alarm_clock->parse_alarm_label(
            lv_label_get_text(this->alarm_clock->weekendsButtonLabel), true);
        int minute_from_label = this->alarm_clock->parse_alarm_label(
            lv_label_get_text(this->alarm_clock->weekendsButtonLabel), false);
        if (this->is_weekends(timeinfo.tm_wday)) {

            if (timeinfo.tm_hour == hour_from_label &
                timeinfo.tm_min == minute_from_label) {
                if (!this->weekends_already_fired) {
                    this->alarm_clock->fire_alarm(this->alarm_clock->weekendsButtonLabel);
                    this->weekends_already_fired = true;
                }
            } else {
                this->weekends_already_fired = false;
            }
        }
        this->calculate_weekends_remaining_time(hour_from_label, minute_from_label,
                                                timeinfo);
    } else {
        lv_label_set_text(this->alarm_clock->weekendsRingsInLabel, "");
    }
}

void TimeApp::check_weekdays_alarm_clock(tm &timeinfo) {
    if (lv_obj_has_state(this->alarm_clock->weekdaysSwitch, LV_STATE_CHECKED)) {
        int hour_from_label = this->alarm_clock->parse_alarm_label(
            lv_label_get_text(this->alarm_clock->weekdaysButtonLabel), true);
        int minute_from_label = this->alarm_clock->parse_alarm_label(
            lv_label_get_text(this->alarm_clock->weekdaysButtonLabel), false);
        if (!this->is_weekends(timeinfo.tm_wday)) {

            if (timeinfo.tm_hour == hour_from_label &
                timeinfo.tm_min == minute_from_label) {
                if (!this->weekdays_already_fired) {
                    this->alarm_clock->fire_alarm(this->alarm_clock->weekdaysButtonLabel);
                    this->weekdays_already_fired = true;
                }
            } else {
                this->weekdays_already_fired = false;
            }
        }
        this->calculate_weekdays_remaining_time(hour_from_label, minute_from_label,
                                                timeinfo);
    } else {
        lv_label_set_text(this->alarm_clock->weekdaysRingsInLabel, "");
    }
}

void TimeApp::check_oneOff_alarm_clock(tm &timeinfo) {
    if (lv_obj_has_state(this->alarm_clock->oneOffSwitch, LV_STATE_CHECKED)) {
        int hour_from_label = this->alarm_clock->parse_alarm_label(
            lv_label_get_text(this->alarm_clock->oneOffButtonLabel), true);
        int minute_from_label = this->alarm_clock->parse_alarm_label(
            lv_label_get_text(this->alarm_clock->oneOffButtonLabel), false);
        if (timeinfo.tm_hour == hour_from_label & timeinfo.tm_min == minute_from_label) {
            this->alarm_clock->fire_alarm(this->alarm_clock->oneOffButtonLabel);
            lv_obj_clear_state(this->alarm_clock->oneOffSwitch, LV_STATE_CHECKED);
            this->alarm_clock->event_alarmSwitch_cb();
        } else {
            this->calculate_oneOff_remaining_time(hour_from_label, minute_from_label,
                                                  timeinfo);
        }
    } else {
        lv_label_set_text(this->alarm_clock->oneOffRingsInLabel, "");
    }
}

void TimeApp::calculate_oneOff_remaining_time(int hour, int minute, struct tm &timeinfo) {
    struct tm timeinfo_local;
    this->copy_timeinfo_struct(timeinfo_local, timeinfo);
    time_t now = mktime(&timeinfo_local);
    if (timeinfo_local.tm_hour >= hour & timeinfo_local.tm_min >= minute) {
        timeinfo_local.tm_mday += 1;
    }
    timeinfo_local.tm_hour = hour;
    timeinfo_local.tm_min = minute;
    time_t next_time = mktime(&timeinfo_local);
    double difference = difftime(next_time, now);
    this->set_rings_in_label_text(difference, this->alarm_clock->oneOffRingsInLabel);
}

void TimeApp::calculate_weekends_remaining_time(int hour, int minute,
                                                struct tm &timeinfo) {
    struct tm timeinfo_local;
    this->copy_timeinfo_struct(timeinfo_local, timeinfo);
    time_t now = mktime(&timeinfo);
    int weekdays_add[5] = {5, 4, 3, 2, 1};
    if (0 < timeinfo_local.tm_wday & timeinfo_local.tm_wday < 6) {
        timeinfo_local.tm_mday += weekdays_add[timeinfo_local.tm_wday - 1];
    }
    if (timeinfo_local.tm_wday == 0) {
        if (timeinfo_local.tm_hour > hour) {
            timeinfo_local.tm_mday += 6;
        }
    }
    if (timeinfo_local.tm_wday == 6) {
        if (timeinfo_local.tm_hour > hour) {
            timeinfo_local.tm_mday += 1;
        }
    }
    timeinfo_local.tm_hour = hour;
    timeinfo_local.tm_min = minute;
    time_t next_time = mktime(&timeinfo_local);
    double difference = difftime(next_time, now);
    this->set_rings_in_label_text(difference, this->alarm_clock->weekendsRingsInLabel);
}

void TimeApp::calculate_weekdays_remaining_time(int hour, int minute,
                                                struct tm &timeinfo) {
    struct tm timeinfo_local;
    this->copy_timeinfo_struct(timeinfo_local, timeinfo);
    time_t now = mktime(&timeinfo_local);
    if (0 < timeinfo_local.tm_wday < 5) {
        if (timeinfo_local.tm_hour > hour) {
            timeinfo_local.tm_mday += 1;
        }
    }
    if (timeinfo_local.tm_wday == 5) {
        if (timeinfo_local.tm_hour > hour) {
            timeinfo_local.tm_mday += 3;
        }
    }
    if (timeinfo_local.tm_wday == 6) {
        if (timeinfo_local.tm_hour > hour) {
            timeinfo_local.tm_mday += 1;
        } else {
            timeinfo_local.tm_mday += 2;
        }
    }
    if (timeinfo_local.tm_wday == 0) {
        if (timeinfo_local.tm_hour > hour) {
            timeinfo_local.tm_mday += 2;
        } else {
            timeinfo_local.tm_mday += 1;
        }
    }
    timeinfo_local.tm_hour = hour;
    timeinfo_local.tm_min = minute;
    time_t next_time = mktime(&timeinfo_local);
    double difference = difftime(next_time, now);
    this->set_rings_in_label_text(difference, this->alarm_clock->weekdaysRingsInLabel);
}

void TimeApp::set_rings_in_label_text(double &difference_in_seconds,
                                      lv_obj_t *rings_in_label) {
    String time;
    time.reserve(28);
    time += alarm_translation[rings_in];
    if (difference_in_seconds > 86400) {
        time += (int)difference_in_seconds / 86400;
        time += alarm_translation[day_short];
    }
    time += (int)difference_in_seconds % 86400 / 3600;
    time += alarm_translation[hour_short];
    time += (int)difference_in_seconds % 86400 % 3600 / 60;
    time += alarm_translation[minute_short];
    lv_label_set_text(rings_in_label, time.c_str());
}
bool TimeApp::is_weekends(int week_day) {
    if (week_day == 0 | week_day == 6) {
        return true;
    }
    return false;
}
void TimeApp::copy_timeinfo_struct(tm &new_tm, tm &old_tm) {
    new_tm.tm_hour = old_tm.tm_hour;
    new_tm.tm_min = old_tm.tm_min;
    new_tm.tm_mday = old_tm.tm_mday;
    new_tm.tm_sec = old_tm.tm_sec;
    new_tm.tm_mon = old_tm.tm_mon;
    new_tm.tm_year = old_tm.tm_year;
    new_tm.tm_wday = old_tm.tm_wday;
    new_tm.tm_yday = old_tm.tm_yday;
    new_tm.tm_isdst = old_tm.tm_isdst;
}
void TimeApp::set_timezone() {
    setenv("TZ", this->_state_app->time_state->timezone_posix.c_str(), 1);
    tzset();
}
void TimeApp::update_time_timer() {
    this->config_time();
    if (this->_state_app->wifi_state->wifi_connected ||
        this->_state_app->time_state->time_is_set) {
        lv_timer_resume(this->_time_update_timer);
    } else {
        lv_timer_pause(this->_time_update_timer);
    }
}
TimeApp::~TimeApp() {}