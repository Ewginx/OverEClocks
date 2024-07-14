#include "AlarmClock.h"

static AlarmClock *instance;

extern "C" void eventAlarmModalCancelButtonCallbackWrapper(lv_event_t *e) {
    instance->eventAlarmModalCancelButtonCallback(e);
}
extern "C" void event_alarmModalOkButton_cb_wrapper(lv_event_t *e) {
    instance->eventAlarmModalOkButtonCallback(e);
}

extern "C" void eventOffAlarmButtonCallbackWrapper(lv_event_t *e) {
    instance->eventOffAlarmButtonCallback(e);
}

extern "C" void eventSnoozeAlarmButtonCallbackWrapper(lv_event_t *e) {
    instance->eventSnoozeAlarmButtonCallback(e);
}

extern "C" void eventAlarmButtonsCallbackWrapper(lv_event_t *e) {
    instance->eventAlarmButtonsCallback(e);
}

extern "C" void eventAlarmSwitchCallbackWrapper(lv_event_t *e) {
    instance->eventAlarmSwitchCallback();
}

extern "C" void updateAlarmClockGuiCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->setAlarmClockGui();
}

AlarmClock::AlarmClock(StateApp *stateApp) {
    instance = this;
    this->stateApp = stateApp;

    this->createAlarmClockWidgets();

    this->setDefaultValues();

    lv_obj_add_event_cb(weekdaysButton, eventAlarmButtonsCallbackWrapper,
                        LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(weekendsButton, eventAlarmButtonsCallbackWrapper,
                        LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(oneOffButton, eventAlarmButtonsCallbackWrapper, LV_EVENT_CLICKED,
                        NULL);

    lv_obj_add_event_cb(weekdaysSwitch, eventAlarmSwitchCallbackWrapper,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(weekendsSwitch, eventAlarmSwitchCallbackWrapper,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(oneOffSwitch, eventAlarmSwitchCallbackWrapper,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_msg_subscribe(MSG_UPDATE_ALARM_GUI, updateAlarmClockGuiCallbackWrapper, NULL);
}

void AlarmClock::createAlarmClockWidgets() {
    alarmModalPanel = NULL;
    this->createRollerData();

    alarmScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(alarmScreen, LV_OBJ_FLAG_SCROLLABLE);

    alarmPanel = lv_obj_create(alarmScreen);
    lv_obj_set_size(alarmPanel, lv_pct(100), lv_pct(100));
    lv_obj_set_pos(alarmPanel, 1, -1);
    lv_obj_set_align(alarmPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(alarmPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(alarmPanel, LV_OBJ_FLAG_EVENT_BUBBLE);

    weekdaysLabel = lv_label_create(alarmPanel);
    lv_obj_set_size(weekdaysLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(weekdaysLabel, 10, 65);
    lv_obj_set_align(weekdaysLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(weekdaysLabel, alarm_translation[working_day_label]);
    lv_obj_set_style_text_font(weekdaysLabel, &font_18, LV_PART_MAIN);

    weekdaysRingsInLabel = lv_label_create(alarmPanel);
    lv_obj_set_size(weekdaysRingsInLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_text_font(weekdaysRingsInLabel, &font_14, LV_PART_MAIN);
    lv_obj_align_to(weekdaysRingsInLabel, weekdaysLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    weekdaysButton = lv_btn_create(alarmPanel);
    lv_obj_set_size(weekdaysButton, 70, 41);
    lv_obj_align_to(weekdaysButton, weekdaysLabel, LV_ALIGN_BOTTOM_LEFT, 270, 10);
    lv_obj_set_style_bg_opa(weekdaysButton, 0, 0);
    lv_obj_add_flag(weekdaysButton, LV_OBJ_FLAG_EVENT_BUBBLE);

    weekdaysButtonLabel = lv_label_create(weekdaysButton);
    lv_obj_set_size(weekdaysButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(weekdaysButtonLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(weekdaysButtonLabel, &font_18, LV_PART_MAIN);

    weekdaysSwitch = lv_switch_create(alarmPanel);
    lv_obj_set_size(weekdaysSwitch, 50, 25);
    lv_obj_align_to(weekdaysSwitch, weekdaysButton, LV_ALIGN_BOTTOM_LEFT, 80, 5);
    lv_obj_add_flag(weekdaysSwitch, LV_OBJ_FLAG_EVENT_BUBBLE);

    weekendsLabel = lv_label_create(alarmPanel);
    lv_obj_set_size(weekendsLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_align_to(weekendsLabel, weekdaysLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40);
    lv_label_set_text(weekendsLabel, alarm_translation[weekend_day_label]);
    lv_obj_set_style_text_font(weekendsLabel, &font_18, LV_PART_MAIN);

    weekendsRingsInLabel = lv_label_create(alarmPanel);
    lv_obj_set_size(weekendsRingsInLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_text_font(weekendsRingsInLabel, &font_14, LV_PART_MAIN);
    lv_obj_align_to(weekendsRingsInLabel, weekendsLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    weekendsButton = lv_btn_create(alarmPanel);
    lv_obj_set_size(weekendsButton, 70, 41);
    lv_obj_align_to(weekendsButton, weekdaysButton, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_style_bg_opa(weekendsButton, 0, 0);
    lv_obj_set_style_text_font(weekendsButton, &font_18, LV_PART_MAIN);
    lv_obj_add_flag(weekendsButton, LV_OBJ_FLAG_EVENT_BUBBLE);

    weekendsButtonLabel = lv_label_create(weekendsButton);
    lv_obj_set_size(weekendsButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(weekendsButtonLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(weekendsButtonLabel, &font_18, LV_PART_MAIN);

    weekendsSwitch = lv_switch_create(alarmPanel);
    lv_obj_set_size(weekendsSwitch, 50, 25);
    lv_obj_align_to(weekendsSwitch, weekdaysSwitch, LV_ALIGN_OUT_BOTTOM_MID, 0, 35);
    lv_obj_add_flag(weekendsSwitch, LV_OBJ_FLAG_EVENT_BUBBLE);

    oneOffLabel = lv_label_create(alarmPanel);
    lv_obj_set_size(oneOffLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_align_to(oneOffLabel, weekendsLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40);
    lv_label_set_text(oneOffLabel, alarm_translation[oneoff_day_label]);
    lv_obj_set_style_text_font(oneOffLabel, &font_18, LV_PART_MAIN);

    oneOffRingsInLabel = lv_label_create(alarmPanel);
    lv_obj_set_size(oneOffRingsInLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_text_font(oneOffRingsInLabel, &font_14, LV_PART_MAIN);
    lv_obj_align_to(oneOffRingsInLabel, oneOffLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    oneOffButton = lv_btn_create(alarmPanel);
    lv_obj_set_size(oneOffButton, 70, 41);
    lv_obj_align_to(oneOffButton, weekendsButton, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_style_bg_opa(oneOffButton, 0, 0);
    lv_obj_add_flag(oneOffButton, LV_OBJ_FLAG_EVENT_BUBBLE);

    oneOffButtonLabel = lv_label_create(oneOffButton);
    lv_obj_set_size(oneOffButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(oneOffButtonLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(oneOffButtonLabel, &font_18, LV_PART_MAIN);

    oneOffSwitch = lv_switch_create(alarmPanel);
    lv_obj_set_size(oneOffSwitch, 50, 25);
    lv_obj_align_to(oneOffSwitch, weekendsSwitch, LV_ALIGN_OUT_BOTTOM_MID, 0, 35);
    lv_obj_add_flag(oneOffSwitch, LV_OBJ_FLAG_EVENT_BUBBLE);
}

void AlarmClock::setRollerTime(const lv_obj_t *label) {
    lv_roller_set_selected(hourRoller, parseAlarmLabel(lv_label_get_text(label), true),
                           LV_ANIM_OFF);
    lv_roller_set_selected(minuteRoller, parseAlarmLabel(lv_label_get_text(label), false),
                           LV_ANIM_OFF);
}

void AlarmClock::createRollerModalPanel(lv_obj_t *targetLabel) {
    if (alarmModalPanel == NULL) {

        alarmDummyPanel = lv_obj_create(alarmScreen);
        lv_obj_set_size(alarmDummyPanel, 480, 320);
        lv_obj_set_align(alarmDummyPanel, LV_ALIGN_CENTER);
        lv_obj_set_style_bg_opa(alarmDummyPanel, 200, 0);
        lv_obj_add_flag(alarmDummyPanel, LV_OBJ_FLAG_EVENT_BUBBLE);

        alarmModalPanel = lv_obj_create(alarmDummyPanel);
        lv_obj_set_size(alarmModalPanel, 250, 230);
        lv_obj_set_align(alarmModalPanel, LV_ALIGN_CENTER);
        lv_obj_clear_flag(alarmModalPanel, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_flag(alarmModalPanel, LV_OBJ_FLAG_EVENT_BUBBLE);

        hourRoller = lv_roller_create(alarmModalPanel);
        lv_roller_set_options(hourRoller, this->hourCount, LV_ROLLER_MODE_INFINITE);
        lv_obj_set_size(hourRoller, 90, 110);
        lv_obj_set_pos(hourRoller, -50, -35);
        lv_obj_set_align(hourRoller, LV_ALIGN_CENTER);
        lv_obj_set_style_text_font(hourRoller, &font_18, LV_PART_MAIN);
        lv_obj_add_flag(hourRoller, LV_OBJ_FLAG_EVENT_BUBBLE);

        minuteRoller = lv_roller_create(alarmModalPanel);
        lv_roller_set_options(minuteRoller, this->minuteCount, LV_ROLLER_MODE_INFINITE);
        lv_obj_set_size(minuteRoller, 90, 110);
        lv_obj_align_to(minuteRoller, hourRoller, LV_ALIGN_BOTTOM_LEFT, 83, 18);
        lv_obj_set_style_text_font(minuteRoller, &font_18, LV_PART_MAIN);
        lv_obj_add_flag(minuteRoller, LV_OBJ_FLAG_EVENT_BUBBLE);

        modalCancelButton = lv_btn_create(alarmModalPanel);
        lv_obj_set_size(modalCancelButton, 90, 45);
        lv_obj_set_pos(modalCancelButton, 13, 0);
        lv_obj_set_align(modalCancelButton, LV_ALIGN_BOTTOM_LEFT);
        lv_obj_add_flag(modalCancelButton, LV_OBJ_FLAG_EVENT_BUBBLE);

        modalCancelButtonLabel = lv_label_create(modalCancelButton);
        lv_obj_set_size(modalCancelButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_align(modalCancelButtonLabel, LV_ALIGN_CENTER);
        lv_label_set_text(modalCancelButtonLabel, alarm_translation[cancel_button]);
        lv_obj_set_style_text_font(modalCancelButtonLabel, &font_18, LV_PART_MAIN);

        modalOkButton = lv_btn_create(alarmModalPanel);
        lv_obj_set_size(modalOkButton, 90, 45);
        lv_obj_align_to(modalOkButton, modalCancelButton, LV_ALIGN_BOTTOM_LEFT, 85, 10);
        lv_obj_add_flag(modalOkButton, LV_OBJ_FLAG_EVENT_BUBBLE);

        modalOkButtonLabel = lv_label_create(modalOkButton);
        lv_obj_set_size(modalOkButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_align(modalOkButtonLabel, LV_ALIGN_CENTER);
        lv_label_set_text(modalOkButtonLabel, alarm_translation[ok_button]);
        lv_obj_set_style_text_font(modalOkButtonLabel, &font_18, LV_PART_MAIN);

        lv_obj_add_event_cb(modalCancelButton, eventAlarmModalCancelButtonCallbackWrapper,
                            LV_EVENT_ALL, NULL);
        lv_obj_add_event_cb(modalOkButton, event_alarmModalOkButton_cb_wrapper,
                            LV_EVENT_ALL, targetLabel);
        this->targetLabel = targetLabel;
        this->setRollerTime(targetLabel);
    }
}

void AlarmClock::createRollerData() {
    char hour_buffer[4] = {0};
    char minute_buffer[4] = {0};
    for (short int i = 0; i < HOUR_COUNT; i++) {
        if (i == HOUR_COUNT - 1) {
            sprintf(hour_buffer, "%2i", i);
        } else {
            sprintf(hour_buffer, "%02i\n", i);
        }
        strcat(this->hourCount, hour_buffer);
    }
    for (short int i = 0; i < MINUTE_COUNT; i++) {
        if (i == MINUTE_COUNT - 1) {
            sprintf(minute_buffer, "%02i", i);
        } else {
            sprintf(minute_buffer, "%02i\n", i);
        }
        strcat(this->minuteCount, minute_buffer);
    }
}

void AlarmClock::deleteRollerModalPanel() {
    if (alarmModalPanel != NULL) {
        lv_obj_remove_event_cb(modalCancelButton,
                               eventAlarmModalCancelButtonCallbackWrapper);
        lv_obj_remove_event_cb(modalOkButton, event_alarmModalOkButton_cb_wrapper);
        lv_obj_del(alarmDummyPanel);
        alarmDummyPanel = NULL;
        alarmModalPanel = NULL;
        hourRoller = NULL;
        minuteRoller = NULL;
        modalCancelButton = NULL;
        modalCancelButtonLabel = NULL;
        modalOkButton = NULL;
        modalOkButtonLabel = NULL;
    }
}
void AlarmClock::showAlarm(int hour, int minute) {
    this->deleteRollerModalPanel();
    this->createAlarmModalPanel(hour, minute);
}
void AlarmClock::createAlarmModalPanel(int hour, int minute) {
    alarmDummyPanel = lv_obj_create(lv_scr_act());
    lv_obj_set_size(alarmDummyPanel, 480, 320);
    lv_obj_set_align(alarmDummyPanel, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(alarmDummyPanel, 200, 0);

    alarmModalPanel = lv_obj_create(alarmDummyPanel);
    lv_obj_set_size(alarmModalPanel, 380, 240);
    lv_obj_set_align(alarmModalPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(alarmModalPanel, LV_OBJ_FLAG_SCROLLABLE);

    alarmTimeLabel = lv_label_create(alarmModalPanel);
    lv_label_set_text_fmt(alarmTimeLabel, "%02d:%02d", hour, minute);
    lv_obj_set_align(alarmTimeLabel, LV_ALIGN_CENTER);
    lv_obj_set_pos(alarmTimeLabel, 0, -30);
    lv_obj_set_style_text_font(alarmTimeLabel, &d_clock_font_120, LV_PART_MAIN);

    modalOkButton = lv_btn_create(alarmModalPanel);
    lv_obj_set_size(modalOkButton, 130, 50);
    lv_obj_align_to(modalOkButton, modalCancelButton, LV_ALIGN_BOTTOM_LEFT, 190, -5);
    lv_obj_add_flag(modalOkButton, LV_OBJ_FLAG_EVENT_BUBBLE);

    modalOkButtonLabel = lv_label_create(modalOkButton);
    lv_obj_set_align(modalOkButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(modalOkButtonLabel, alarm_translation[stop_button]);
    lv_obj_set_style_text_font(modalOkButtonLabel, &font_18, LV_PART_MAIN);

    modalCancelButton = lv_btn_create(alarmModalPanel);
    lv_obj_set_size(modalCancelButton, 130, 50);
    lv_obj_set_pos(modalCancelButton, 20, -5);
    lv_obj_set_align(modalCancelButton, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_add_flag(modalCancelButton, LV_OBJ_FLAG_EVENT_BUBBLE);

    modalCancelButtonLabel = lv_label_create(modalCancelButton);
    lv_obj_set_size(modalCancelButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(modalCancelButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(modalCancelButtonLabel, alarm_translation[snooze_button]);
    lv_obj_set_style_text_font(modalCancelButtonLabel, &font_18, LV_PART_MAIN);

    lv_obj_add_event_cb(modalOkButton, eventOffAlarmButtonCallbackWrapper, LV_EVENT_ALL,
                        NULL);
    lv_obj_add_event_cb(modalCancelButton, eventSnoozeAlarmButtonCallbackWrapper,
                        LV_EVENT_ALL, NULL);
}

void AlarmClock::deleteAlarmModalPanel() {
    if (alarmModalPanel != NULL) {
        lv_obj_remove_event_cb(modalOkButton, eventOffAlarmButtonCallbackWrapper);
        lv_obj_remove_event_cb(modalCancelButton, eventSnoozeAlarmButtonCallbackWrapper);
        lv_obj_del(alarmDummyPanel);
        alarmDummyPanel = NULL;
        alarmModalPanel = NULL;
        modalOkButton = NULL;
        modalOkButtonLabel = NULL;
        modalCancelButton = NULL;
        modalCancelButtonLabel = NULL;
    }
}

void AlarmClock::changeAlarmPanelParentScreen() {
    if (alarmModalPanel != NULL) {
        lv_obj_set_parent(this->alarmDummyPanel, lv_scr_act());
    }
}

void AlarmClock::setDefaultValues() {
    lv_label_set_text(weekdaysRingsInLabel, "");
    lv_label_set_text(weekdaysButtonLabel, "08:00");
    lv_label_set_text(weekendsRingsInLabel, "");
    lv_label_set_text(weekendsButtonLabel, "11:00");
    lv_label_set_text(oneOffRingsInLabel, "");
    lv_label_set_text(oneOffButtonLabel, "18:36");
}

void AlarmClock::setAlarmClockGui() {
    this->setAlarmButtons(this->stateApp->alarmState->weekdaysTime.c_str(),
                          this->stateApp->alarmState->weekendsTime.c_str(),
                          this->stateApp->alarmState->oneOffTime.c_str());
    this->setAlarmSwitches(this->stateApp->alarmState->weekdaysSwitchEnabled,
                           this->stateApp->alarmState->weekendsSwitchEnabled,
                           this->stateApp->alarmState->oneOffSwitchEnabled);
}

void AlarmClock::setAlarmSwitches(bool weekdaysSwitch, bool weekendsSwitch,
                                  bool oneOffSwitch) {
    if (weekdaysSwitch) {
        lv_obj_add_state(this->weekdaysSwitch, LV_STATE_CHECKED);
    } else {
        lv_obj_clear_state(this->weekdaysSwitch, LV_STATE_CHECKED);
    }
    if (weekendsSwitch) {
        lv_obj_add_state(this->weekendsSwitch, LV_STATE_CHECKED);
    } else {
        lv_obj_clear_state(this->weekendsSwitch, LV_STATE_CHECKED);
    }
    if (oneOffSwitch) {
        lv_obj_add_state(this->oneOffSwitch, LV_STATE_CHECKED);
    } else {
        lv_obj_clear_state(this->oneOffSwitch, LV_STATE_CHECKED);
    }
}

void AlarmClock::setAlarmButtons(const char *weekdaysTime, const char *weekendsTime,
                                 const char *oneOffTime) {
    lv_label_set_text(weekdaysButtonLabel, weekdaysTime);
    lv_label_set_text(weekendsButtonLabel, weekendsTime);
    lv_label_set_text(oneOffButtonLabel, oneOffTime);
}

int AlarmClock::getHourFromAlarmLabel(lv_obj_t *alarmLabel) {
    return this->parseAlarmLabel(lv_label_get_text(alarmLabel), true);
}

int AlarmClock::getMinuteFromAlarmLabel(lv_obj_t *alarmLabel) {
    return this->parseAlarmLabel(lv_label_get_text(alarmLabel), false);
}

bool AlarmClock::isAlarmDisabled(lv_obj_t *alarmSwitch) {
    return !lv_obj_has_state(alarmSwitch, LV_STATE_CHECKED);
}

void AlarmClock::eventAlarmModalCancelButtonCallback(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED) {
        this->deleteRollerModalPanel();
    }
}

void AlarmClock::eventAlarmModalOkButtonCallback(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED) {
        lv_label_set_text_fmt(this->targetLabel, "%02i:%02i",
                              lv_roller_get_selected(hourRoller),
                              lv_roller_get_selected(minuteRoller));
        this->deleteRollerModalPanel();
        this->stateApp->alarmState->saveAlarmTimes(
            lv_label_get_text(this->weekdaysButtonLabel),
            lv_label_get_text(this->weekendsButtonLabel),
            lv_label_get_text(this->oneOffButtonLabel));
    }
}

void AlarmClock::eventOffAlarmButtonCallback(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED) {
        lv_msg_send(MSG_ALARM_STOP, NULL);
    }
}

void AlarmClock::eventSnoozeAlarmButtonCallback(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED) {
        lv_msg_send(MSG_ALARM_SNOOZE, NULL);
    }
}

void AlarmClock::eventAlarmButtonsCallback(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (target == weekdaysButton) {
        targetLabel = weekdaysButtonLabel;
    } else if (target == weekendsButton) {
        targetLabel = weekendsButtonLabel;
    } else if (target == oneOffButton) {
        targetLabel = oneOffButtonLabel;
    }
    this->createRollerModalPanel(targetLabel);
}

void AlarmClock::eventAlarmSwitchCallback() {
    this->stateApp->alarmState->saveAlarmSwitchStates(
        lv_obj_has_state(this->weekdaysSwitch, LV_STATE_CHECKED),
        lv_obj_has_state(this->weekendsSwitch, LV_STATE_CHECKED),
        lv_obj_has_state(this->oneOffSwitch, LV_STATE_CHECKED));
}

int AlarmClock::parseAlarmLabel(char *string, bool hour) {
    char alarm_buff[3];
    for (size_t i = 0; i < 2; i++) {
        if (hour) {
            alarm_buff[i] = string[i];
        } else {
            alarm_buff[i] = string[i + 3];
        }
    }
    alarm_buff[2] = '\0';
    return atoi(alarm_buff);
}

AlarmClock::~AlarmClock() {}