#include "GuiApp.h"

static GuiApp *instance = NULL;

extern "C" void swipe_screen_event_cb_wrapper(lv_event_t *e) {
  instance->swipe_screen_event_cb(e);
}

extern "C" void screen_load_event_cb_wrapper(lv_event_t *e) {
  instance->screen_load_event_cb(e);
}


GuiApp::GuiApp(/* args */)
{   
    instance = this;
    alarm_screen = new Alarm();
    digital_clock_screen = new DigitalClock();
    analog_clock_screen = new AnalogClock();
    weather_screen = new Weather();
    dock_panel = new DockPanel(digital_clock_screen->digitalClockPanel);
    lv_obj_add_event_cb(digital_clock_screen->digitalClockScreen, swipe_screen_event_cb_wrapper, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(weather_screen->weatherScreen, swipe_screen_event_cb_wrapper, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(analog_clock_screen->analogClockScreen, swipe_screen_event_cb_wrapper, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(alarm_screen->alarmScreen, swipe_screen_event_cb_wrapper, LV_EVENT_GESTURE, NULL);
    
    lv_obj_add_event_cb(digital_clock_screen->digitalClockScreen, screen_load_event_cb_wrapper, LV_EVENT_SCREEN_LOADED, NULL);
    lv_obj_add_event_cb(weather_screen->weatherScreen, screen_load_event_cb_wrapper, LV_EVENT_SCREEN_LOADED, NULL);
    lv_obj_add_event_cb(analog_clock_screen->analogClockScreen, screen_load_event_cb_wrapper, LV_EVENT_SCREEN_LOADED, NULL);
    lv_obj_add_event_cb(alarm_screen->alarmScreen, screen_load_event_cb_wrapper, LV_EVENT_SCREEN_LOADED, NULL);
    
    
};

void GuiApp::init_gui(){
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_TEAL), lv_palette_main(LV_PALETTE_TEAL),
                                              true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    lv_disp_load_scr(digital_clock_screen->digitalClockScreen);
};

void GuiApp::swipe_screen_event_cb(lv_event_t *e){
    lv_obj_t *target = lv_event_get_target(e);
    if(target == this->digital_clock_screen->digitalClockScreen){
        this->swipe_digital_clock_screen();
    }
    else if (target == this->weather_screen->weatherScreen)
    {
        this->swipe_weather_screen();
    }
    else if (target == this->analog_clock_screen->analogClockScreen)
    {
        this->swipe_analog_clock_screen();
    }
    else if (target == this->alarm_screen->alarmScreen)
    {
        this->swipe_alarm_screen();
    }
    
}

void GuiApp::swipe_digital_clock_screen(){
    if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT)
    {
        lv_scr_load_anim(this->weather_screen->weatherScreen, LV_SCR_LOAD_ANIM_MOVE_LEFT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
    else if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
    {
        lv_scr_load_anim(this->analog_clock_screen->analogClockScreen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
}

void GuiApp::swipe_analog_clock_screen(){
    if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT)
    {
        lv_scr_load_anim(this->digital_clock_screen->digitalClockScreen, LV_SCR_LOAD_ANIM_MOVE_LEFT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
    else if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
    {
        lv_scr_load_anim(this->alarm_screen->alarmScreen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
}
void GuiApp::swipe_weather_screen(){
    if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
    {
        lv_scr_load_anim(this->digital_clock_screen->digitalClockScreen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
}

void GuiApp::swipe_alarm_screen(){
    if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT)
    {
        lv_scr_load_anim(this->analog_clock_screen->analogClockScreen, LV_SCR_LOAD_ANIM_MOVE_LEFT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
}

void  GuiApp::screen_load_event_cb(lv_event_t *e){
    this->dock_panel->change_dock_parent(lv_scr_act());
}


GuiApp::~GuiApp()
{
}