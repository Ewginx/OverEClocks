#pragma once
#include "Translation/Translation.h"
#include <lvgl.h>


class DigitalClock {

  public:
    lv_obj_t *digitalClockScreen;
    lv_obj_t *digitalClockPanel;
    lv_obj_t *clockLabel;
    lv_obj_t *secondsLabel;
    lv_obj_t *dateLabel;

    void set_time(int hours, int minutes, int seconds);
    void set_date(int month_day, int month, int year, int day);
    void set_default_values();

    DigitalClock();
    ~DigitalClock();
};