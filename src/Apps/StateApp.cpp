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
    this->ap_login = _preferences.getString("sta_login", "");
    this->ap_password = _preferences.getString("sta_password", "");

    this->weather_enabled = _preferences.getBool("weather_enab", false);
    this->city = _preferences.getString("city", "");
    this->language = _preferences.getString("language", "");
    this->api_key = _preferences.getString("api_key", API_KEY);
    this->request_period =
        _preferences.getInt("request_period", WEATHER_API_POLLING_INTERVAL_MILLISECONDS);

    this->auto_brightness = _preferences.getBool("auto_bright", false);
    this->auto_theme_change = _preferences.getBool("auto_theme", false);
    this->brightness_level = _preferences.getUInt("brightness", 255);
    this->threshold = _preferences.getInt("threshold", 120);

    this->digital_main_screen = _preferences.getBool("dig_main_screen", true);

    this->dark_theme_enabled = _preferences.getBool("dark_theme", false);
    this->light_primary_color = _preferences.getInt("light_back",0);
    this->light_second_color = _preferences.getInt("light_second", 0);
    this->dark_background_color = _preferences.getInt("dark_back", 0);
    this->dark_second_color = _preferences.getInt("dark_second", 0);

    this->timezone_posix = _preferences.getString("timezone", "<+10>-10");

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
void StateApp::save_auto_theme_change(bool change) {
    this->auto_theme_change = change;
    _preferences.begin(NAMESPACE);
    _preferences.putBool("auto_theme", change);
    _preferences.end();
}
void StateApp::save_digital_main_screen(bool digital_main_screen) {
    this->digital_main_screen = digital_main_screen;
    _preferences.begin(NAMESPACE);
    _preferences.putBool("dig_main_screen", digital_main_screen);
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
void StateApp::save_brightness_threshold(int threshold) {
    this->threshold = threshold;
    _preferences.begin(NAMESPACE);
    _preferences.putInt("threshold", threshold);
    _preferences.end();
}
void StateApp::save_city(const char *city) {
    this->city = city;
    _preferences.begin(NAMESPACE);
    _preferences.putString("city", city);
    _preferences.end();
}
void StateApp::save_ap_login(const char *login) {
    this->ap_login = login;
    _preferences.begin(NAMESPACE);
    _preferences.putString("ap_login", login);
    _preferences.end();
}
void StateApp::save_ap_password(const char *password) {
    this->ap_password = password;
    _preferences.begin(NAMESPACE);
    _preferences.putString("ap_password", password);
    _preferences.end();
}
void StateApp::save_language(const char *language) {
    this->language = language;
    _preferences.begin(NAMESPACE);
    _preferences.putString("language", language);
    _preferences.end();
}
void StateApp::save_api_key(const char *api_key) {
    this->api_key = api_key;
    _preferences.begin(NAMESPACE);
    _preferences.putString("api_key", api_key);
    _preferences.end();
}
void StateApp::save_request_period(int request_period) {
    this->request_period = request_period;
    _preferences.begin(NAMESPACE);
    _preferences.putInt("request_period", request_period);
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
void StateApp::save_light_colors(int light_primary_color, int light_second_color) {
    this->light_primary_color = light_primary_color;
    this->light_second_color = light_second_color;
    _preferences.begin(NAMESPACE);
    _preferences.putInt("light_back", light_primary_color);
    _preferences.putInt("light_second", light_second_color);
    _preferences.end();
}
void StateApp::save_dark_colors(int dark_background_color, int dark_second_color) {
    this->dark_background_color = dark_background_color;
    this->dark_second_color = dark_second_color;
    _preferences.begin(NAMESPACE);
    _preferences.putInt("dark_back", dark_background_color);
    _preferences.putInt("dark_second", dark_second_color);
    _preferences.end();
}
void StateApp::save_timezone(const char *timezone_posix) {
    this->timezone_posix = timezone_posix;
    _preferences.begin(NAMESPACE);
    _preferences.putString("timezone", timezone_posix);
    _preferences.end();
}
StateApp::~StateApp() {}