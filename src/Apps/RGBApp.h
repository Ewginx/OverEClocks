#pragma once
#include "Config/Config.h"
#include "StateApp.h"
#include "lvgl.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

class RGBApp {
  public:

    void setup();
    void show();
    void update();

    RGBApp(StateApp *state_app);
    ~RGBApp();

  private:
    enum RGB_effect {
        one_color = 1,
        three_colors,
        rainbow,
        running_rainbow,
        cycle_three_colors_with_breathe
    };
    StateApp *_state_app;
    Adafruit_NeoPixel pixels;

    lv_timer_t *_rgb_timer = NULL;

    uint8_t border_pixels = NUMBER_OF_PIXELS / 3;

    uint32_t wheel(byte WheelPos);

    bool _already_disabled = false;
    bool _static_effect_enabled = false;

    uint16_t rainbowCycles = 0;

    uint32_t three_colors_array[3];
    uint8_t breathe_iterator = 1;
    uint8_t color_iterator = 0;
    uint8_t rgb_breathe_count = 1;

    void begin();
    void toggle();

    void set_brightness();
    void calculate_breathe_count();
    void update_triColor_array();
    void change_iterator_to_next_color();

    bool is_disabled_at_night();

    bool is_dynamic_effect();

    bool is_enabled();

    // RGB effects
    void one_color_effect();
    void three_colors_effect();
    void cycle_three_colors_breathe_effect();
    void rainbow_effect();
    void running_rainbow_effect();
};
