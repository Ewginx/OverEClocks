#include "DigitalClock.h"

void DigitalClock::set_time(int hours, int minutes, int seconds) {
    lv_label_set_text_fmt(this->clockLabel, "%02d:%02d", hours, minutes);
    lv_label_set_text_fmt(this->secondsLabel, "%02d", seconds);
}

void DigitalClock::set_date(int month_day, int month, int year, int day) {
    lv_label_set_text_fmt(this->dateLabel, "%02d.%02d.%d, %s", month_day, month + 1,
                          year + 1900, digital_clock_translation[day]);
}
void DigitalClock::set_default_values() {
    lv_label_set_text(this->clockLabel, "--:--");
    lv_label_set_text(this->secondsLabel, "--");
    lv_label_set_text(this->dateLabel, "--.--.----");
}
DigitalClock::DigitalClock() {
    digitalClockScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(digitalClockScreen, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    digitalClockPanel = lv_obj_create(digitalClockScreen);
    lv_obj_set_size(digitalClockPanel, lv_pct(100), lv_pct(100));
    lv_obj_set_align(digitalClockPanel, LV_ALIGN_TOP_LEFT);
    lv_obj_clear_flag(digitalClockPanel, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_add_flag(digitalClockPanel, LV_OBJ_FLAG_EVENT_BUBBLE);

    clockLabel = lv_label_create(digitalClockPanel);
    lv_obj_set_size(clockLabel, 320, 100);
    lv_obj_set_pos(clockLabel, 0, 0);
    lv_obj_set_align(clockLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_align(clockLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(clockLabel, &d_clock_font_120, 0);
    lv_obj_add_flag(clockLabel, LV_OBJ_FLAG_EVENT_BUBBLE);

    secondsLabel = lv_label_create(digitalClockPanel);
    lv_obj_set_size(secondsLabel, 60, 40);
    lv_obj_align_to(secondsLabel, clockLabel, LV_ALIGN_BOTTOM_RIGHT, 55, -3);
    lv_obj_set_style_text_align(secondsLabel, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_text_font(secondsLabel, &d_clock_font_45, 0);
    lv_obj_add_flag(secondsLabel, LV_OBJ_FLAG_EVENT_BUBBLE);

    dateLabel = lv_label_create(digitalClockPanel);
    lv_obj_set_pos(dateLabel, 0, 95);
    lv_obj_set_align(dateLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_align(dateLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(dateLabel, &font_32, 0);
    lv_obj_add_flag(dateLabel, LV_OBJ_FLAG_EVENT_BUBBLE);

    this->set_default_values();
};

DigitalClock::~DigitalClock(){

};
