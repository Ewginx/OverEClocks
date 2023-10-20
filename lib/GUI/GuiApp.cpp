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
    alarm_screen = new AlarmScreen();
    digital_clock_screen = new DigitalClockScreen();
    analog_clock_screen = new AnalogClockScreen();
    weather_screen = new WeatherScreen();
    dock_panel = new DockPanel(digital_clock_screen->ui_DigitalClockPanel);
    lv_obj_add_event_cb(digital_clock_screen->ui_DigitalClockScreen, swipe_screen_event_cb_wrapper, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(weather_screen->ui_WeatherScreen, swipe_screen_event_cb_wrapper, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(analog_clock_screen->ui_AnalogClockScreen, swipe_screen_event_cb_wrapper, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(alarm_screen->ui_AlarmScreen, swipe_screen_event_cb_wrapper, LV_EVENT_GESTURE, NULL);
    
    lv_obj_add_event_cb(digital_clock_screen->ui_DigitalClockScreen, screen_load_event_cb_wrapper, LV_EVENT_SCREEN_LOADED, NULL);
    lv_obj_add_event_cb(weather_screen->ui_WeatherScreen, screen_load_event_cb_wrapper, LV_EVENT_SCREEN_LOADED, NULL);
    lv_obj_add_event_cb(analog_clock_screen->ui_AnalogClockScreen, screen_load_event_cb_wrapper, LV_EVENT_SCREEN_LOADED, NULL);
    lv_obj_add_event_cb(alarm_screen->ui_AlarmScreen, screen_load_event_cb_wrapper, LV_EVENT_SCREEN_LOADED, NULL);
    
    
};

void GuiApp::init_gui(){
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_TEAL), lv_palette_main(LV_PALETTE_TEAL),
                                              true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    lv_disp_load_scr(digital_clock_screen->ui_DigitalClockScreen);
};

void GuiApp::swipe_screen_event_cb(lv_event_t *e){
    lv_obj_t *target = lv_event_get_target(e);
    if(target == this->digital_clock_screen->ui_DigitalClockScreen){
        this->swipe_digital_clock_screen();
    }
    else if (target == this->weather_screen->ui_WeatherScreen)
    {
        this->swipe_weather_screen();
    }
    else if (target == this->analog_clock_screen->ui_AnalogClockScreen)
    {
        this->swipe_analog_clock_screen();
    }
    else if (target == this->alarm_screen->ui_AlarmScreen)
    {
        this->swipe_alarm_screen();
    }
    
}

void GuiApp::swipe_digital_clock_screen(){
    if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT)
    {
        lv_scr_load_anim(this->weather_screen->ui_WeatherScreen, LV_SCR_LOAD_ANIM_MOVE_LEFT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
    else if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
    {
        lv_scr_load_anim(this->analog_clock_screen->ui_AnalogClockScreen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
}

void GuiApp::swipe_analog_clock_screen(){
    if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT)
    {
        lv_scr_load_anim(this->digital_clock_screen->ui_DigitalClockScreen, LV_SCR_LOAD_ANIM_MOVE_LEFT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
    else if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
    {
        lv_scr_load_anim(this->alarm_screen->ui_AlarmScreen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
}
void GuiApp::swipe_weather_screen(){
    if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
    {
        lv_scr_load_anim(this->digital_clock_screen->ui_DigitalClockScreen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
}

void GuiApp::swipe_alarm_screen(){
    if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT)
    {
        lv_scr_load_anim(this->analog_clock_screen->ui_AnalogClockScreen, LV_SCR_LOAD_ANIM_MOVE_LEFT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
}

void  GuiApp::screen_load_event_cb(lv_event_t *e){
    this->dock_panel->change_dock_parent(lv_scr_act());
}


GuiApp::~GuiApp()
{
}