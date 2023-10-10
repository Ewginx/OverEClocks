#include "AlarmScreen.h"

AlarmScreen *alarm_screen_instance;

extern "C" void event_AlarmModalCancelButton_cb_wrapper(lv_event_t *e) {
  alarm_screen_instance->event_AlarmModalCancelButton_cb(e);
}
extern "C" void event_AlarmModalOkButton_cb_wrapper(lv_event_t *e) {
  alarm_screen_instance->event_AlarmModalOkButton_cb(e);
}
extern "C" void event_WorkingDayButton_cb_wrapper(lv_event_t *e) {
  alarm_screen_instance->event_WorkingDayButton_cb(e);
} 
 extern "C" void event_WeekendButton_cb_wrapper(lv_event_t *e) {
  alarm_screen_instance->event_WeekendButton_cb(e);
}
extern "C" void event_OneOffButton_cb_wrapper(lv_event_t *e) {
  alarm_screen_instance->event_OneOffButton_cb(e);
}    

void AlarmScreen::ui_set_roller_time(const lv_obj_t *label)
{
    char alarm_buff[2];
    parse_alarm_label(lv_label_get_text(label), hour_label, alarm_buff);
    lv_roller_set_selected(ui_AlarmHourRoller, atoi(alarm_buff), LV_ANIM_OFF);
    parse_alarm_label(lv_label_get_text(label), minute_label, alarm_buff);
    lv_roller_set_selected(ui_AlarmMinuteRoller, atoi(alarm_buff), LV_ANIM_OFF);
}

