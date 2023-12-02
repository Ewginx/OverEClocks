#pragma once
#include "Config/Config.h"
#include "GUI/AlarmClock.h"
#include "GUI/AnalogClock.h"
#include "GUI/DigitalClock.h"
#include "GUI/DockPanel.h"
#include "GUI/Settings.h"
#include "GUI/Weather.h"

class GuiApp {
  private:
    lv_timer_t *_screen_timer;
    /* data */
  public:
    AlarmClock *alarm_clock;
    DigitalClock *digital_clock;
    AnalogClock *analog_clock;
    Weather *weather;
    Settings *settings;
    DockPanel *dock_panel;

    lv_obj_t *loading_screen;
    lv_obj_t *loading_spinner;

    void swipe_screen_event_cb(lv_event_t *e);
    void swipe_digital_clock_screen();
    void swipe_analog_clock_screen();
    void swipe_weather_screen();
    void swipe_alarm_screen();

    void screen_load_event_cb(lv_event_t *e);

    void user_activity_event_cb(lv_event_t *e);
    void screen_timer_cb(lv_timer_t *timer);
    void settings_button_event_cb(lv_event_t *e);
    void darkmode_switch_event_cb(lv_event_t *e);
    void set_light_theme(lv_disp_t *display);
    void set_dark_theme(lv_disp_t *display);

    void create_loading_screen();
    void delete_loading_screen();
    void set_screens_to_default_values();
    // void settings_textarea_event_cb(lv_event_t *e);

    void init_gui();
    GuiApp(/* args */);
    ~GuiApp();
};
