#include "GUI/DigitalClock.h"

void DigitalClock::set_time(char *fullTime, char *seconds) {
    lv_label_set_text(this->clockLabel, fullTime);
    lv_label_set_text(this->secondsLabel, seconds);
}

void DigitalClock::set_date(char *date, int day) {
    lv_label_set_text_fmt(this->dateLabel, "%s, %s", date,
                          digital_clock_translation[day]);
}

DigitalClock::DigitalClock() {
    digitalClockScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(digitalClockScreen, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    digitalClockPanel = lv_obj_create(digitalClockScreen);
    lv_obj_set_size(digitalClockPanel, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(digitalClockPanel, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(digitalClockPanel, 0, 0);
    lv_obj_set_align(digitalClockPanel, LV_ALIGN_TOP_LEFT);
    lv_obj_clear_flag(digitalClockPanel, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    clockLabel = lv_label_create(digitalClockPanel);
    lv_obj_set_size(clockLabel, 320, 100);
    lv_obj_set_pos(clockLabel, 0, 0);
    lv_obj_set_align(clockLabel, LV_ALIGN_CENTER);
    lv_label_set_text(clockLabel, "--:--");

    lv_obj_set_style_text_align(clockLabel, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_text_font(clockLabel, &d_clock_font_120, 0);

    secondsLabel = lv_label_create(digitalClockPanel);
    lv_obj_set_size(secondsLabel, 60, 40);
    lv_obj_align_to(secondsLabel, clockLabel, LV_ALIGN_BOTTOM_RIGHT, 50, -3);
    lv_label_set_text(secondsLabel, "--");
    lv_obj_set_style_text_align(secondsLabel, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_text_font(secondsLabel, &d_clock_font_45, 0);

    dateLabel = lv_label_create(digitalClockPanel);
    lv_obj_set_pos(dateLabel, 0, 95);
    lv_obj_set_align(dateLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_align(dateLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(dateLabel, &font_32, 0);
    lv_label_set_text(dateLabel, "--.--.----");
};

DigitalClock::~DigitalClock(){

};