void AlarmScreen::create_alarm_modal_panel(lv_obj_t *target_label)
{
    if (ui_AlarmModalPanel == NULL)
    {
        char hour_count[hour * 3] = {0};
        char hour_buffer[4] = {0};
        char minute_count[minute * 3] = {0};
        char minute_buffer[3] = {0};
        ui_AlarmDummyPanel = lv_obj_create(ui_AlarmScreen);
        lv_obj_set_size(ui_AlarmDummyPanel, 480, 320);
        lv_obj_set_align(ui_AlarmDummyPanel, LV_ALIGN_CENTER);
        lv_obj_set_style_bg_opa(ui_AlarmDummyPanel, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
        ui_AlarmModalPanel = lv_obj_create(ui_AlarmDummyPanel);
        lv_obj_set_size(ui_AlarmModalPanel, 250, 230);
        lv_obj_set_align(ui_AlarmModalPanel, LV_ALIGN_CENTER);
        lv_obj_clear_flag(ui_AlarmModalPanel, LV_OBJ_FLAG_SCROLLABLE); /// Flags
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
        ui_AlarmHourRoller = lv_roller_create(ui_AlarmModalPanel);
        lv_roller_set_options(ui_AlarmHourRoller,
                              hour_count, LV_ROLLER_MODE_INFINITE);
        lv_obj_set_size(ui_AlarmHourRoller, 90, 110);
        lv_obj_set_pos(ui_AlarmHourRoller, -50, -35);
        lv_obj_set_align(ui_AlarmHourRoller, LV_ALIGN_CENTER);
        lv_obj_set_style_text_font(ui_AlarmHourRoller, &lv_font_montserrat_18, LV_PART_MAIN);

        ui_AlarmMinuteRoller = lv_roller_create(ui_AlarmModalPanel);
        lv_roller_set_options(ui_AlarmMinuteRoller,
                              minute_count,
                              LV_ROLLER_MODE_INFINITE);
        lv_obj_set_size(ui_AlarmMinuteRoller, 90, 110);
        lv_obj_align_to(ui_AlarmMinuteRoller, ui_AlarmHourRoller, LV_ALIGN_BOTTOM_LEFT, 83, 18);
        lv_obj_set_style_text_font(ui_AlarmMinuteRoller, &lv_font_montserrat_18, LV_PART_MAIN);

        ui_AlarmModalCancelButton = lv_btn_create(ui_AlarmModalPanel);
        lv_obj_set_size(ui_AlarmModalCancelButton, 90, 45);
        lv_obj_set_pos(ui_AlarmModalCancelButton, 13, 0);
        lv_obj_set_align(ui_AlarmModalCancelButton, LV_ALIGN_BOTTOM_LEFT);

        ui_AlarmModalCancelButtonLabel = lv_label_create(ui_AlarmModalCancelButton);
        lv_obj_set_size(ui_AlarmModalCancelButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
        lv_obj_set_align(ui_AlarmModalCancelButtonLabel, LV_ALIGN_CENTER);
        lv_label_set_text(ui_AlarmModalCancelButtonLabel, "Cancel");
        lv_obj_set_style_text_font(ui_AlarmModalCancelButtonLabel, &lv_font_montserrat_18, LV_PART_MAIN);

        ui_AlarmModalOkButton = lv_btn_create(ui_AlarmModalPanel);
        lv_obj_set_size(ui_AlarmModalOkButton, 90, 45);
        lv_obj_align_to(ui_AlarmModalOkButton, ui_AlarmModalCancelButton, LV_ALIGN_BOTTOM_LEFT, 85, 10);

        ui_AlarmModalOkButtonLabel = lv_label_create(ui_AlarmModalOkButton);
        lv_obj_set_size(ui_AlarmModalOkButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
        lv_obj_set_align(ui_AlarmModalOkButtonLabel, LV_ALIGN_CENTER);
        lv_label_set_text(ui_AlarmModalOkButtonLabel, "OK");
        lv_obj_set_style_text_font(ui_AlarmModalOkButtonLabel, &lv_font_montserrat_18, LV_PART_MAIN);

        lv_obj_add_event_cb(ui_AlarmModalCancelButton, event_AlarmModalCancelButton_cb_wrapper, LV_EVENT_ALL, NULL);
        lv_obj_add_event_cb(ui_AlarmModalOkButton, event_AlarmModalOkButton_cb_wrapper, LV_EVENT_ALL, target_label);
        this->target_label = target_label;
        this->ui_set_roller_time(target_label);
    }
}

void AlarmScreen::delete_alarm_modal_panel()
{
    if (ui_AlarmModalPanel != NULL)
    {
        lv_obj_remove_event_cb(ui_AlarmModalCancelButton, event_AlarmModalCancelButton_cb_wrapper);
        lv_obj_remove_event_cb(ui_AlarmModalOkButton, event_AlarmModalOkButton_cb_wrapper);
        lv_obj_del(ui_AlarmDummyPanel);
        ui_AlarmDummyPanel = NULL;
        ui_AlarmModalPanel = NULL;
        ui_AlarmHourRoller = NULL;
        ui_AlarmMinuteRoller = NULL;
        ui_AlarmModalCancelButton = NULL;
        ui_AlarmModalCancelButtonLabel = NULL;
        ui_AlarmModalOkButton = NULL;
        ui_AlarmModalOkButtonLabel = NULL;
    }
}

AlarmScreen::AlarmScreen(/* args */)
{
    alarm_screen_instance = this;
    ui_AlarmModalPanel = NULL;
    ui_AlarmScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_AlarmScreen, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_AlarmPanel = lv_obj_create(ui_AlarmScreen);
    lv_obj_set_size(ui_AlarmPanel, lv_pct(100), lv_pct(100));
    lv_obj_set_pos(ui_AlarmPanel, 1, -1);
    lv_obj_set_align(ui_AlarmPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_AlarmPanel, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(ui_AlarmPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_AlarmPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_AlarmWorkingDayLabel = lv_label_create(ui_AlarmPanel);
    lv_obj_set_size(ui_AlarmWorkingDayLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(ui_AlarmWorkingDayLabel, 10, 65);
    lv_obj_set_align(ui_AlarmWorkingDayLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(ui_AlarmWorkingDayLabel, "Alarm Clock Working Day");
    lv_obj_set_style_text_font(ui_AlarmWorkingDayLabel, &lv_font_montserrat_18, LV_PART_MAIN);

    ui_AlarmWorkingDayButton = lv_btn_create(ui_AlarmPanel);
    lv_obj_set_size(ui_AlarmWorkingDayButton, 70, 41);
    lv_obj_align_to(ui_AlarmWorkingDayButton, ui_AlarmWorkingDayLabel, LV_ALIGN_BOTTOM_LEFT, 270, 10);
    lv_obj_set_style_bg_opa(ui_AlarmWorkingDayButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_AlarmWorkingDayButtonLabel = lv_label_create(ui_AlarmWorkingDayButton);
    lv_obj_set_size(ui_AlarmWorkingDayButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_AlarmWorkingDayButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_AlarmWorkingDayButtonLabel, "08:15");
    lv_obj_set_style_text_font(ui_AlarmWorkingDayButtonLabel, &lv_font_montserrat_18, LV_PART_MAIN);


    ui_AlarmWorkingDaySwitch = lv_switch_create(ui_AlarmPanel);
    lv_obj_set_size(ui_AlarmWorkingDaySwitch, 50, 25);
    lv_obj_align_to(ui_AlarmWorkingDaySwitch, ui_AlarmWorkingDayButton, LV_ALIGN_BOTTOM_LEFT, 80, 5);

    ui_AlarmWeekendDayLabel = lv_label_create(ui_AlarmPanel);
    lv_obj_set_size(ui_AlarmWeekendDayLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_align_to(ui_AlarmWeekendDayLabel, ui_AlarmWorkingDayLabel, LV_ALIGN_OUT_BOTTOM_MID, -105, 40);
    lv_label_set_text(ui_AlarmWeekendDayLabel, "Alarm Clock Weekends");
    lv_obj_set_style_text_font(ui_AlarmWeekendDayLabel, &lv_font_montserrat_18, LV_PART_MAIN);

    ui_AlarmWeekendDayButton = lv_btn_create(ui_AlarmPanel);
    lv_obj_set_size(ui_AlarmWeekendDayButton, 70, 41);
    lv_obj_align_to(ui_AlarmWeekendDayButton, ui_AlarmWorkingDayButton, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_style_bg_opa(ui_AlarmWeekendDayButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_AlarmWeekendDayButton, &lv_font_montserrat_18, LV_PART_MAIN);

    ui_AlarmWeekendDayButtonLabel = lv_label_create(ui_AlarmWeekendDayButton);
    lv_obj_set_size(ui_AlarmWeekendDayButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_AlarmWeekendDayButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_AlarmWeekendDayButtonLabel, "15:23");
    lv_obj_set_style_text_font(ui_AlarmWeekendDayButtonLabel, &lv_font_montserrat_18, LV_PART_MAIN);

    ui_AlarmWeekendDaySwitch = lv_switch_create(ui_AlarmPanel);
    lv_obj_set_size(ui_AlarmWeekendDaySwitch, 50, 25);
    lv_obj_align_to(ui_AlarmWeekendDaySwitch, ui_AlarmWorkingDaySwitch, LV_ALIGN_OUT_BOTTOM_MID, 0, 35);

    ui_AlarmOneOffLabel = lv_label_create(ui_AlarmPanel);
    lv_obj_set_size(ui_AlarmOneOffLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_align_to(ui_AlarmOneOffLabel, ui_AlarmWeekendDayLabel, LV_ALIGN_OUT_BOTTOM_MID, -90, 40);
    lv_label_set_text(ui_AlarmOneOffLabel, "Alarm Clock One Off");
    lv_obj_set_style_text_font(ui_AlarmOneOffLabel, &lv_font_montserrat_18, LV_PART_MAIN);

    ui_AlarmOneOffButton = lv_btn_create(ui_AlarmPanel);
    lv_obj_set_size(ui_AlarmOneOffButton, 70, 41);
    lv_obj_align_to(ui_AlarmOneOffButton, ui_AlarmWeekendDayButton, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_style_bg_opa(ui_AlarmOneOffButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_AlarmOneOffButtonLabel = lv_label_create(ui_AlarmOneOffButton);
    lv_obj_set_size(ui_AlarmOneOffButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_AlarmOneOffButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_AlarmOneOffButtonLabel, "12:03");
    lv_obj_set_style_text_font(ui_AlarmOneOffButtonLabel, &lv_font_montserrat_18, LV_PART_MAIN);

    ui_AlarmOneOffSwitch = lv_switch_create(ui_AlarmPanel);
    lv_obj_set_size(ui_AlarmOneOffSwitch, 50, 25);
    lv_obj_align_to(ui_AlarmOneOffSwitch, ui_AlarmWeekendDaySwitch, LV_ALIGN_OUT_BOTTOM_MID, 0, 35);


    lv_obj_add_event_cb(ui_AlarmWorkingDayButton, event_WorkingDayButton_cb_wrapper, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_AlarmWeekendDayButton, event_WeekendButton_cb_wrapper, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_AlarmOneOffButton, event_OneOffButton_cb_wrapper, LV_EVENT_ALL, NULL);

}

void AlarmScreen::event_AlarmModalCancelButton_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        this->delete_alarm_modal_panel();
    }
}

void AlarmScreen::event_AlarmModalOkButton_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        lv_label_set_text_fmt(this->target_label, "%02i:%02i", lv_roller_get_selected(ui_AlarmHourRoller), lv_roller_get_selected(ui_AlarmMinuteRoller));
        this->delete_alarm_modal_panel();
    }
}

void AlarmScreen::event_WorkingDayButton_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        this->create_alarm_modal_panel(ui_AlarmWorkingDayButtonLabel);
    }
}

void AlarmScreen::event_WeekendButton_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        this->create_alarm_modal_panel(ui_AlarmWeekendDayButtonLabel);
    }
}

void AlarmScreen::event_OneOffButton_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
       this->create_alarm_modal_panel(ui_AlarmOneOffButtonLabel);
    }
}

void AlarmScreen::parse_alarm_label(char *string, int pos, char *buff)
{
    for (size_t i = 0; i < 2; i++)
    {
        buff[i] = string[i + pos];
    }
}

AlarmScreen::~AlarmScreen()
{
}