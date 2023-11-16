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

    void set_time(char *fullTime, char *seconds);
    void set_date(char *date, int day);

    DigitalClock();
    ~DigitalClock();
};