#pragma once
#include "Config/Config.h"
#include "Display/Display.h"
#include "GUI/Settings.h"
#include <Arduino.h>
#include <BH1750.h>
#include "StateApp.h"

class BrightnessApp {
  private:
    BH1750 _light_sensor;
    lv_timer_t *_light_sensor_timer = NULL;
    lv_anim_t _brightness_anim;
    Display *_display;
    Settings *_settings;


    int map_light_level(int light_level);
    void change_brightness_smoothly(int new_light_level, int old_light_level);

  public:
    StateApp *_state_app;

    bool begin();
    void set_auto_brightness_timer(bool auto_brightness);
    void light_sensor_timer_cb();
    void set_display_brightness(u_int32_t brightness);
    void update_settings_slider(u_int32_t slider_value);
    int get_light_level();
    BrightnessApp(Display *display, Settings *settings, StateApp *state_app);
    ~BrightnessApp();
};
