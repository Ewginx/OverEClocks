#include "GUI/GuiApp.h"

static GuiApp *instance = NULL;

extern "C" void swipe_screen_event_cb_wrapper(lv_event_t *e) {
  instance->swipe_screen_event_cb(e);
}

extern "C" void screen_load_event_cb_wrapper(lv_event_t *e) {
  instance->screen_load_event_cb(e);
}
extern "C" void settings_button_event_cb_wrapper(lv_event_t *e) {
  instance->settings_button_event_cb(e);
}
extern "C" void darkmode_switch_event_cb_wrapper(lv_event_t *e) {
  instance->darkmode_switch_event_cb(e);
}

GuiApp::GuiApp(/* args */)
{   
    instance = this;
    alarm = new Alarm();
    digital_clock = new DigitalClock();
    analog_clock_screen = new AnalogClock();
    weather = new Weather();
    settings  = new Settings();    

    dock_panel = new DockPanel(digital_clock->digitalClockPanel);


    lv_obj_add_event_cb(digital_clock->digitalClockScreen, swipe_screen_event_cb_wrapper, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(weather->weatherScreen, swipe_screen_event_cb_wrapper, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(analog_clock_screen->analogClockScreen, swipe_screen_event_cb_wrapper, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(alarm->alarmScreen, swipe_screen_event_cb_wrapper, LV_EVENT_GESTURE, NULL);
    
    lv_obj_add_event_cb(digital_clock->digitalClockScreen, screen_load_event_cb_wrapper, LV_EVENT_SCREEN_LOADED, NULL);
    lv_obj_add_event_cb(weather->weatherScreen, screen_load_event_cb_wrapper, LV_EVENT_SCREEN_LOADED, NULL);
    lv_obj_add_event_cb(analog_clock_screen->analogClockScreen, screen_load_event_cb_wrapper, LV_EVENT_SCREEN_LOADED, NULL);
    lv_obj_add_event_cb(alarm->alarmScreen, screen_load_event_cb_wrapper, LV_EVENT_SCREEN_LOADED, NULL);
    
    lv_obj_add_event_cb(dock_panel->settingsButton, settings_button_event_cb_wrapper, LV_EVENT_CLICKED, NULL);

};

void GuiApp::init_gui(){
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_TEAL), lv_palette_main(LV_PALETTE_TEAL),
                                              true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    lv_disp_load_scr(digital_clock->digitalClockScreen);
};

void GuiApp::swipe_screen_event_cb(lv_event_t *e){
    lv_obj_t *target = lv_event_get_target(e);
    if(target == this->digital_clock->digitalClockScreen){
        this->swipe_digital_clock_screen();
    }
    else if (target == this->weather->weatherScreen)
    {
        this->swipe_weather_screen();
    }
    else if (target == this->analog_clock_screen->analogClockScreen)
    {
        this->swipe_analog_clock_screen();
    }
    else if (target == this->alarm->alarmScreen)
    {
        this->swipe_alarm_screen();
    }
    
}

void GuiApp::swipe_digital_clock_screen(){
    if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT)
    {
        lv_scr_load_anim(this->weather->weatherScreen, LV_SCR_LOAD_ANIM_MOVE_LEFT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
    else if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
    {
        lv_scr_load_anim(this->analog_clock_screen->analogClockScreen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
}

void GuiApp::swipe_analog_clock_screen(){
    if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT)
    {
        lv_scr_load_anim(this->digital_clock->digitalClockScreen, LV_SCR_LOAD_ANIM_MOVE_LEFT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
    else if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
    {
        lv_scr_load_anim(this->alarm->alarmScreen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
}
void GuiApp::swipe_weather_screen(){
    if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
    {
        lv_scr_load_anim(this->digital_clock->digitalClockScreen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SCREEN_CHANGE_ANIM_TIME, 0, false);
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

void GuiApp::settings_button_event_cb(lv_event_t *e)
{    
        lv_obj_add_event_cb(settings->ui_DarkmodeSwitch, darkmode_switch_event_cb_wrapper, LV_EVENT_ALL, NULL);

        settings->load_settings_screen(lv_scr_act());
}

void GuiApp::darkmode_switch_event_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_VALUE_CHANGED)
    {
        lv_disp_t *disp = lv_disp_get_default();
        if (lv_obj_has_state(target, LV_STATE_CHECKED))
        {
            lv_theme_t *theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_CYAN), lv_palette_main(LV_PALETTE_NONE),
                                                      false, LV_FONT_DEFAULT);
            lv_disp_set_theme(disp, theme);
            lv_obj_set_style_text_color(dock_panel->settingsButtonLabel, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(dock_panel->settingsButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(alarm->weekdaysButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(alarm->weekendsButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(alarm->oneOffButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(alarm->weekdaysButtonLabel, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(alarm->weekendsButtonLabel, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(alarm->oneOffButtonLabel, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        else
        {
            lv_theme_t *theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_TEAL), lv_palette_main(LV_PALETTE_TEAL),
                                                      true, LV_FONT_DEFAULT);
            lv_disp_set_theme(disp, theme);
            lv_obj_set_style_text_color(dock_panel->settingsButtonLabel, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(alarm->weekdaysButtonLabel, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(alarm->weekendsButtonLabel, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(alarm->oneOffButtonLabel, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}
GuiApp::~GuiApp()
{
}