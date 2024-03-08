#pragma once
#include "Config/Config.h"
#include "StateApp.h"
#include "lvgl.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

class RGBApp {
  public:
    Adafruit_NeoPixel pixels;

    StateApp *_state_app;
    void setup();

    void begin_rgb();

    void show();

    void switch_rgb();

    void set_brightness();
    
    void update_rgb();

    bool is_rgb_enabled();

    uint32_t wheel(byte WheelPos);

    //RGB effects
    void solid_color_effect();
    
    void rainbow_effect();

    RGBApp(StateApp *state_app);
    ~RGBApp();

  private:
    lv_timer_t *_rgb_show_timer = NULL;
    int rainbowCycles = 0;
    bool solid_enabled = false;
};
