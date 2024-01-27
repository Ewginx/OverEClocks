#pragma once
#include "Apps/StateApp.h"
#include "Config/Config.h"
#include "GUI/AlarmClock.h"
#include "GUI/AnalogClock.h"
#include "GUI/DigitalClock.h"
#include "GUI/DockPanel.h"
#include "GUI/Settings.h"
#include "GUI/Weather.h"
#include "Theme/Theme.h"


class GuiApp {
  private:
    lv_timer_t *_screen_timer;
    lv_timer_t *_dock_panel_timer;
    /* data */
  public:
    AlarmClock *alarm_clock;
    DigitalClock *digital_clock;
    AnalogClock *analog_clock;
    Weather *weather;
    Settings *settings;
    DockPanel *dock_panel;
    StateApp *state_app;
    lv_obj_t *loading_screen;
    lv_obj_t *loading_spinner;

    lv_obj_t *gif_image;

    void swipe_screen_event_cb(lv_event_t *e);
    void swipe_digital_clock_screen();
    void swipe_analog_clock_screen();
    void swipe_weather_screen();
    void swipe_alarm_screen();
    void switch_theme(bool dark_theme_enabled);
    void screen_load_event_cb(lv_event_t *e);

    void user_activity_event_cb(lv_event_t *e);
    void screen_timer_cb(lv_timer_t *timer);
    void dock_panel_timer_cb(lv_timer_t *timer);
    void settings_button_event_cb(lv_event_t *e);
    void theme_switch_event_cb(lv_event_t *e);
    void switch_to_light_theme();
    void switch_to_dark_theme();
    
    void init_light_theme();
    void init_dark_theme();


    void create_loading_screen();
    void delete_loading_screen();

    void setup_gui();

    void create_gif_img();
    void set_gif_parent();
    void update_gif_img_src();

    void load_default_screen();

    GuiApp(StateApp *state_app);
    ~GuiApp();
};
