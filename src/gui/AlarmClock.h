#pragma once
#include "Apps/StateApp.h"
#include "Config/Config.h"
#include "Preferences.h"
#include "Translation/Translation.h"
#include "lvgl.h"
#include <stdio.h>

class AlarmClock {

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

    lv_obj_t *targetLabel;

    void changeAlarmPanelParentScreen();
    void deleteAlarmModalPanel();
    void showAlarm(int hour, int minute);
    void deleteRollerModalPanel();

    void eventAlarmModalCancelButtonCallback(lv_event_t *e);
    void eventAlarmModalOkButtonCallback(lv_event_t *e);
    void eventOffAlarmButtonCallback(lv_event_t *e);
    void eventSnoozeAlarmButtonCallback(lv_event_t *e);
    void eventAlarmButtonsCallback(lv_event_t *e);
    void eventAlarmSwitchCallback();

    void setDefaultValues();

    void setAlarmClockGui();

    int getHourFromAlarmLabel(lv_obj_t *alarmLabel);
    int getMinuteFromAlarmLabel(lv_obj_t *alarmLabel);

    bool isAlarmDisabled(lv_obj_t *alarmSwitch);

    AlarmClock(StateApp *stateApp);
    ~AlarmClock();

  private:
    char hourCount[HOUR_COUNT * 3] = {0};
    char minuteCount[MINUTE_COUNT * 3] = {0};

    StateApp *stateApp;
    void createAlarmClockWidgets();

    void setRollerTime(const lv_obj_t *label);

    void createRollerModalPanel(lv_obj_t *targetLabel);
    void createRollerData();
    void createAlarmModalPanel(int hour, int minute);

    void setAlarmSwitches(bool weekdaysSwitch, bool weekendsSwitch, bool oneOffSwitch);
    void setAlarmButtons(const char *weekdaysTime, const char *weekendsTime,
                         const char *oneOffTime);

    int parseAlarmLabel(char *string, bool hour = true);
};
