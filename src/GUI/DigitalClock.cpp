#include "GUI/DigitalClock.h"

void DigitalClock::set_time(char *fullTime, char *seconds)
{
    lv_label_set_text(this->clockLabel, fullTime);
    lv_label_set_text(this->secondsLabel, seconds);

}

void DigitalClock::set_date(char *date, int day)
{
    lv_label_set_text(this->dayLabel, digital_clock_translation[day]);
    lv_label_set_text(this->dateLabel, date);
}

DigitalClock::DigitalClock()
{
    // LV_FONT_DECLARE(ui_font_Roboto120);
    // LV_FONT_DECLARE(ui_font_Roboto45);
    digitalClockScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(digitalClockScreen, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    digitalClockPanel = lv_obj_create(digitalClockScreen);
    lv_obj_set_size(digitalClockPanel, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(digitalClockPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(digitalClockPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_align(digitalClockPanel, LV_ALIGN_TOP_LEFT);
    lv_obj_clear_flag(digitalClockPanel, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    clockLabel = lv_label_create(digitalClockPanel);
    lv_obj_set_size(clockLabel, 320, 100);
    lv_obj_set_pos(clockLabel, 0, 0);
    lv_obj_set_align(clockLabel, LV_ALIGN_CENTER);
    lv_label_set_text(clockLabel, "00:00");

    lv_obj_set_style_text_align(clockLabel, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(clockLabel, &font_Roboto120, LV_PART_MAIN | LV_STATE_DEFAULT);

    secondsLabel = lv_label_create(digitalClockPanel);
    lv_obj_set_size(secondsLabel, 60, 40);
    lv_obj_align_to(secondsLabel, clockLabel, LV_ALIGN_BOTTOM_RIGHT, 50, -3);
    lv_label_set_text(secondsLabel, "00");
    lv_obj_set_style_text_align(secondsLabel, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(secondsLabel, &font_Roboto45, LV_PART_MAIN | LV_STATE_DEFAULT);

    dateLabel = lv_label_create(digitalClockPanel);
    lv_obj_set_size(dateLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(dateLabel, 0, 95);
    lv_obj_set_align(dateLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_align(dateLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(dateLabel, &montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);

    dayLabel = lv_label_create(digitalClockPanel);
    lv_obj_set_size(dayLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_align_to(dayLabel,dateLabel, LV_ALIGN_OUT_RIGHT_MID, 50,-10);
    lv_obj_set_style_text_align(dayLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(dayLabel, &montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);

};

DigitalClock::~DigitalClock()
{
    
};
