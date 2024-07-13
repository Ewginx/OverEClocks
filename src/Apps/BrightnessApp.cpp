#include "BrightnessApp.h"

static BrightnessApp *instance = NULL;

extern "C" void autoBrightnessCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->setAutoBrightnessTimer(instance->stateApp->displayState->autoBrightness);
}
extern "C" void brightnessChangedCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->setDisplayBrightness(instance->stateApp->displayState->brightnessLevel);
}
extern "C" void lightSensorTimerCallbackWrapper(lv_timer_t *timer) {
    instance->lightSensorTimerCallback();
}
extern "C" void brightnessSmoothChangeCallback(void *var, int32_t v) {
    instance->setDisplayBrightness(v);
}

BrightnessApp::BrightnessApp(Display *display, Settings *settings, StateApp *stateApp) {
    instance = this;
    this->display = display;
    this->settings = settings;
    this->stateApp = stateApp;
    lv_anim_init(&brightnessLevelAnimation);
    lv_msg_subscribe(MSG_BRIGHTNESS_CHANGED, brightnessChangedCallbackWrapper, NULL);
    lv_msg_subscribe(MSG_AUTO_BRIGHTNESS, autoBrightnessCallbackWrapper, NULL);
}

bool BrightnessApp::begin() {
    if (!lightSensor.begin(BH1750_TO_GROUND)) {
        Serial.println("Can't find the light sensor");
        return false;
    } else {
        Serial.println("Light sensor is connected");
        lightSensor.setQuality(BH1750Quality::BH1750_QUALITY_LOW);
        lightSensor.calibrateTiming();
        lightSensor.start();
        return true;
    }
}

void BrightnessApp::lightSensorTimerCallback() {
    if (lightSensor.hasValue()) {
        int lightLevel = this->getLightLevel();
        lightSensor.start();
        int oldBrightness = this->display->get_brightness();
        int newBrightness = this->mapLightLevel(lightLevel);
        this->switchTheme(newBrightness);
        this->changeBrightnessSmoothly(newBrightness, oldBrightness);
    }
}

int BrightnessApp::getLightLevel() { return int(this->lightSensor.getLux()); }

int BrightnessApp::mapLightLevel(int lightLevel) {
    if (lightLevel > this->stateApp->displayState->threshold) {
        return 255;
    }
    int output = static_cast<int>(1 + (250 / (this->stateApp->displayState->threshold)) *
                                          (lightLevel));
    return output;
}

void BrightnessApp::switchTheme(uint8_t brightness) {
    if (!this->stateApp->displayState->autoThemeChange) {
        return;
    }
    bool darkTheme = true;
    if (brightness < DARK_THEME_CHANGE_THRESHOLD) {
        if (!stateApp->themeState->currentThemeIsDark) {
            lv_msg_send(MSG_CHANGE_THEME, static_cast<const void *>(&darkTheme));
        }
    } else if (stateApp->themeState->currentThemeIsDark !=
               this->stateApp->themeState->darkThemeEnabled) {
        if (brightness > LIGHT_THEME_CHANGE_THRESHOLD) {
            darkTheme = this->stateApp->themeState->darkThemeEnabled;
            lv_msg_send(MSG_CHANGE_THEME, static_cast<const void *>(&darkTheme));
        }
    }
}

void BrightnessApp::changeBrightnessSmoothly(int newLightLevel, int oldLightLevel) {
    lv_anim_set_exec_cb(&brightnessLevelAnimation, brightnessSmoothChangeCallback);
    lv_anim_set_time(&brightnessLevelAnimation, 200);
    lv_anim_set_values(&brightnessLevelAnimation, oldLightLevel, newLightLevel);
    lv_anim_start(&brightnessLevelAnimation);
}

void BrightnessApp::setAutoBrightnessTimer(bool autoBrightness) {
    this->settings->set_brightness_checkbox(autoBrightness);
    if (this->lightSensorTimer == NULL & autoBrightness) {
        lightSensorTimer = lv_timer_create(lightSensorTimerCallbackWrapper, 1500, NULL);
    } else if (this->lightSensorTimer != NULL & !autoBrightness) {
        lv_timer_del(this->lightSensorTimer);
        this->lightSensorTimer = NULL;
    }
}

void BrightnessApp::setDisplayBrightness(uint8_t brightness) {
    this->settings->set_brightness_slider(brightness);
    this->display->set_brightness((uint8_t)brightness);
}

BrightnessApp::~BrightnessApp() {}