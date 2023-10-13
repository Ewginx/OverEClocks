#include <lvgl.h>
class DigitalClockScreen{

public:
    lv_obj_t *ui_DigitalClockScreen;
    lv_obj_t *ui_DigitalClockPanel;
    lv_obj_t *ui_DigitalClockLabel;
    lv_obj_t *ui_DigitalClockSecondLabel;
    lv_obj_t *ui_DigitalClockDateLabel;
    

    void set_time(char *fullTime, char *seconds);
    void set_date(char *date);

    DigitalClockScreen();
    ~DigitalClockScreen();

    // void swap_screen_event_cb(lv_event_t *e);

};