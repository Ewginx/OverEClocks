#include "RGBApp.h"
static RGBApp *instance = NULL;

extern "C" void update_rgb_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->update();
}
extern "C" void rgb_show_cb_wrapper(lv_timer_t *timer) { instance->show(); }

RGBApp::RGBApp(StateApp *p_stateApp) : pixels(NUMBER_OF_PIXELS, RGB_PIN) {
    instance = this;
    this->p_stateApp = p_stateApp;
    this->updateThreeColorArray();
    this->calculateBreatheCount();
    lv_msg_subscribe(MSG_RGB_STATE_CHANGED, update_rgb_cb_wrapper, NULL);
}

void RGBApp::begin() {
    pixels.begin();
    pixels.clear();
}

void RGBApp::setup() {
    this->begin();
    p_rgbTimer =
        lv_timer_create(rgb_show_cb_wrapper, p_stateApp->rgb_state->delay, NULL);
    this->toggle();
}

void RGBApp::toggle() {
    if (this->isEnabled()) {
        lv_timer_set_period(this->p_rgbTimer, p_stateApp->rgb_state->delay);
        lv_timer_resume(this->p_rgbTimer);
    } else {
        pixels.clear();
        pixels.show();
        lv_timer_pause(this->p_rgbTimer);
    }
}

bool RGBApp::isEnabled() { return p_stateApp->rgb_state->enabled; }

void RGBApp::update() {
    breatheIterator = 1;
    this->calculateBreatheCount();
    this->updateThreeColorArray();
    this->toggle();
    this->setBrightness();
}

void RGBApp::calculateBreatheCount() {
    rgbBreatheCount = p_stateApp->rgb_state->brightness / RGB_BREATHE_STEP * 2;
}

void RGBApp::updateThreeColorArray() {
    threeColorsArray[0] = p_stateApp->rgb_state->first_rgb_color;
    threeColorsArray[1] = p_stateApp->rgb_state->second_rgb_color;
    threeColorsArray[2] = p_stateApp->rgb_state->third_rgb_color;
}

void RGBApp::changeIteratorToNextColor() {
    if (colorIterator == 2) {
        colorIterator = 0;
    } else {
        colorIterator++;
    }
}

void RGBApp::setBrightness() {
    pixels.setBrightness(this->p_stateApp->rgb_state->brightness);
    pixels.show();
}

void RGBApp::show() {
    if (!this->isEnabled() || this->isDisabledAtNight()) {
        return;
    }
    if (this->isDynamicEffect()) {
        staticEffectEnabled = false;
    }
    switch (this->p_stateApp->rgb_state->effect) {
    case RGB_effect::oneColor:
        if (!staticEffectEnabled) {
            this->oneColorEffect();
            staticEffectEnabled = true;
        }
        break;
    case RGB_effect::threeColors:
        if (!staticEffectEnabled) {
            this->threeColorsEffect();
            staticEffectEnabled = true;
        }
        break;
    case RGB_effect::rainbow:
        this->rainbowEffect();
        break;
    case RGB_effect::runningRainbow:
        this->runningRainbowEffect();
        break;
    case RGB_effect::cycleThreeColorsWithBreathe:
        this->cycleThreeColorsBreatheEffect();
        break;
    default:
        break;
    }
}

bool RGBApp::isDisabledAtNight() {
    if (p_stateApp->rgb_state->turn_off_at_night &
        p_stateApp->time_state->is_night) {
        if (!alreadyDisabled) {
            pixels.clear();
            pixels.show();
            alreadyDisabled = true;
        }
        return true;
    } else {
        alreadyDisabled = false;
        return false;
    }
}

bool RGBApp::isDynamicEffect() {
    if (p_stateApp->rgb_state->effect != RGB_effect::oneColor ||
        p_stateApp->rgb_state->effect != RGB_effect::threeColors) {
        return true;
    }
    return false;
}

void RGBApp::oneColorEffect() {
    pixels.clear();
    for (uint8_t i = 0; i < NUMBER_OF_PIXELS; i++) {
        pixels.setPixelColor(i, p_stateApp->rgb_state->first_rgb_color);
    }
    pixels.show();
}
void RGBApp::threeColorsEffect() {
    pixels.clear();
    for (uint8_t i = 0; i < borderPixels; i++) {
        pixels.setPixelColor(i, p_stateApp->rgb_state->first_rgb_color);
    }
    for (uint8_t i = borderPixels; i < NUMBER_OF_PIXELS - borderPixels; i++) {
        pixels.setPixelColor(i, p_stateApp->rgb_state->second_rgb_color);
    }
    for (uint8_t i = NUMBER_OF_PIXELS - borderPixels; i < NUMBER_OF_PIXELS; i++) {
        pixels.setPixelColor(i, p_stateApp->rgb_state->third_rgb_color);
    }
    pixels.show();
}
void RGBApp::cycleThreeColorsBreatheEffect() {
    pixels.clear();
    if (breatheIterator == rgbBreatheCount - 1) {
        breatheIterator = 1;
    }

    for (uint8_t i = 0; i < NUMBER_OF_PIXELS; i++) {
        pixels.setPixelColor(i, threeColorsArray[colorIterator]);
    }

    if (breatheIterator < rgbBreatheCount / 2) {
        pixels.setBrightness(p_stateApp->rgb_state->brightness -
                                   (breatheIterator * RGB_BREATHE_STEP));
        breatheIterator++;
        pixels.show();
        return;
    }

    if (breatheIterator == rgbBreatheCount / 2) {
        this->changeIteratorToNextColor();
        breatheIterator++;
        return;
    }
    if (breatheIterator > rgbBreatheCount / 2) {
        pixels.setBrightness(
            p_stateApp->rgb_state->brightness -
            ((rgbBreatheCount - breatheIterator) * RGB_BREATHE_STEP));
        breatheIterator++;
        pixels.show();
        return;
    }
}
void RGBApp::rainbowEffect() {
    uint32_t color = this->wheel((rainbowCycles)&255);
    for (uint8_t i = 0; i < NUMBER_OF_PIXELS; i++) {
        pixels.setPixelColor(i, color);
    }
    pixels.show();
    rainbowCycles += 2;
    if (rainbowCycles >= 256) {
        rainbowCycles = 0;
    }
}
void RGBApp::runningRainbowEffect() {
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
