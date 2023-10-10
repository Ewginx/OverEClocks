#include "lvgl.h"
#include <stdio.h>
class AlarmScreen
{
private:
    short int hour_label = 0;
    short int minute_label = 3;
    short int hour = 24;
    short int minute = 60;

public:
    lv_obj_t *ui_AlarmScreen;
    lv_obj_t *ui_AlarmPanel;
    lv_obj_t *ui_AlarmWorkingDayLabel;
    lv_obj_t *ui_AlarmWeekendDayLabel;
    lv_obj_t *ui_AlarmOneOffLabel;
    lv_obj_t *ui_AlarmWorkingDaySwitch;
    lv_obj_t *ui_AlarmWeekendDaySwitch;
    lv_obj_t *ui_AlarmOneOffSwitch;
    lv_obj_t *ui_AlarmWorkingDayButton;
    lv_obj_t *ui_AlarmWorkingDayButtonLabel;
    lv_obj_t *ui_AlarmWeekendDayButton;
    lv_obj_t *ui_AlarmWeekendDayButtonLabel;
    lv_obj_t *ui_AlarmOneOffButton;
    lv_obj_t *ui_AlarmOneOffButtonLabel;
    lv_obj_t *ui_AlarmModalPanel;
    lv_obj_t *ui_AlarmHourRoller;
    lv_obj_t *ui_AlarmMinuteRoller;
    lv_obj_t *ui_AlarmModalCancelButton;
    lv_obj_t *ui_AlarmModalCancelButtonLabel;
    lv_obj_t *ui_AlarmModalOkButton;
    lv_obj_t *ui_AlarmModalOkButtonLabel;
    lv_obj_t *ui_AlarmDummyPanel;
    lv_obj_t *target_label;

    void event_AlarmModalCancelButton_cb(lv_event_t *e);
    void event_AlarmModalOkButton_cb(lv_event_t *e);
    void event_WorkingDayButton_cb(lv_event_t *e);
    void event_WeekendButton_cb(lv_event_t *e);
    void event_OneOffButton_cb(lv_event_t *e);
    void parse_alarm_label(char *string, int pos, char *buff);
    void ui_set_roller_time(const lv_obj_t *label);
    void create_alarm_modal_panel(lv_obj_t *target_label);
    void delete_alarm_modal_panel();
    AlarmScreen(/* args */);
    ~AlarmScreen();
};


