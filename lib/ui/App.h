#include "AlarmScreen.h"
#include "DigitalClockScreen.h"
#include "AnalogClockScreen.h"
#include "SettingsScreen.h"
#include "WeatherScreen.h"
#include "DockPanel.h"

class App
{
private:
    /* data */
public:

    AlarmScreen *alarm_screen;
    DigitalClockScreen *digital_clock_screen;
    AnalogClockScreen *analog_clock_screen;
    WeatherScreen *weather_screen;
    SettingsScreen *settings_screen;
    DockPanel *dock_panel;
    void swipe_screen_event_cb(lv_event_t *e);
    void swipe_digital_clock_screen();
    void swipe_analog_clock_screen();
    void swipe_weather_screen();
    void swipe_alarm_screen();
    void screen_load_event_cb(lv_event_t *e);
    void settings_button_event_cb(lv_event_t *e);

    void init_gui();
    App(/* args */);
    ~App();
};


