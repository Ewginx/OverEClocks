#include "GUI/Alarm.h"

static Alarm *instance;

extern "C" void event_alarmModalCancelButton_cb_wrapper(lv_event_t *e) {
  instance->event_alarmModalCancelButton_cb(e);
}
extern "C" void event_alarmModalOkButton_cb_wrapper(lv_event_t *e) {
  instance->event_alarmModalOkButton_cb(e);
}
extern "C" void event_workingDayButton_cb_wrapper(lv_event_t *e) {
  instance->event_workingDayButton_cb(e);
} 
 extern "C" void event_weekendButton_cb_wrapper(lv_event_t *e) {
  instance->event_weekendButton_cb(e);
}
extern "C" void event_oneOffButton_cb_wrapper(lv_event_t *e) {
  instance->event_oneOffButton_cb(e);
}    

void Alarm::ui_set_roller_time(const lv_obj_t *label)
{
    char alarm_buff[2];
    parse_alarm_label(lv_label_get_text(label), hour_label, alarm_buff);
    lv_roller_set_selected(hourRoller, atoi(alarm_buff), LV_ANIM_OFF);
    parse_alarm_label(lv_label_get_text(label), minute_label, alarm_buff);
    lv_roller_set_selected(minuteRoller, atoi(alarm_buff), LV_ANIM_OFF);
}

