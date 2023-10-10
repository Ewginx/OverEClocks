#include "lvgl.h"

class AlarmScreen
{
private:
    /* data */
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
    AlarmScreen(/* args */);
    ~AlarmScreen();
};


