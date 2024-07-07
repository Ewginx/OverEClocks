#include "RGBApp.h"
static RGBApp *instance = NULL;

extern "C" void update_rgb_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->update();
}
extern "C" void rgb_show_cb_wrapper(lv_timer_t *timer) { instance->show(); }

RGBApp::RGBApp(StateApp *state_app) : pixels(NUMBER_OF_PIXELS, RGB_PIN) {
    instance = this;
    this->_state_app = state_app;
    this->update_triColor_array();
    this->calculate_breathe_count();
    lv_msg_subscribe(MSG_RGB_STATE_CHANGED, update_rgb_cb_wrapper, NULL);
}

void RGBApp::begin() {
    this->pixels.begin();
    this->pixels.clear();
}

void RGBApp::setup() {
    this->begin();
    _rgb_timer =
        lv_timer_create(rgb_show_cb_wrapper, this->_state_app->rgb_state->delay, NULL);
    this->toggle();
}

void RGBApp::toggle() {
    if (this->is_enabled()) {
        lv_timer_set_period(this->_rgb_timer, this->_state_app->rgb_state->delay);
        lv_timer_resume(_rgb_timer);
    } else {
        this->pixels.clear();
        pixels.show();
        lv_timer_pause(_rgb_timer);
    }
}

bool RGBApp::is_enabled() { return this->_state_app->rgb_state->enabled; }

void RGBApp::update() {
    this->breathe_iterator = 1;
    this->calculate_breathe_count();
    this->update_triColor_array();
    this->toggle();
    this->set_brightness();
}

void RGBApp::calculate_breathe_count() {
    this->rgb_breathe_count =
        this->_state_app->rgb_state->brightness / RGB_BREATHE_STEP * 2;
}

void RGBApp::update_triColor_array() {
    this->three_colors_array[0] = this->_state_app->rgb_state->first_rgb_color;
    this->three_colors_array[1] = this->_state_app->rgb_state->second_rgb_color;
    this->three_colors_array[2] = this->_state_app->rgb_state->third_rgb_color;
}

void RGBApp::change_iterator_to_next_color() {
    if (this->color_iterator == 2) {
        this->color_iterator = 0;
    } else {
        this->color_iterator++;
    }
}

void RGBApp::set_brightness() {
    this->pixels.setBrightness(this->_state_app->rgb_state->brightness);
    this->pixels.show();
}

void RGBApp::show() {
    if (!this->is_enabled() || this->is_disabled_at_night()) {
        return;
    }
    if (this->is_dynamic_effect()) {
        this->_static_effect_enabled = false;
    }
    switch (this->_state_app->rgb_state->effect) {
    case RGB_effect::one_color:
        if (!this->_static_effect_enabled) {
            this->one_color_effect();
            this->_static_effect_enabled = true;
        }
        break;
    case RGB_effect::three_colors:
        if (!this->_static_effect_enabled) {
            this->three_colors_effect();
            this->_static_effect_enabled = true;
        }
        break;
    case RGB_effect::rainbow:
        this->rainbow_effect();
        break;
    case RGB_effect::running_rainbow:
        this->running_rainbow_effect();
        break;
    case RGB_effect::cycle_three_colors_with_breathe:
        this->cycle_three_colors_breathe_effect();
        break;
    default:
        break;
    }
}

bool RGBApp::is_disabled_at_night() {
    if (this->_state_app->rgb_state->turn_off_at_night &
        this->_state_app->time_state->is_night) {
        if (!this->_already_disabled) {
            this->pixels.clear();
            pixels.show();
            this->_already_disabled = true;
        }
        return true;
    } else {
        this->_already_disabled = false;
        return false;
    }
}

bool RGBApp::is_dynamic_effect() {
    if (this->_state_app->rgb_state->effect != RGB_effect::one_color ||
        this->_state_app->rgb_state->effect != RGB_effect::three_colors) {
        return true;
    }
    return false;
}

void RGBApp::one_color_effect() {
    pixels.clear();
    for (uint8_t i = 0; i < NUMBER_OF_PIXELS; i++) {
        pixels.setPixelColor(i, this->_state_app->rgb_state->first_rgb_color);
    }
    pixels.show();
}
void RGBApp::three_colors_effect() {
    pixels.clear();
    for (uint8_t i = 0; i < this->border_pixels; i++) {
        pixels.setPixelColor(i, this->_state_app->rgb_state->first_rgb_color);
    }
    for (uint8_t i = this->border_pixels; i < NUMBER_OF_PIXELS - this->border_pixels;
         i++) {
        pixels.setPixelColor(i, this->_state_app->rgb_state->second_rgb_color);
    }
    for (uint8_t i = NUMBER_OF_PIXELS - this->border_pixels; i < NUMBER_OF_PIXELS; i++) {
        pixels.setPixelColor(i, this->_state_app->rgb_state->third_rgb_color);
    }
    pixels.show();
}
void RGBApp::cycle_three_colors_breathe_effect() {
    pixels.clear();
    if (this->breathe_iterator == this->rgb_breathe_count - 1) {
        this->breathe_iterator = 1;
    }

    for (uint8_t i = 0; i < NUMBER_OF_PIXELS; i++) {
        pixels.setPixelColor(i, this->three_colors_array[color_iterator]);
    }

    if (this->breathe_iterator < this->rgb_breathe_count / 2) {
        pixels.setBrightness(this->_state_app->rgb_state->brightness -
                             (this->breathe_iterator * RGB_BREATHE_STEP));
        this->breathe_iterator++;
        pixels.show();
        return;
    }

    if (this->breathe_iterator == this->rgb_breathe_count / 2) {
        this->change_iterator_to_next_color();
        this->breathe_iterator++;
        return;
    }
    if (this->breathe_iterator > this->rgb_breathe_count / 2) {
        pixels.setBrightness(
            this->_state_app->rgb_state->brightness -
            ((this->rgb_breathe_count - this->breathe_iterator) * RGB_BREATHE_STEP));
        this->breathe_iterator++;
        pixels.show();
        return;
    }
}
void RGBApp::rainbow_effect() {
    uint32_t color = this->wheel((rainbowCycles)&255);
    for (uint8_t i = 0; i < NUMBER_OF_PIXELS; i++) {
        pixels.setPixelColor(i, color);
    }
    pixels.show();
    this->rainbowCycles += 2;
    if (rainbowCycles >= 256) {
        rainbowCycles = 0;
    }
}
void RGBApp::running_rainbow_effect() {
    for (uint8_t i = 0; i < NUMBER_OF_PIXELS; i++) {
        pixels.setPixelColor(i, this->wheel((i + rainbowCycles) & 255));
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
