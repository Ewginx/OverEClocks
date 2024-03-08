#include "RGBApp.h"
static RGBApp *instance = NULL;

extern "C" void update_rgb_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->update_rgb();
}
extern "C" void rgb_show_cb_wrapper(lv_timer_t *timer) { instance->show(); }

RGBApp::RGBApp(StateApp *state_app) : pixels(NUMPIXELS, RGB_PIN) {
    instance = this;
    this->_state_app = state_app;
    lv_msg_subscribe(MSG_RGB_STATE_CHANGED, update_rgb_cb_wrapper, NULL);
}

void RGBApp::setup() {
    this->begin_rgb();
    _rgb_show_timer =
        lv_timer_create(rgb_show_cb_wrapper, this->_state_app->rgb_state->delay, NULL);
    this->switch_rgb();
}

void RGBApp::begin_rgb() {
    this->pixels.begin();
    this->pixels.clear();
}

void RGBApp::show() {
    if (this->_state_app->rgb_state->effect != 1) {
        this->solid_enabled = false;
    }
    if (this->_state_app->rgb_state->effect == 1) {
        if (!this->solid_enabled) {
            this->solid_color_effect();
            this->solid_enabled = true;
        }
    } else if (this->_state_app->rgb_state->effect == 3) {
        this->rainbow_effect();
    }
}

void RGBApp::switch_rgb() {
    if (this->_state_app->rgb_state->enabled) {
        lv_timer_set_period(this->_rgb_show_timer, this->_state_app->rgb_state->delay);
        lv_timer_resume(_rgb_show_timer);
    } else {
        this->pixels.clear();
        pixels.show();
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
void RGBApp::set_brightness() {
    this->pixels.setBrightness(this->_state_app->rgb_state->brightness);
    this->solid_enabled = false;
}
void RGBApp::update_rgb() {
    this->switch_rgb();
    this->set_brightness();
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