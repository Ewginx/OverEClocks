#include "RGBApp.h"
static RGBApp *instance = NULL;

extern "C" void update_rgb_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->update_rgb();
}
extern "C" void rgb_show_cb_wrapper(lv_timer_t *timer) { instance->show(); }

RGBApp::RGBApp(StateApp *state_app) : pixels(NUMPIXELS, RGB_PIN) {
    instance = this;
    this->_state_app = state_app;
    this->update_triColor_array();
    this->calculate_breathe_count();
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
        this->cycle_tri_colors_breathe_effect();
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
void RGBApp::cycle_tri_colors_breathe_effect() {
    pixels.clear();
    if (this->triCyclesBreatheCnt == this->rgb_breathe_count - 1) {
        this->triCyclesBreatheCnt = 1;
    }
    for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, this->tri_color[solid_tri_cycle_iterator]);
    }

    if (this->triCyclesBreatheCnt < this->rgb_breathe_count / 2) {
        pixels.setBrightness(this->_state_app->rgb_state->brightness -
                             (this->triCyclesBreatheCnt * RGB_BREATHE_STEP));
        this->triCyclesBreatheCnt++;
        pixels.show();
        return;
    }

    if (this->triCyclesBreatheCnt == this->rgb_breathe_count / 2) {
        this->triCyclesBreatheCnt++;
        if (solid_tri_cycle_iterator == 2) {
            solid_tri_cycle_iterator = 0;
        } else {
            solid_tri_cycle_iterator++;
        }
        return;
    }
    if (this->triCyclesBreatheCnt > this->rgb_breathe_count / 2) {
        pixels.setBrightness(
            this->_state_app->rgb_state->brightness -
            ((this->rgb_breathe_count - this->triCyclesBreatheCnt) * RGB_BREATHE_STEP));
        this->triCyclesBreatheCnt++;
        pixels.show();
        return;
    }
}
void RGBApp::rainbow_effect() {
    uint32_t color = this->wheel((rainbowCycles)&255);
    for (uint16_t i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, color);
    }
    pixels.show();
    this->rainbowCycles += 2;
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
void RGBApp::set_rgb_brightness() {
    this->pixels.setBrightness(this->_state_app->rgb_state->brightness);
    this->solid_enabled = false;
}
void RGBApp::update_rgb() {
    this->triCyclesBreatheCnt = 1;
    this->calculate_breathe_count();
    this->update_triColor_array();
    this->switch_rgb();
    this->set_rgb_brightness();
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

void RGBApp::calculate_breathe_count() {
    this->rgb_breathe_count =
        this->_state_app->rgb_state->brightness / RGB_BREATHE_STEP * 2;
}

void RGBApp::update_triColor_array() {
    this->tri_color[0] = this->_state_app->rgb_state->first_rgb_color;
    this->tri_color[1] = this->_state_app->rgb_state->second_rgb_color;
    this->tri_color[2] = this->_state_app->rgb_state->third_rgb_color;
}
