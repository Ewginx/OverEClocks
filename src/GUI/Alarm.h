#pragma once
#include "lvgl.h"
#include <stdio.h>
#include "Translation/Translation.h"
#include "Config/Config.h"

class Alarm
{
private:
    short int hour_position_on_label = 0;
    short int minute_position_on_label = 3;
    bool workdays_enabled = false;
    bool weekend_enabled = false;
    bool oneOff_enabled = false;

public:
    lv_obj_t *alarmScreen;
    lv_obj_t *alarmPanel;

    lv_obj_t *weekdaysLabel;
    lv_obj_t *weekdaysButton;
    lv_obj_t *weekdaysButtonLabel;
    lv_obj_t *weekdaysSwitch;

    lv_obj_t *weekendLabel;
    lv_obj_t *weekendSwitch;
    lv_obj_t *weekendButton;
    lv_obj_t *weekendButtonLabel;

    lv_obj_t *oneOffSwitch;
    lv_obj_t *oneOffLabel;
    lv_obj_t *oneOffButton;
    lv_obj_t *oneOffButtonLabel;

    lv_obj_t *alarmModalPanel;
    lv_obj_t *hourRoller;
    lv_obj_t *minuteRoller;
    lv_obj_t *modalCancelButton;
    lv_obj_t *modalCancelButtonLabel;
    lv_obj_t *modalOkButton;
    lv_obj_t *modalOkButtonLabel;
    lv_obj_t *alarmDummyPanel;

    lv_obj_t *target_label;

    void parse_alarm_label(char *string, int pos, char *buff);
    void set_roller_time(const lv_obj_t *label);

    void create_alarm_modal_panel(lv_obj_t *target_label);
    void delete_alarm_modal_panel();

    void event_alarmModalCancelButton_cb(lv_event_t *e);
    void event_alarmModalOkButton_cb(lv_event_t *e);

    void event_alarmButtons_cb(lv_event_t *e);

    void event_alarm_switch_cb(lv_event_t *e);

    Alarm(/* args */);
    ~Alarm();
};
