#include "BrightnessApp.h"

static BrightnessApp *instance = NULL;

extern "C" void auto_brightness_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->set_auto_brightness_timer(instance->_state_app->display_state->auto_brightness);
}
extern "C" void brightness_changed_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->set_display_brightness(instance->_state_app->display_state->brightness_level);
}
extern "C" void light_sensor_timer_cb_wrapper(lv_timer_t *timer) {
    instance->light_sensor_timer_cb();
}
extern "C" void anim_brightness_change_cb(void *var, int32_t v) {
    instance->set_display_brightness(v);
}

void BrightnessApp::light_sensor_timer_cb() {
    int light_level = this->get_light_level();
    int old_brightness = this->_display->get_brightness();
    int new_brightness = this->map_light_level(light_level);
    bool dark_theme = false;
    if (new_brightness < CHANGE_THEME_THRESHOLD & this->_state_app->display_state->auto_theme_change) {
        dark_theme = true;
        lv_msg_send(MSG_CHANGE_THEME, static_cast<const void *>(&dark_theme));
    } else if(this->_state_app->display_state->auto_theme_change){
        dark_theme = this->_state_app->theme_state->dark_theme_enabled ? true : false;
        lv_msg_send(MSG_CHANGE_THEME, static_cast<const void *>(&dark_theme));
    }

    this->change_brightness_smoothly(new_brightness, old_brightness);
    this->update_settings_slider(new_brightness);
}
void BrightnessApp::set_auto_brightness_timer(bool auto_brightness) {
    this->_settings->set_brightness_checkbox(auto_brightness);
    if (this->_light_sensor_timer == NULL & auto_brightness) {
        _light_sensor_timer = lv_timer_create(light_sensor_timer_cb_wrapper, 600, NULL);
    } else if (this->_light_sensor_timer != NULL & !auto_brightness) {
        lv_timer_del(this->_light_sensor_timer);
        this->_light_sensor_timer = NULL;
    }
}
void BrightnessApp::change_brightness_smoothly(int new_light_level, int old_light_level) {
    lv_anim_set_exec_cb(&_brightness_anim, anim_brightness_change_cb);
    lv_anim_set_var(&_brightness_anim, NULL);
    lv_anim_set_time(&_brightness_anim, 300);
    lv_anim_set_values(&_brightness_anim, old_light_level, new_light_level);
    lv_anim_start(&_brightness_anim);
}
void BrightnessApp::set_display_brightness(u_int32_t brightness) {
    this->_settings->set_brightness_slider(brightness);
    this->_display->set_brightness((uint8_t)brightness);
}
void BrightnessApp::update_settings_slider(u_int32_t slider_value) {
    this->_settings->set_brightness_slider(slider_value, true);
}
int BrightnessApp::get_light_level() {
    return static_cast<int>(this->_light_sensor.readLightLevel());
}
bool BrightnessApp::begin() {
    if (!_light_sensor.begin()) {
        Serial.println("Can't find the light sensor");
        return false;
    } else {
        Serial.println("Light sensor is connected");
        return true;
    }
}
int BrightnessApp::map_light_level(int light_level) {
    if (light_level > this->_state_app->display_state->threshold) {
        return 255;
    }
    int output =
        static_cast<int>(5 + (250 / (this->_state_app->display_state->threshold)) * (light_level));
    return output;
}
BrightnessApp::BrightnessApp(Display *display, Settings *settings, StateApp *state_app) {
    instance = this;
    this->_display = display;
    this->_settings = settings;
    this->_state_app = state_app;
    lv_anim_init(&_brightness_anim);
    lv_msg_subscribe(MSG_BRIGHTNESS_CHANGED, brightness_changed_cb_wrapper, NULL);
    lv_msg_subscribe(MSG_AUTO_BRIGHTNESS, auto_brightness_cb_wrapper, NULL);
}

BrightnessApp::~BrightnessApp() {}