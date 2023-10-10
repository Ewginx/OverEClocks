#include "AlarmScreen.h"

AlarmScreen::AlarmScreen(/* args */)
{
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
}

AlarmScreen::~AlarmScreen()
{
}