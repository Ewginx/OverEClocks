#include "RGBApp.h"
static RGBApp *instance = NULL;

extern "C" void update_rgb_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->update_rgb();
}
extern "C" void rgb_show_cb_wrapper(lv_timer_t *timer) { instance->show(); }

RGBApp::RGBApp(StateApp *state_app) : pixels(NUMPIXELS, RGB_PIN) {
    instance = this;
    this->_state_app = state_app;
    this->tri_color[0] = this->_state_app->rgb_state->first_rgb_color;
    this->tri_color[1] = this->_state_app->rgb_state->second_rgb_color;
    this->tri_color[2] = this->_state_app->rgb_state->third_rgb_color;
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
    if (!this->is_rgb_enabled()) {
        if (!this->_already_disabled) {
            this->pixels.clear();
            pixels.show();
            this->_already_disabled = true;
        }

        return;
    }
    this->_already_disabled = false;
    if (this->_state_app->rgb_state->effect != 1 ||
        this->_state_app->rgb_state->effect != 2) {
        this->solid_enabled = false;
    }
    switch (this->_state_app->rgb_state->effect) {
    case 1:
        if (!this->solid_enabled) {
            this->solid_color_effect();
            this->solid_enabled = true;
        }
        break;
    case 2:
        if (!this->solid_enabled) {
            this->solid_tri_color_effect();
            this->solid_enabled = true;
        }
        break;
    case 3:
        this->rainbow_effect();
        break;
    case 4:
        this->running_rainbow_effect();
        break;
    case 5:
        this->cycle_tri_colors();
        break;

    default:
        break;
    }
}

void RGBApp::switch_rgb() {
    if (this->is_rgb_enabled()) {
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
    }
    pixels.show();
}
void RGBApp::solid_tri_color_effect() {
    pixels.clear();

    for (int i = 0; i < this->border_pixels; i++) {
        pixels.setPixelColor(i, this->_state_app->rgb_state->first_rgb_color);
    }
    for (int i = this->border_pixels; i < NUMPIXELS - this->border_pixels; i++) {
        pixels.setPixelColor(i, this->_state_app->rgb_state->second_rgb_color);
    }
    for (int i = NUMPIXELS - this->border_pixels; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, this->_state_app->rgb_state->third_rgb_color);
    }
    pixels.show();
}
void RGBApp::cycle_tri_colors() {
    pixels.clear();
    uint32_t startColor = this->tri_color[solid_tri_cycle_iterator];
    byte startRed = (startColor >> 16) & 0xff;
    byte startGreen = (startColor >> 8) & 0xff;
    byte startBlue = startColor & 0xff;
    uint32_t endColor = 0;
    if (this->tri_color[solid_tri_cycle_iterator] < 2) {
        endColor = this->tri_color[solid_tri_cycle_iterator + 1];
    } else {
        endColor = this->tri_color[0];
    }

    byte endRed = (endColor >> 16) & 0xff;
    byte endGreen = (endColor >> 8) & 0xff;
    byte endBlue = endColor & 0xff;
    for (int step = 0; step < 256; step++) {
        byte red = map(step, 0, 255, startRed, endRed);
        byte green = map(step, 0, 255, startGreen, endGreen);
        byte blue = map(step, 0, 255, startBlue, endBlue);
        for (int i = 0; i < NUMPIXELS; i++) {
            pixels.setPixelColor(i, red, green, blue);
        }
        pixels.show();
    }
    // triCycles++;
    // if (triCycles >= 256) {
    //     triCycles = 0;
    // }
    if (solid_tri_cycle_iterator == 2) {
        solid_tri_cycle_iterator = 0;
    } else {
        solid_tri_cycle_iterator++;
    }
}
void RGBApp::rainbow_effect() {
    uint32_t color = this->wheel((rainbowCycles) & 255);
    for (uint16_t i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, color);
    }
    pixels.show();
    rainbowCycles++;
    if (rainbowCycles >= 256) {
        rainbowCycles = 0;
    }
}
void RGBApp::running_rainbow_effect() {
    for (uint16_t i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, this->wheel((i + rainbowCycles) & 255));
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
bool RGBApp::is_rgb_enabled() {
    if ((this->_state_app->rgb_state->turn_off_at_night &
         this->_state_app->time_state->is_night) ||
        !this->_state_app->rgb_state->enabled) {
        return false;
    }
    return true;
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