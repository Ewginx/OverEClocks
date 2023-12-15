#include "AlarmClock.h"

static AlarmClock *instance;

extern "C" void event_alarmModalCancelButton_cb_wrapper(lv_event_t *e) {
    instance->event_alarmModalCancelButton_cb(e);
}
extern "C" void event_alarmModalOkButton_cb_wrapper(lv_event_t *e) {
    instance->event_alarmModalOkButton_cb(e);
}
extern "C" void event_offAlarmButton_cb_wrapper(lv_event_t *e) {
    instance->event_offAlarmButton_cb(e);
}
extern "C" void event_alarmButtons_cb_wrapper(lv_event_t *e) {
    instance->event_alarmButtons_cb(e);
}
extern "C" void event_alarmSwitch_cb_wrapper(lv_event_t *e) {
    instance->event_alarmSwitch_cb(e);
}
AlarmClock::AlarmClock(/* args */) {
    instance = this;
    alarmModalPanel = NULL;

    this->create_roller_data();

    alarmScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(alarmScreen,
                      LV_OBJ_FLAG_SCROLLABLE); /// Flags

    alarmPanel = lv_obj_create(alarmScreen);
    lv_obj_set_size(alarmPanel, lv_pct(100), lv_pct(100));
    lv_obj_set_pos(alarmPanel, 1, -1);
    lv_obj_set_align(alarmPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(alarmPanel,
                      LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(alarmPanel, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(alarmPanel, 0, 0);
    lv_obj_add_flag(alarmPanel, LV_OBJ_FLAG_EVENT_BUBBLE);

    weekdaysLabel = lv_label_create(alarmPanel);
    lv_obj_set_size(weekdaysLabel, LV_SIZE_CONTENT,
                    LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(weekdaysLabel, 10, 65);
    lv_obj_set_align(weekdaysLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(weekdaysLabel, alarm_translation[working_day_label]);
    lv_obj_set_style_text_font(weekdaysLabel, &font_18, LV_PART_MAIN);

    weekdaysRingsInLabel = lv_label_create(alarmPanel);
    lv_obj_set_size(weekdaysRingsInLabel, LV_SIZE_CONTENT,
                    LV_SIZE_CONTENT); /// 1
    lv_obj_set_style_text_font(weekdaysRingsInLabel, &font_14, LV_PART_MAIN);
    lv_obj_align_to(weekdaysRingsInLabel, weekdaysLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    weekdaysButton = lv_btn_create(alarmPanel);
    lv_obj_set_size(weekdaysButton, 70, 41);
    lv_obj_align_to(weekdaysButton, weekdaysLabel, LV_ALIGN_BOTTOM_LEFT, 270, 10);
    lv_obj_set_style_bg_opa(weekdaysButton, 0, 0);
    lv_obj_add_flag(weekdaysButton, LV_OBJ_FLAG_EVENT_BUBBLE);

    weekdaysButtonLabel = lv_label_create(weekdaysButton);
    lv_obj_set_size(weekdaysButtonLabel, LV_SIZE_CONTENT,
                    LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(weekdaysButtonLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(weekdaysButtonLabel, &font_18, LV_PART_MAIN);

    weekdaysSwitch = lv_switch_create(alarmPanel);
    lv_obj_set_size(weekdaysSwitch, 50, 25);
    lv_obj_align_to(weekdaysSwitch, weekdaysButton, LV_ALIGN_BOTTOM_LEFT, 80, 5);
    lv_obj_add_flag(weekdaysSwitch, LV_OBJ_FLAG_EVENT_BUBBLE);

    weekendsLabel = lv_label_create(alarmPanel);
    lv_obj_set_size(weekendsLabel, LV_SIZE_CONTENT,
                    LV_SIZE_CONTENT); /// 1
    lv_obj_align_to(weekendsLabel, weekdaysLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40);
    lv_label_set_text(weekendsLabel, alarm_translation[weekend_day_label]);
    lv_obj_set_style_text_font(weekendsLabel, &font_18, LV_PART_MAIN);

    weekendsRingsInLabel = lv_label_create(alarmPanel);
    lv_obj_set_size(weekendsRingsInLabel, LV_SIZE_CONTENT,
                    LV_SIZE_CONTENT); /// 1
    lv_obj_set_style_text_font(weekendsRingsInLabel, &font_14, LV_PART_MAIN);
    lv_obj_align_to(weekendsRingsInLabel, weekendsLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    weekendsButton = lv_btn_create(alarmPanel);
    lv_obj_set_size(weekendsButton, 70, 41);
    lv_obj_align_to(weekendsButton, weekdaysButton, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_style_bg_opa(weekendsButton, 0, 0);
    lv_obj_set_style_text_font(weekendsButton, &font_18, LV_PART_MAIN);
    lv_obj_add_flag(weekendsButton, LV_OBJ_FLAG_EVENT_BUBBLE);

    weekendsButtonLabel = lv_label_create(weekendsButton);
    lv_obj_set_size(weekendsButtonLabel, LV_SIZE_CONTENT,
                    LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(weekendsButtonLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(weekendsButtonLabel, &font_18, LV_PART_MAIN);

    weekendsSwitch = lv_switch_create(alarmPanel);
    lv_obj_set_size(weekendsSwitch, 50, 25);
    lv_obj_align_to(weekendsSwitch, weekdaysSwitch, LV_ALIGN_OUT_BOTTOM_MID, 0, 35);
    lv_obj_add_flag(weekendsSwitch, LV_OBJ_FLAG_EVENT_BUBBLE);

    oneOffLabel = lv_label_create(alarmPanel);
    lv_obj_set_size(oneOffLabel, LV_SIZE_CONTENT,
                    LV_SIZE_CONTENT); /// 1
    lv_obj_align_to(oneOffLabel, weekendsLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40);
    lv_label_set_text(oneOffLabel, alarm_translation[oneoff_day_label]);
    lv_obj_set_style_text_font(oneOffLabel, &font_18, LV_PART_MAIN);

    oneOffRingsInLabel = lv_label_create(alarmPanel);
    lv_obj_set_size(oneOffRingsInLabel, LV_SIZE_CONTENT,
                    LV_SIZE_CONTENT); /// 1
    lv_obj_set_style_text_font(oneOffRingsInLabel, &font_14, LV_PART_MAIN);
    lv_obj_align_to(oneOffRingsInLabel, oneOffLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    oneOffButton = lv_btn_create(alarmPanel);
    lv_obj_set_size(oneOffButton, 70, 41);
    lv_obj_align_to(oneOffButton, weekendsButton, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_style_bg_opa(oneOffButton, 0, 0);
    lv_obj_add_flag(oneOffButton, LV_OBJ_FLAG_EVENT_BUBBLE);

    oneOffButtonLabel = lv_label_create(oneOffButton);
    lv_obj_set_size(oneOffButtonLabel, LV_SIZE_CONTENT,
                    LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(oneOffButtonLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(oneOffButtonLabel, &font_18, LV_PART_MAIN);

    oneOffSwitch = lv_switch_create(alarmPanel);
    lv_obj_set_size(oneOffSwitch, 50, 25);
    lv_obj_align_to(oneOffSwitch, weekendsSwitch, LV_ALIGN_OUT_BOTTOM_MID, 0, 35);
    lv_obj_add_flag(oneOffSwitch, LV_OBJ_FLAG_EVENT_BUBBLE);

    this->set_default_values();

    lv_obj_add_event_cb(weekdaysButton, event_alarmButtons_cb_wrapper, LV_EVENT_CLICKED,
                        NULL);
    lv_obj_add_event_cb(weekendsButton, event_alarmButtons_cb_wrapper, LV_EVENT_CLICKED,
                        NULL);
    lv_obj_add_event_cb(oneOffButton, event_alarmButtons_cb_wrapper, LV_EVENT_CLICKED,
                        NULL);

    lv_obj_add_event_cb(weekdaysSwitch, event_alarmSwitch_cb_wrapper,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(weekendsSwitch, event_alarmSwitch_cb_wrapper,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(oneOffSwitch, event_alarmSwitch_cb_wrapper,
                        LV_EVENT_VALUE_CHANGED, NULL);
}

void AlarmClock::set_roller_time(const lv_obj_t *label) {
    lv_roller_set_selected(hourRoller, parse_alarm_label(lv_label_get_text(label), true),
                           LV_ANIM_OFF);
    lv_roller_set_selected(
        minuteRoller, parse_alarm_label(lv_label_get_text(label), false), LV_ANIM_OFF);
}

void AlarmClock::create_roller_modal_panel(lv_obj_t *target_label) {
    if (alarmModalPanel == NULL) {

        alarmDummyPanel = lv_obj_create(alarmScreen);
        lv_obj_set_size(alarmDummyPanel, 480, 320);
        lv_obj_set_align(alarmDummyPanel, LV_ALIGN_CENTER);
        lv_obj_set_style_bg_opa(alarmDummyPanel, 200, 0);
        lv_obj_add_flag(alarmDummyPanel, LV_OBJ_FLAG_EVENT_BUBBLE);

        alarmModalPanel = lv_obj_create(alarmDummyPanel);
        lv_obj_set_size(alarmModalPanel, 250, 230);
        lv_obj_set_align(alarmModalPanel, LV_ALIGN_CENTER);
        lv_obj_clear_flag(alarmModalPanel,
                          LV_OBJ_FLAG_SCROLLABLE); /// Flags
        lv_obj_add_flag(alarmModalPanel, LV_OBJ_FLAG_EVENT_BUBBLE);

        hourRoller = lv_roller_create(alarmModalPanel);
        lv_roller_set_options(hourRoller, this->_hour_count, LV_ROLLER_MODE_INFINITE);
        lv_obj_set_size(hourRoller, 90, 110);
        lv_obj_set_pos(hourRoller, -50, -35);
        lv_obj_set_align(hourRoller, LV_ALIGN_CENTER);
        lv_obj_set_style_text_font(hourRoller, &font_18, LV_PART_MAIN);
        lv_obj_add_flag(hourRoller, LV_OBJ_FLAG_EVENT_BUBBLE);

        minuteRoller = lv_roller_create(alarmModalPanel);
        lv_roller_set_options(minuteRoller, this->_minute_count, LV_ROLLER_MODE_INFINITE);
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
        lv_obj_set_size(modalCancelButtonLabel, LV_SIZE_CONTENT,
                        LV_SIZE_CONTENT); /// 1
        lv_obj_set_align(modalCancelButtonLabel, LV_ALIGN_CENTER);
        lv_label_set_text(modalCancelButtonLabel, alarm_translation[cancel_button]);
        lv_obj_set_style_text_font(modalCancelButtonLabel, &font_18, LV_PART_MAIN);

        modalOkButton = lv_btn_create(alarmModalPanel);
        lv_obj_set_size(modalOkButton, 90, 45);
        lv_obj_align_to(modalOkButton, modalCancelButton, LV_ALIGN_BOTTOM_LEFT, 85, 10);
        lv_obj_add_flag(modalOkButton, LV_OBJ_FLAG_EVENT_BUBBLE);

        modalOkButtonLabel = lv_label_create(modalOkButton);
        lv_obj_set_size(modalOkButtonLabel, LV_SIZE_CONTENT,
                        LV_SIZE_CONTENT); /// 1
        lv_obj_set_align(modalOkButtonLabel, LV_ALIGN_CENTER);
        lv_label_set_text(modalOkButtonLabel, alarm_translation[ok_button]);
        lv_obj_set_style_text_font(modalOkButtonLabel, &font_18, LV_PART_MAIN);

        lv_obj_add_event_cb(modalCancelButton, event_alarmModalCancelButton_cb_wrapper,
                            LV_EVENT_ALL, NULL);
        lv_obj_add_event_cb(modalOkButton, event_alarmModalOkButton_cb_wrapper,
                            LV_EVENT_ALL, target_label);
        this->target_label = target_label;
        this->set_roller_time(target_label);
    }
}

void AlarmClock::create_roller_data() {
    char hour_buffer[4] = {0};
    char minute_buffer[3] = {0};
    for (short int i = 0; i < HOUR_COUNT; i++) {
        if (i == HOUR_COUNT - 1) {
            sprintf(hour_buffer, "%2i", i);
        } else {
            sprintf(hour_buffer, "%02i\n", i);
        }
        strcat(this->_hour_count, hour_buffer);
    }
    for (short int i = 0; i < MINUTE_COUNT; i++) {
        if (i == MINUTE_COUNT - 1) {
            sprintf(minute_buffer, "%02i", i);
        } else {
            sprintf(minute_buffer, "%02i\n", i);
        }
        strcat(this->_minute_count, minute_buffer);
    }
}

void AlarmClock::delete_roller_modal_panel() {
    if (alarmModalPanel != NULL) {
        lv_obj_remove_event_cb(modalCancelButton,
                               event_alarmModalCancelButton_cb_wrapper);
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

void AlarmClock::create_alarm_modal_panel(lv_obj_t *target_label) {
    alarmDummyPanel = lv_obj_create(lv_scr_act());
    lv_obj_set_size(alarmDummyPanel, 480, 320);
    lv_obj_set_align(alarmDummyPanel, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(alarmDummyPanel, 200, 0);

    alarmModalPanel = lv_obj_create(alarmDummyPanel);
    lv_obj_set_size(alarmModalPanel, 240, 160);
    lv_obj_set_align(alarmModalPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(alarmModalPanel, LV_OBJ_FLAG_SCROLLABLE);

    alarmTimeLabel = lv_label_create(alarmModalPanel);
    lv_label_set_text(alarmTimeLabel, lv_label_get_text(target_label));
    lv_obj_set_align(alarmTimeLabel, LV_ALIGN_CENTER);
    lv_obj_set_pos(alarmTimeLabel, 0, -30);
    lv_obj_set_style_text_font(alarmTimeLabel, &font_32, LV_PART_MAIN);

    modalOkButton = lv_btn_create(alarmModalPanel);
    lv_obj_set_size(modalOkButton, 80, 45);
    lv_obj_set_align(modalOkButton, LV_ALIGN_CENTER);
    lv_obj_set_pos(modalOkButton, 0, 40);

    modalOkButtonLabel = lv_label_create(modalOkButton);
    lv_obj_set_align(modalOkButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(modalOkButtonLabel, alarm_translation[ok_button]);
    lv_obj_set_style_text_font(modalOkButtonLabel, &font_18, LV_PART_MAIN);

    lv_obj_add_event_cb(modalOkButton, event_offAlarmButton_cb_wrapper, LV_EVENT_ALL,
                        NULL);
}

void AlarmClock::delete_alarm_modal_panel() {
    if (alarmModalPanel != NULL) {
        lv_obj_remove_event_cb(modalOkButton, event_offAlarmButton_cb_wrapper);
        lv_obj_del(alarmDummyPanel);
        alarmDummyPanel = NULL;
        alarmModalPanel = NULL;
        modalOkButton = NULL;
        modalOkButtonLabel = NULL;
    }
}

bool AlarmClock::is_weekends(int week_day) {
    if (week_day == 0 | week_day == 6) {
        return true;
    }
    return false;
}

void AlarmClock::check_alarm_clocks(struct tm &timeinfo) {
    this->check_weekends_alarm_clock(timeinfo);
    this->check_weekdays_alarm_clock(timeinfo);
    this->check_oneOff_alarm_clock(timeinfo);
}

void AlarmClock::check_weekends_alarm_clock(tm &timeinfo) {
    if (lv_obj_has_state(weekendsSwitch, LV_STATE_CHECKED)) {
        int hour_from_label =
            parse_alarm_label(lv_label_get_text(weekendsButtonLabel), true);
        int minute_from_label =
            parse_alarm_label(lv_label_get_text(weekendsButtonLabel), false);
        if (this->is_weekends(timeinfo.tm_wday)) {

            if (timeinfo.tm_hour == hour_from_label &
                timeinfo.tm_min == minute_from_label) {
                if (!this->weekends_already_fired) {
                    this->fire_alarm(weekendsButtonLabel);
                    this->weekends_already_fired = true;
                }
            } else {
                this->weekends_already_fired = false;
            }
        }
        this->calculate_weekends_remaining_time(hour_from_label, minute_from_label,
                                                timeinfo);
    } else {
        lv_label_set_text(weekendsRingsInLabel, "");
    }
}

void AlarmClock::check_weekdays_alarm_clock(tm &timeinfo) {
    if (lv_obj_has_state(weekdaysSwitch, LV_STATE_CHECKED)) {
        int hour_from_label =
            parse_alarm_label(lv_label_get_text(weekdaysButtonLabel), true);
        int minute_from_label =
            parse_alarm_label(lv_label_get_text(weekdaysButtonLabel), false);
        if (!this->is_weekends(timeinfo.tm_wday)) {

            if (timeinfo.tm_hour == hour_from_label &
                timeinfo.tm_min == minute_from_label) {
                if (!this->weekdays_already_fired) {
                    this->fire_alarm(weekdaysButtonLabel);
                    this->weekdays_already_fired = true;
                }
            } else {
                this->weekdays_already_fired = false;
            }
        }
        this->calculate_weekdays_remaining_time(hour_from_label, minute_from_label,
                                                timeinfo);
    } else {
        lv_label_set_text(weekdaysRingsInLabel, "");
    }
}

void AlarmClock::check_oneOff_alarm_clock(tm &timeinfo) {
    if (lv_obj_has_state(oneOffSwitch, LV_STATE_CHECKED)) {
        int hour_from_label =
            parse_alarm_label(lv_label_get_text(oneOffButtonLabel), true);
        int minute_from_label =
            parse_alarm_label(lv_label_get_text(oneOffButtonLabel), false);
        if (timeinfo.tm_hour == hour_from_label & timeinfo.tm_min == minute_from_label) {
            this->fire_alarm(oneOffButtonLabel);
            lv_obj_clear_state(oneOffSwitch, LV_STATE_CHECKED);
        } else {
            this->calculate_oneOff_remaining_time(hour_from_label, minute_from_label,
                                                  timeinfo);
        }
    } else {
        lv_label_set_text(oneOffRingsInLabel, "");
    }
}

void AlarmClock::fire_alarm(lv_obj_t *target_label) {
    this->delete_roller_modal_panel();
    this->create_alarm_modal_panel(target_label);
}

void AlarmClock::calculate_oneOff_remaining_time(int hour, int minute,
                                                 struct tm &timeinfo) {
    struct tm timeinfo_local;
    this->copy_timeinfo_struct(timeinfo_local, timeinfo);
    time_t now = mktime(&timeinfo_local);
    if (timeinfo_local.tm_hour > hour) {
        timeinfo_local.tm_mday += 1;
    }
    timeinfo_local.tm_hour = hour;
    timeinfo_local.tm_min = minute;
    time_t next_time = mktime(&timeinfo_local);
    double difference = difftime(next_time, now);
    this->set_rings_in_label_text(difference, oneOffRingsInLabel);
}

void AlarmClock::calculate_weekends_remaining_time(int hour, int minute,
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
    this->set_rings_in_label_text(difference, weekendsRingsInLabel);
}

void AlarmClock::calculate_weekdays_remaining_time(int hour, int minute,
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
    this->set_rings_in_label_text(difference, weekdaysRingsInLabel);
}

void AlarmClock::set_rings_in_label_text(double &difference_in_seconds,
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

void AlarmClock::set_default_values() {
    lv_label_set_text(weekdaysRingsInLabel, "");
    lv_label_set_text(weekdaysButtonLabel, "08:00");
    lv_label_set_text(weekendsRingsInLabel, "");
    lv_label_set_text(weekendsButtonLabel, "11:00");
    lv_label_set_text(oneOffRingsInLabel, "");
    lv_label_set_text(oneOffButtonLabel, "18:36");
}

void AlarmClock::set_alarm_switches(bool weekdays_sw, bool weekends_sw, bool oneOff_sw) {
    lv_obj_add_state(this->weekdaysSwitch,
                     weekdays_sw ? LV_STATE_CHECKED : LV_STATE_DEFAULT);
    lv_obj_add_state(this->weekendsSwitch,
                     weekends_sw ? LV_STATE_CHECKED : LV_STATE_DEFAULT);
    lv_obj_add_state(this->oneOffSwitch, oneOff_sw ? LV_STATE_CHECKED : LV_STATE_DEFAULT);
}

void AlarmClock::set_alarm_buttons(const char *weekdays_time, const char *weekends_time,
                                   const char *oneOff_time) {
    lv_label_set_text(weekdaysButtonLabel, weekdays_time);
    lv_label_set_text(weekendsButtonLabel, weekends_time);
    lv_label_set_text(oneOffButtonLabel, oneOff_time);
}

void AlarmClock::event_alarmModalCancelButton_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED) {
        this->delete_roller_modal_panel();
    }
}

void AlarmClock::event_alarmModalOkButton_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED) {
        lv_label_set_text_fmt(this->target_label, "%02i:%02i",
                              lv_roller_get_selected(hourRoller),
                              lv_roller_get_selected(minuteRoller));
        this->delete_roller_modal_panel();
        _preferences.begin(NAMESPACE);
        _preferences.putString("weekdays_time",
                               lv_label_get_text(this->weekdaysButtonLabel));
        _preferences.putString("weekends_time",
                               lv_label_get_text(this->weekendsButtonLabel));
        _preferences.putString("oneOff_time", lv_label_get_text(this->oneOffButtonLabel));
        _preferences.end();
    }
}

void AlarmClock::event_offAlarmButton_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED) {
        this->delete_alarm_modal_panel();
    }
}

void AlarmClock::event_alarmButtons_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (target == weekdaysButton) {
        target_label = weekdaysButtonLabel;
    } else if (target == weekendsButton) {
        target_label = weekendsButtonLabel;
    } else if (target == oneOffButton) {
        target_label = oneOffButtonLabel;
    }
    this->create_roller_modal_panel(target_label);
}

void AlarmClock::event_alarmSwitch_cb(lv_event_t *e) {
    _preferences.begin(NAMESPACE);
    _preferences.putBool("weekdays_sw",
                         lv_obj_has_state(this->weekdaysSwitch, LV_STATE_CHECKED));
    _preferences.putBool("weekends_sw",
                         lv_obj_has_state(this->weekendsSwitch, LV_STATE_CHECKED));
    _preferences.putBool("oneOff_sw",
                         lv_obj_has_state(this->oneOffSwitch, LV_STATE_CHECKED));
    _preferences.end();
}

int AlarmClock::parse_alarm_label(char *string, bool hour) {
    char alarm_buff[2];
    for (size_t i = 0; i < 2; i++) {
        if (hour) {
            alarm_buff[i] = string[i + 0];
        } else {
            alarm_buff[i] = string[i + 3];
        }
    }
    return atoi(alarm_buff);
}
void AlarmClock::set_preferences(Preferences &preferences) { _preferences = preferences; }
void AlarmClock::copy_timeinfo_struct(tm &new_tm, tm &old_tm) {
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

AlarmClock::~AlarmClock() {}