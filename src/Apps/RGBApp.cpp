#include "RGBApp.h"
static RGBApp *instance = NULL;

extern "C" void switch_rgb_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->switch_rgb();
}
extern "C" void rgb_show_cb_wrapper(lv_timer_t *timer) { instance->show(); }

RGBApp::RGBApp(StateApp *state_app) : pixels(NUMPIXELS, RGB_PIN) {
    instance = this;
    this->_state_app = state_app;
    _rgb_show_timer = lv_timer_create(rgb_show_cb_wrapper, this->_state_app->rgb_state->delay, NULL);
    lv_timer_pause(this->_rgb_show_timer);
    lv_msg_subscribe(MSG_SWITCH_RGB, switch_rgb_cb_wrapper, NULL);
}

void RGBApp::begin_rgb() {
    this->pixels.begin();
    this->pixels.show();
}

void RGBApp::show() {
    if(this->_state_app->rgb_state->effect != 1){
        this->solid_enabled = false;
    }
    if (this->_state_app->rgb_state->effect == 1) {
        if (!this->solid_enabled) {
            this->solid_color_effect();
            this->solid_enabled = true;
        }
    } else if (this->_state_app->rgb_state->effect == 2) {
        this->rainbow_effect();
    }
}

void RGBApp::switch_rgb() {
    if (this->_state_app->rgb_state->enabled) {
        lv_timer_resume(_rgb_show_timer);
    } else {
        lv_timer_pause(_rgb_show_timer);
    }
}

void RGBApp::solid_color_effect() {
    pixels.clear();
    for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, this->_state_app->rgb_state->first_rgb_color);
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