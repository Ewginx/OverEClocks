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

    void begin_rgb();

    void show();

    void solid_color_effect();
    void rainbow_effect();
    uint32_t wheel(byte WheelPos);

    RGBApp(StateApp *state_app);
    ~RGBApp();

  private:
    unsigned long solid_color_effect_previous_millis = 0;
    unsigned long rainbow_previous_millis = 0;

    bool rainbow = false;

    int pixels_interval = 0;
    int rainbowCycles = 0;
};
