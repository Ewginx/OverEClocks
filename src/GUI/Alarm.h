#pragma once
#include "lvgl.h"
#include <stdio.h>
class Alarm
{
private:
    short int hour_label = 0;
    short int minute_label = 3;
    short int hour = 24;
    short int minute = 60;

public:
    lv_obj_t *alarmScreen;
    lv_obj_t *alarmPanel;
    lv_obj_t *workingDayLabel;
    lv_obj_t *weekendDayLabel;
    lv_obj_t *oneOffLabel;
    lv_obj_t *workingDaySwitch;
    lv_obj_t *weekendDaySwitch;
    lv_obj_t *oneOffSwitch;
    lv_obj_t *workingDayButton;
    lv_obj_t *workingDayButtonLabel;
    lv_obj_t *weekendDayButton;
    lv_obj_t *weekendDayButtonLabel;
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

    void event_alarmModalCancelButton_cb(lv_event_t *e);
    void event_alarmModalOkButton_cb(lv_event_t *e);
    void event_workingDayButton_cb(lv_event_t *e);
    void event_weekendButton_cb(lv_event_t *e);
    void event_oneOffButton_cb(lv_event_t *e);
    void parse_alarm_label(char *string, int pos, char *buff);
    void ui_set_roller_time(const lv_obj_t *label);
    void create_alarm_modal_panel(lv_obj_t *target_label);
    void delete_alarm_modal_panel();
    Alarm(/* args */);
    ~Alarm();
};


