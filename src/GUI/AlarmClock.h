#pragma once
#include "Apps/StateApp.h"
#include "Config/Config.h"
#include "Preferences.h"
#include "Translation/Translation.h"
#include "lvgl.h"
#include <stdio.h>
#include <time.h>

class AlarmClock {
  private:
    bool weekdays_already_fired = false;
    bool weekends_already_fired = false;

    char _hour_count[HOUR_COUNT * 3] = {0};
    char _minute_count[MINUTE_COUNT * 3] = {0};

    StateApp *_state_app;

    int parse_alarm_label(char *string, bool hour = true);
    void set_roller_time(const lv_obj_t *label);

    void create_roller_modal_panel(lv_obj_t *target_label);
    void create_roller_data();
    void create_alarm_modal_panel(lv_obj_t *target_label);
    void delete_alarm_modal_panel();

    static bool is_weekends(int week_day);
    static void copy_timeinfo_struct(struct tm &new_tm, struct tm &old_tm);

    void check_weekends_alarm_clock(tm &timeinfo);
    void check_weekdays_alarm_clock(tm &timeinfo);
    void check_oneOff_alarm_clock(tm &timeinfo);

    void fire_alarm(lv_obj_t *target_label);

    void calculate_oneOff_remaining_time(int hour, int minute, struct tm &timeinfo);
    void calculate_weekends_remaining_time(int hour, int minute, struct tm &timeinfo);
    void calculate_weekdays_remaining_time(int hour, int minute, struct tm &timeinfo);

    void set_rings_in_label_text(double &difference_in_seconds, lv_obj_t *rings_in_label);
    void set_default_values();

  public:
    lv_obj_t *alarmScreen;
    lv_obj_t *alarmPanel;

    lv_obj_t *weekdaysLabel;
    lv_obj_t *weekdaysButton;
    lv_obj_t *weekdaysButtonLabel;
    lv_obj_t *weekdaysSwitch;
    lv_obj_t *weekdaysRingsInLabel;

    lv_obj_t *weekendsLabel;
    lv_obj_t *weekendsSwitch;
    lv_obj_t *weekendsButton;
    lv_obj_t *weekendsButtonLabel;
    lv_obj_t *weekendsRingsInLabel;

    lv_obj_t *oneOffSwitch;
    lv_obj_t *oneOffLabel;
    lv_obj_t *oneOffButton;
    lv_obj_t *oneOffButtonLabel;
    lv_obj_t *oneOffRingsInLabel;

    lv_obj_t *alarmDummyPanel;
    lv_obj_t *alarmModalPanel;

    lv_obj_t *alarmTimeLabel;

    lv_obj_t *hourRoller;
    lv_obj_t *minuteRoller;

    lv_obj_t *modalCancelButton;
    lv_obj_t *modalCancelButtonLabel;
    lv_obj_t *modalOkButton;
    lv_obj_t *modalOkButtonLabel;

    lv_obj_t *target_label;

    void check_alarm_clocks(tm &timeinfo);

    void delete_roller_modal_panel();

    void event_alarmModalCancelButton_cb(lv_event_t *e);
    void event_alarmModalOkButton_cb(lv_event_t *e);
    void event_offAlarmButton_cb(lv_event_t *e);
    void event_alarmButtons_cb(lv_event_t *e);
    void event_alarmSwitch_cb(lv_event_t *e);

    void set_alarm_switches(bool weekdays_sw, bool weekends_sw, bool oneOff_sw);
    void set_alarm_buttons(const char *weekdays_time, const char *weekends_time,
                           const char *oneOff_time);

    AlarmClock(StateApp *state_app);
    ~AlarmClock();
};
