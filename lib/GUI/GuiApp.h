#pragma once
#include "Alarm.h"
#include "DigitalClock.h"
#include "AnalogClock.h"
#include "Config.h"
#include "Weather.h"
#include "DockPanel.h"


class GuiApp
{
private:
    /* data */
public:

    Alarm *alarm_screen;
    DigitalClock *digital_clock_screen;
    AnalogClock *analog_clock_screen;
    Weather *weather_screen;
    DockPanel *dock_panel;
    void swipe_screen_event_cb(lv_event_t *e);
    void swipe_digital_clock_screen();
    void swipe_analog_clock_screen();
    void swipe_weather_screen();
    void swipe_alarm_screen();

    void screen_load_event_cb(lv_event_t *e);

    // void darkmode_switch_event_cb(lv_event_t *e);
    // void settings_textarea_event_cb(lv_event_t *e);

    void init_gui();
    GuiApp(/* args */);
    ~GuiApp();
};


