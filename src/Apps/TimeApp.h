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
  private:
    StateApp *_state_app;

    DigitalClock *digital_clock;
    AnalogClock *analog_clock;
    AlarmClock *alarm_clock;

    lv_timer_t *_time_update_timer = NULL;

    bool weekdays_already_fired = false;
    bool weekends_already_fired = false;

    struct tm timeinfo;

    const char *ntpServer = "pool.ntp.org";

    static void copy_timeinfo_struct(struct tm &new_tm, struct tm &old_tm);
    static bool is_weekends(int week_day);

    void check_weekends_alarm_clock(tm &timeinfo);
    void check_weekdays_alarm_clock(tm &timeinfo);
    void check_oneOff_alarm_clock(tm &timeinfo);

    void calculate_oneOff_remaining_time(int hour, int minute, struct tm &timeinfo);
    void calculate_weekends_remaining_time(int hour, int minute, struct tm &timeinfo);
    void calculate_weekdays_remaining_time(int hour, int minute, struct tm &timeinfo);

    void set_rings_in_label_text(double &difference_in_seconds, lv_obj_t *rings_in_label);

  public:
    void check_alarm_clocks(tm &timeinfo);
    void notifyAboutTime();
    void config_time();
    void set_timezone();
    void update_time_timer();
    TimeApp(DigitalClock *digital_clock, AnalogClock *analog_clock,
            AlarmClock *alarm_clock, StateApp *state_app);
    ~TimeApp();
};
