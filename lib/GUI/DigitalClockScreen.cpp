#include "DigitalClockScreen.h"

void DigitalClockScreen::set_time(char *fullTime, char *seconds)
{
    lv_label_set_text(this->ui_DigitalClockLabel, fullTime);
    lv_label_set_text(this->ui_DigitalClockSecondLabel, seconds);

}

void DigitalClockScreen::set_date(char *date)
{
    lv_label_set_text(this->ui_DigitalClockDateLabel, date);
}

DigitalClockScreen::DigitalClockScreen()
{
    // LV_FONT_DECLARE(ui_font_Roboto120);
    // LV_FONT_DECLARE(ui_font_Roboto45);
    ui_DigitalClockScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_DigitalClockScreen, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_DigitalClockPanel = lv_obj_create(ui_DigitalClockScreen);
    lv_obj_set_size(ui_DigitalClockPanel, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(ui_DigitalClockPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_DigitalClockPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_align(ui_DigitalClockPanel, LV_ALIGN_TOP_LEFT);
    lv_obj_clear_flag(ui_DigitalClockPanel, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_DigitalClockLabel = lv_label_create(ui_DigitalClockPanel);
    lv_obj_set_size(ui_DigitalClockLabel, 320, 100);
    lv_obj_set_pos(ui_DigitalClockLabel, 70, 75);
    lv_obj_set_align(ui_DigitalClockLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(ui_DigitalClockLabel, "00:00");

    lv_obj_set_style_text_align(ui_DigitalClockLabel, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_DigitalClockLabel, &ui_font_Roboto120, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DigitalClockSecondLabel = lv_label_create(ui_DigitalClockPanel);
    lv_obj_set_size(ui_DigitalClockSecondLabel, 60, 40);
    lv_obj_align_to(ui_DigitalClockSecondLabel, ui_DigitalClockLabel, LV_ALIGN_BOTTOM_RIGHT, 50, -3);
    lv_label_set_text(ui_DigitalClockSecondLabel, "00");
    lv_obj_set_style_text_align(ui_DigitalClockSecondLabel, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_DigitalClockSecondLabel, &ui_font_Roboto45, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DigitalClockDateLabel = lv_label_create(ui_DigitalClockPanel);
    lv_obj_set_size(ui_DigitalClockDateLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(ui_DigitalClockDateLabel, 90, 215);
    lv_obj_set_align(ui_DigitalClockDateLabel, LV_ALIGN_TOP_LEFT);
    // lv_label_set_text(ui_DigitalClockDateLabel, "04.05.2023");
    lv_obj_set_style_text_align(ui_DigitalClockDateLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_DigitalClockDateLabel, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);

};

DigitalClockScreen::~DigitalClockScreen()
{
    
};
