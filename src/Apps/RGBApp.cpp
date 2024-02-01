#include "RGBApp.h"

RGBApp::RGBApp(StateApp *state_app) : pixels(NUMPIXELS, RGB_PIN) {
    this->_state_app = state_app;
}

void RGBApp::begin_rgb() {
    this->pixels.begin();
    this->pixels.show();
}

void RGBApp::show() {
    if (!rainbow) {
        this->solid_color_effect();
    } else if (rainbow) {
        this->rainbow_effect();
    }
}

void RGBApp::solid_color_effect() {
    pixels.clear();
    for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 255, 0));
        pixels.show();
    }
}
void RGBApp::rainbow_effect() {
    for (uint16_t i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, wheel((i + rainbowCycles) & 255));
    }
    pixels.show();
    rainbowCycles++;
    if (rainbowCycles >= 256) {
        rainbowCycles = 0;
    }
}
uint32_t RGBApp::wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
        return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
RGBApp::~RGBApp() {}