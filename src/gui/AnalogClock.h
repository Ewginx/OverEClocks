#pragma once
#include <lvgl.h>

class AnalogClock {

  public:
    lv_obj_t *analogClockScreen;
    lv_obj_t *analogClockPanel;
    lv_obj_t *watchface;
    lv_obj_t *imageArmHour;
    lv_obj_t *imageArmMinute;
    lv_obj_t *imageArmSecond;

    void setTime(int hour, int minute, int second);
    void setDefaultValues();
    void setAnalogClockImages();

    AnalogClock();
    ~AnalogClock();

  private:
    void createAnalogClockWidgets();
};