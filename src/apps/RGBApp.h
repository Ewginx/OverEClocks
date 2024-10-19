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

    RGBApp(StateApp *stateApp);
    ~RGBApp();

  private:
    enum RGB_effect {
        oneColor = 1,
        threeColors,
        rainbow,
        runningRainbow,
        cycleThreeColorsWithBreathe
    };
    StateApp *stateApp;
    Adafruit_NeoPixel pixels;

    lv_timer_t *p_rgbTimer = NULL;

    uint8_t borderPixels = NUMBER_OF_PIXELS / 3;

    uint32_t wheel(byte WheelPos);

    bool alreadyDisabled = false;
    bool staticEffectEnabled = false;

    uint16_t rainbowCycles = 0;

    uint32_t threeColorsArray[3];
    uint8_t breatheIterator = 1;
    uint8_t colorIterator = 0;
    uint8_t rgbBreatheCount = 1;

    void begin();
    void toggle();

    void setBrightness();
    void calculateBreatheCount();
    void updateThreeColorArray();
    void changeIteratorToNextColor();

    bool isDisabledAtNight();

    bool isDynamicEffect();

    bool isEnabled();

    // RGB effects
    void oneColorEffect();
    void threeColorsEffect();
    void cycleThreeColorsBreatheEffect();
    void rainbowEffect();
    void runningRainbowEffect();
};
