#include <lvgl.h>

class AnalogClockScreen{

public:
    lv_obj_t *ui_AnalogClockScreen;
    lv_obj_t *ui_AnalogClockPanel;
    lv_obj_t *ui_ImageWatchface;
    lv_obj_t *ui_ImageArmHour;
    lv_obj_t *ui_ImageArmMinute;
    lv_obj_t *ui_ImageArmSecond;
    AnalogClockScreen();
    ~AnalogClockScreen();

};