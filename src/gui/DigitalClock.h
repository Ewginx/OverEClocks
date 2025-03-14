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

    void setTime(int hours, int minutes);
    void setSeconds(int seconds);
    void setDate(int month_day, int month, int year, int day);
    void setDefaultValues();

    DigitalClock();
    ~DigitalClock();
    private:
    void createDigitalClockWidgets();
};