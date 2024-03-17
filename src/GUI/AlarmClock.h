#pragma once
#include "Apps/StateApp.h"
#include "Config/Config.h"
#include "Preferences.h"
#include "Translation/Translation.h"
#include "lvgl.h"
#include <stdio.h>

class AlarmClock {
  private:
    char _hour_count[HOUR_COUNT * 3] = {0};
    char _minute_count[MINUTE_COUNT * 3] = {0};

    StateApp *_state_app;

    void set_roller_time(const lv_obj_t *label);

    void create_roller_modal_panel(lv_obj_t *target_label);
    void create_roller_data();
    void create_alarm_modal_panel(lv_obj_t *target_label);

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

    void change_alarm_panel_parent_screen();
    void delete_alarm_modal_panel();
    void show_alarm(lv_obj_t *target_label);
    void delete_roller_modal_panel();

    int parse_alarm_label(char *string, bool hour = true);

    void event_alarmModalCancelButton_cb(lv_event_t *e);
    void event_alarmModalOkButton_cb(lv_event_t *e);
    void event_offAlarmButton_cb(lv_event_t *e);
    void event_snoozeAlarmButton_cb(lv_event_t *e);
    void event_alarmButtons_cb(lv_event_t *e);
    void event_alarmSwitch_cb();

    void set_default_values();

    void set_alarm_clock_gui();

    void set_alarm_switches(bool weekdays_sw, bool weekends_sw, bool oneOff_sw);
    void set_alarm_buttons(const char *weekdays_time, const char *weekends_time,
                           const char *oneOff_time);

    AlarmClock(StateApp *state_app);
    ~AlarmClock();
};
