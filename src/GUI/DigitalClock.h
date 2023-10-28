#pragma once
#include <lvgl.h>
#include "Translation/Translation.h"

class DigitalClock{

public:
    lv_obj_t *digitalClockScreen;
    lv_obj_t *digitalClockPanel;
    lv_obj_t *clockLabel;
    lv_obj_t *secondsLabel;
    lv_obj_t *dateLabel;
    lv_obj_t *dayLabel;
    

    void set_time(char *fullTime, char *seconds);
    void set_date(char *date, int day);

    DigitalClock();
    ~DigitalClock();

    // void swap_digitalClockScreen_event_cb(lv_event_t *e);

};