void Alarm::create_alarm_modal_panel(lv_obj_t *target_label)
{
    if (alarmModalPanel == NULL)
    {
        char hour_count[hour * 3] = {0};
        char hour_buffer[4] = {0};
        char minute_count[minute * 3] = {0};
        char minute_buffer[3] = {0};
        alarmDummyPanel = lv_obj_create(alarmScreen);
        lv_obj_set_size(alarmDummyPanel, 480, 320);
        lv_obj_set_align(alarmDummyPanel, LV_ALIGN_CENTER);
        lv_obj_set_style_bg_opa(alarmDummyPanel, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
        alarmModalPanel = lv_obj_create(alarmDummyPanel);
        lv_obj_set_size(alarmModalPanel, 250, 230);
        lv_obj_set_align(alarmModalPanel, LV_ALIGN_CENTER);
        lv_obj_clear_flag(alarmModalPanel, LV_OBJ_FLAG_SCROLLABLE); /// Flags
        for (short int i = 0; i < hour; i++)
        {
            if (i == hour - 1)
            {
                sprintf(hour_buffer, "%2i", i);
            }
            else
            {
                sprintf(hour_buffer, "%02i\n", i);
            }
            strcat(hour_count, hour_buffer);
        }
        for (short int i = 0; i < minute; i++)
        {
            if (i == minute - 1)
            {
                sprintf(minute_buffer, "%02i", i);
            }
            else
            {
                sprintf(minute_buffer, "%02i\n", i);
            }
            strcat(minute_count, minute_buffer);
        }
        hourRoller = lv_roller_create(alarmModalPanel);
        lv_roller_set_options(hourRoller,
                              hour_count, LV_ROLLER_MODE_INFINITE);
        lv_obj_set_size(hourRoller, 90, 110);
        lv_obj_set_pos(hourRoller, -50, -35);
        lv_obj_set_align(hourRoller, LV_ALIGN_CENTER);
        lv_obj_set_style_text_font(hourRoller, &montserrat_18, LV_PART_MAIN);

        minuteRoller = lv_roller_create(alarmModalPanel);
        lv_roller_set_options(minuteRoller,
                              minute_count,
                              LV_ROLLER_MODE_INFINITE);
        lv_obj_set_size(minuteRoller, 90, 110);
        lv_obj_align_to(minuteRoller, hourRoller, LV_ALIGN_BOTTOM_LEFT, 83, 18);
        lv_obj_set_style_text_font(minuteRoller, &montserrat_18, LV_PART_MAIN);

        modalCancelButton = lv_btn_create(alarmModalPanel);
        lv_obj_set_size(modalCancelButton, 90, 45);
        lv_obj_set_pos(modalCancelButton, 13, 0);
        lv_obj_set_align(modalCancelButton, LV_ALIGN_BOTTOM_LEFT);

        modalCancelButtonLabel = lv_label_create(modalCancelButton);
        lv_obj_set_size(modalCancelButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
        lv_obj_set_align(modalCancelButtonLabel, LV_ALIGN_CENTER);
        lv_label_set_text(modalCancelButtonLabel, alarm_translation[CANCEL_BUTTON]);
        lv_obj_set_style_text_font(modalCancelButtonLabel, &montserrat_18, LV_PART_MAIN);

        modalOkButton = lv_btn_create(alarmModalPanel);
        lv_obj_set_size(modalOkButton, 90, 45);
        lv_obj_align_to(modalOkButton, modalCancelButton, LV_ALIGN_BOTTOM_LEFT, 85, 10);

        modalOkButtonLabel = lv_label_create(modalOkButton);
        lv_obj_set_size(modalOkButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
        lv_obj_set_align(modalOkButtonLabel, LV_ALIGN_CENTER);
        lv_label_set_text(modalOkButtonLabel, alarm_translation[OK_BUTTON]);
        lv_obj_set_style_text_font(modalOkButtonLabel, &montserrat_18, LV_PART_MAIN);

        lv_obj_add_event_cb(modalCancelButton, event_alarmModalCancelButton_cb_wrapper, LV_EVENT_ALL, NULL);
        lv_obj_add_event_cb(modalOkButton, event_alarmModalOkButton_cb_wrapper, LV_EVENT_ALL, target_label);
        this->target_label = target_label;
        this->ui_set_roller_time(target_label);
    }
}

void Alarm::delete_alarm_modal_panel()
{
    if (alarmModalPanel != NULL)
    {
        lv_obj_remove_event_cb(modalCancelButton, event_alarmModalCancelButton_cb_wrapper);
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

Alarm::Alarm(/* args */)
{
    instance = this;
    alarmModalPanel = NULL;
    alarmScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(alarmScreen, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    alarmPanel = lv_obj_create(alarmScreen);
    lv_obj_set_size(alarmPanel, lv_pct(100), lv_pct(100));
    lv_obj_set_pos(alarmPanel, 1, -1);
    lv_obj_set_align(alarmPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(alarmPanel, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(alarmPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(alarmPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    workingDayLabel = lv_label_create(alarmPanel);
    lv_obj_set_size(workingDayLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(workingDayLabel, 10, 65);
    lv_obj_set_align(workingDayLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(workingDayLabel, alarm_translation[WORKING_DAY_LABEL]);
    lv_obj_set_style_text_font(workingDayLabel, &montserrat_18, LV_PART_MAIN);

    workingDayButton = lv_btn_create(alarmPanel);
    lv_obj_set_size(workingDayButton, 70, 41);
    lv_obj_align_to(workingDayButton, workingDayLabel, LV_ALIGN_BOTTOM_LEFT, 270, 10);
    lv_obj_set_style_bg_opa(workingDayButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    workingDayButtonLabel = lv_label_create(workingDayButton);
    lv_obj_set_size(workingDayButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(workingDayButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(workingDayButtonLabel, "08:15");
    lv_obj_set_style_text_font(workingDayButtonLabel, &montserrat_18, LV_PART_MAIN);


    workingDaySwitch = lv_switch_create(alarmPanel);
    lv_obj_set_size(workingDaySwitch, 50, 25);
    lv_obj_align_to(workingDaySwitch, workingDayButton, LV_ALIGN_BOTTOM_LEFT, 80, 5);

    weekendDayLabel = lv_label_create(alarmPanel);
    lv_obj_set_size(weekendDayLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_align_to(weekendDayLabel, workingDayLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40);
    lv_label_set_text(weekendDayLabel, alarm_translation[WEEKEND_DAY_LABEL]);
    lv_obj_set_style_text_font(weekendDayLabel, &montserrat_18, LV_PART_MAIN);

    weekendDayButton = lv_btn_create(alarmPanel);
    lv_obj_set_size(weekendDayButton, 70, 41);
    lv_obj_align_to(weekendDayButton, workingDayButton, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_style_bg_opa(weekendDayButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(weekendDayButton, &montserrat_18, LV_PART_MAIN);

    weekendDayButtonLabel = lv_label_create(weekendDayButton);
    lv_obj_set_size(weekendDayButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(weekendDayButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(weekendDayButtonLabel, "15:23");
    lv_obj_set_style_text_font(weekendDayButtonLabel, &montserrat_18, LV_PART_MAIN);

    weekendDaySwitch = lv_switch_create(alarmPanel);
    lv_obj_set_size(weekendDaySwitch, 50, 25);
    lv_obj_align_to(weekendDaySwitch, workingDaySwitch, LV_ALIGN_OUT_BOTTOM_MID, 0, 35);

    oneOffLabel = lv_label_create(alarmPanel);
    lv_obj_set_size(oneOffLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_align_to(oneOffLabel, weekendDayLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40);
    lv_label_set_text(oneOffLabel, alarm_translation[ONEOFF_DAY_LABEL]);
    lv_obj_set_style_text_font(oneOffLabel, &montserrat_18, LV_PART_MAIN);

    oneOffButton = lv_btn_create(alarmPanel);
    lv_obj_set_size(oneOffButton, 70, 41);
    lv_obj_align_to(oneOffButton, weekendDayButton, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_style_bg_opa(oneOffButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    oneOffButtonLabel = lv_label_create(oneOffButton);
    lv_obj_set_size(oneOffButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(oneOffButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(oneOffButtonLabel, "12:03");
    lv_obj_set_style_text_font(oneOffButtonLabel, &montserrat_18, LV_PART_MAIN);

    oneOffSwitch = lv_switch_create(alarmPanel);
    lv_obj_set_size(oneOffSwitch, 50, 25);
    lv_obj_align_to(oneOffSwitch, weekendDaySwitch, LV_ALIGN_OUT_BOTTOM_MID, 0, 35);


    lv_obj_add_event_cb(workingDayButton, event_workingDayButton_cb_wrapper, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(weekendDayButton, event_weekendButton_cb_wrapper, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(oneOffButton, event_oneOffButton_cb_wrapper, LV_EVENT_ALL, NULL);

}

void Alarm::event_alarmModalCancelButton_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        this->delete_alarm_modal_panel();
    }
}

void Alarm::event_alarmModalOkButton_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        lv_label_set_text_fmt(this->target_label, "%02i:%02i", lv_roller_get_selected(hourRoller), lv_roller_get_selected(minuteRoller));
        this->delete_alarm_modal_panel();
    }
}

void Alarm::event_workingDayButton_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        this->create_alarm_modal_panel(workingDayButtonLabel);
    }
}

void Alarm::event_weekendButton_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        this->create_alarm_modal_panel(weekendDayButtonLabel);
    }
}

void Alarm::event_oneOffButton_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
       this->create_alarm_modal_panel(oneOffButtonLabel);
    }
}

void Alarm::parse_alarm_label(char *string, int pos, char *buff)
{
    for (size_t i = 0; i < 2; i++)
    {
        buff[i] = string[i + pos];
    }
}

Alarm::~Alarm()
{
}