#include "StateApp.h"

static StateApp *instance = NULL;

StateApp::StateApp() { instance = this; }

void StateApp::save_dark_theme_enabled(bool enabled) {
    this->dark_theme_enabled = enabled;
    this->_preferences.begin(NAMESPACE);
    this->_preferences.putBool("dark_theme", enabled);
    this->_preferences.end();
}
void StateApp::init_state() {
    _preferences.begin(NAMESPACE);
    this->ssid = _preferences.getString("ssid", "");
    this->password = _preferences.getString("password", "");
    this->city = _preferences.getString("city", "");
    this->language = _preferences.getString("language", "");
    this->auto_brightness = _preferences.getBool("auto_bright", false);
    this->brightness_level = _preferences.getUInt("brightness", 255);
    this->weather_enabled = _preferences.getBool("weather_enab", false);
    this->dark_theme_enabled = _preferences.getBool("dark_theme", false);
    this->weekdays_switch_enabled = _preferences.getBool("weekdays_sw", false);
    this->weekends_switch_enabled = _preferences.getBool("weekends_sw", false);
    this->oneOff_switch_enabled = _preferences.getBool("oneOff_sw", false);
    this->weekdays_time = _preferences.getString("weekdays_time", "08:00");
    this->weekends_time = _preferences.getString("weekends_time", "09:15");
    this->oneOff_time = _preferences.getString("oneOff_time", "13:23");
    _preferences.end();
}
void StateApp::save_alarm_switches_enabled(bool weekdays_enabled, bool weekends_enabled,
                                           bool oneOff_enabled) {
    this->weekdays_switch_enabled = weekdays_enabled;
    this->weekends_switch_enabled = weekends_enabled;
    this->oneOff_switch_enabled = oneOff_enabled;
    _preferences.begin(NAMESPACE);
    _preferences.putBool("weekdays_sw", weekdays_enabled);
    _preferences.putBool("weekends_sw", weekends_enabled);
    _preferences.putBool("oneOff_sw", oneOff_enabled);
    _preferences.end();
}
void StateApp::save_auto_brightness_enabled(bool enabled) {
    this->auto_brightness = enabled;
    _preferences.begin(NAMESPACE);
    _preferences.putBool("auto_bright", enabled);
    _preferences.end();
}
void StateApp::save_weather_enabled(bool enabled) {
    this->weather_enabled = enabled;
    _preferences.begin(NAMESPACE);
    _preferences.putBool("weather_enab", enabled);
    _preferences.end();
}
void StateApp::save_brightness_level(unsigned int brightness_level) {
    this->brightness_level = brightness_level;
    _preferences.begin(NAMESPACE);
    _preferences.putUInt("brightness", brightness_level);
    _preferences.end();
}
void StateApp::save_ssid(const char *ssid) {
    this->ssid = ssid;
    _preferences.begin(NAMESPACE);
    _preferences.putString("ssid", ssid);
    _preferences.end();
}
void StateApp::save_password(const char *password) {
    this->password = password;
    _preferences.begin(NAMESPACE);
    _preferences.putString("password", password);
    _preferences.end();
}
void StateApp::save_city(const char *city) {
    this->city = city;
    _preferences.begin(NAMESPACE);
    _preferences.putString("city", city);
    _preferences.end();
}
void StateApp::save_language(const char *language) {
    this->language = language;
    _preferences.begin(NAMESPACE);
    _preferences.putString("language", language);
    _preferences.end();
}
void StateApp::save_alarm_time(const char *weekdays_time, const char *weekends_time,
                               const char *oneOff_time) {
    this->weekdays_time = weekdays_time;
    this->weekends_time = weekends_time;
    this->oneOff_time = oneOff_time;
    _preferences.begin(NAMESPACE);
    _preferences.putString("weekdays_time", weekdays_time);
    _preferences.putString("weekends_time", weekends_time);
    _preferences.putString("oneOff_time", oneOff_time);
    _preferences.end();
}
StateApp::~StateApp() {}