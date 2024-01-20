#include "StateApp.h"

static StateApp *instance = NULL;

StateApp::StateApp() {
    instance = this;
    wifi_state = new WiFiState(_preferences);
    weather_state = new WeatherState(_preferences);
    alarm_state = new AlarmState(_preferences);
    theme_state = new ThemeState(_preferences);
}

void StateApp::init_state() {
    _preferences.begin(NAMESPACE);
    this->timezone_posix = _preferences.getString("timezone", TIMEZONE);

    _preferences.end();
}

void StateApp::save_timezone(const char *timezone_posix) {
    this->timezone_posix = timezone_posix;
    this->_preferences.begin(NAMESPACE);
    this->_preferences.putString("timezone", timezone_posix);
    this->_preferences.end();
}
StateApp::~StateApp() {}

void WiFiState::save_ssid(const char *ssid) {
    this->ssid = ssid;
    this->_preferences.begin(NAMESPACE);
    this->_preferences.putString("ssid", ssid);
    this->_preferences.end();
}
void WiFiState::save_password(const char *password) {
    this->password = password;
    this->_preferences.begin(NAMESPACE);
    this->_preferences.putString("password", password);
    this->_preferences.end();
}
void WiFiState::save_ip_and_gateway_addresses(const char *ip_address,
                                              const char *gateway_address) {
    this->ip_address = ip_address;
    this->gateway_address = gateway_address;
    this->_preferences.begin(NAMESPACE);
    this->_preferences.putString("ip_address", ip_address);
    this->_preferences.putString("gateway_address", gateway_address);
    this->_preferences.end();
}
void WiFiState::save_ap_login(const char *login) {
    this->ap_login = login;
    this->_preferences.begin(NAMESPACE);
    this->_preferences.putString("ap_login", login);
    this->_preferences.end();
}
void WiFiState::save_ap_password(const char *password) {
    this->ap_password = password;
    this->_preferences.begin(NAMESPACE);
    this->_preferences.putString("ap_password", password);
    this->_preferences.end();
}
WiFiState::WiFiState(Preferences &preferences) {
    this->_preferences = preferences;
    this->_preferences.begin(NAMESPACE);
    this->ssid = this->_preferences.getString("ssid", WIFI_SSID);
    this->password = this->_preferences.getString("password", WIFI_PASSWORD);
    this->ap_login = this->_preferences.getString("ap_login", ACCESS_POINT_LOGIN);
    this->ap_password =
        this->_preferences.getString("ap_password", ACCESS_POINT_PASSWORD);
    this->ip_address = this->_preferences.getString("ip_address", "192.168.3.50");
    this->gateway_address =
        this->_preferences.getString("gateway_address", "192.168.3.1");
    this->_preferences.end();
}
WiFiState::~WiFiState() {}

WeatherState::WeatherState(Preferences &preferences) {
    this->_preferences = preferences;
    this->_preferences.begin(NAMESPACE);
    this->weather_enabled = _preferences.getBool("weather_enab", false);
    this->city = _preferences.getString("city", WEATHER_CITY);
    this->language = _preferences.getString("language", "");
    this->api_key = _preferences.getString("api_key", API_KEY);
    this->request_period =
        _preferences.getInt("request_period", WEATHER_API_POLLING_INTERVAL_MILLISECONDS);
    this->_preferences.end();
}
void WeatherState::save_weather_enabled(bool enabled) {
    this->weather_enabled = enabled;
    _preferences.begin(NAMESPACE);
    _preferences.putBool("weather_enab", enabled);
    _preferences.end();
}
void WeatherState::save_city(const char *city) {
    this->city = city;
    _preferences.begin(NAMESPACE);
    _preferences.putString("city", city);
    _preferences.end();
}
void WeatherState::save_language(const char *language) {
    this->language = language;
    _preferences.begin(NAMESPACE);
    _preferences.putString("language", language);
    _preferences.end();
}
void WeatherState::save_api_key(const char *api_key) {
    this->api_key = api_key;
    _preferences.begin(NAMESPACE);
    _preferences.putString("api_key", api_key);
    _preferences.end();
}
void WeatherState::save_request_period(int request_period) {
    this->request_period = request_period;
    _preferences.begin(NAMESPACE);
    _preferences.putInt("request_period", request_period);
    _preferences.end();
}

WeatherState::~WeatherState() {}

AlarmState::AlarmState(Preferences &preferences) {
    this->_preferences = preferences;
    this->_preferences.begin(NAMESPACE);
    this->weekdays_switch_enabled = _preferences.getBool("weekdays_sw", false);
    this->weekends_switch_enabled = _preferences.getBool("weekends_sw", false);
    this->oneOff_switch_enabled = _preferences.getBool("oneOff_sw", false);
    this->weekdays_time = _preferences.getString("weekdays_time", "08:00");
    this->weekends_time = _preferences.getString("weekends_time", "09:15");
    this->oneOff_time = _preferences.getString("oneOff_time", "13:23");
    this->_preferences.end();
}

void AlarmState::save_alarm_switches_enabled(bool weekdays_enabled, bool weekends_enabled,
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
void AlarmState::save_alarm_time(const char *weekdays_time, const char *weekends_time,
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
AlarmState::~AlarmState() {}

ThemeState::ThemeState(Preferences &preferences) {
    this->_preferences = preferences;
    this->_preferences.begin(NAMESPACE);
    this->dark_theme_enabled = _preferences.getBool("dark_theme", false);

    this->light_primary_color = _preferences.getInt("light_primary", 48340);
    this->light_second_color = _preferences.getInt("light_second", 48340);
    this->light_screen_color = _preferences.getInt("light_screen", 16448250);
    this->light_card_color = _preferences.getInt("light_card", 16777215);
    this->light_text_color = _preferences.getInt("light_text", 2171169);
    this->light_grey_color = _preferences.getInt("light_grey", 15658734);

    this->dark_primary_color = _preferences.getInt("dark_primary", 38535);
    this->dark_second_color = _preferences.getInt("dark_second", 38535);
    this->dark_screen_color = _preferences.getInt("dark_screen", 1382170);
    this->dark_card_color = _preferences.getInt("dark_card", 2632496);
    this->dark_text_color = _preferences.getInt("dark_text", 16448250);
    this->dark_grey_color = _preferences.getInt("dark_grey", 3093047);
    this->_preferences.end();
}
void ThemeState::save_dark_theme_enabled(bool enabled) {
    this->dark_theme_enabled = enabled;
    this->_preferences.begin(NAMESPACE);
    this->_preferences.putBool("dark_theme", enabled);
    this->_preferences.end();
}
void ThemeState::save_light_colors(int light_primary_color, int light_second_color,
                                   int light_screen_color, int light_card_color,
                                   int light_text_color, int light_grey_color) {
    this->light_primary_color = light_primary_color;
    this->light_second_color = light_second_color;
    this->light_screen_color = light_screen_color;
    this->light_card_color = light_card_color;
    this->light_text_color = light_text_color;
    this->light_grey_color = light_grey_color;

    _preferences.begin(NAMESPACE);
    _preferences.putInt("light_primary", light_primary_color);
    _preferences.putInt("light_second", light_second_color);
    _preferences.putInt("light_screen", light_screen_color);
    _preferences.putInt("light_card", light_card_color);
    _preferences.putInt("light_text", light_text_color);
    _preferences.putInt("light_grey", light_grey_color);
    _preferences.end();
}
void ThemeState::save_dark_colors(int dark_primary_color, int dark_second_color,
                                  int dark_screen_color, int dark_card_color,
                                  int dark_text_color, int dark_grey_color) {
    this->dark_primary_color = dark_primary_color;
    this->dark_second_color = dark_second_color;
    this->dark_screen_color = dark_screen_color;
    this->dark_card_color = dark_card_color;
    this->dark_text_color = dark_text_color;
    this->dark_grey_color = dark_grey_color;

    _preferences.begin(NAMESPACE);
    _preferences.putInt("dark_primary", dark_primary_color);
    _preferences.putInt("dark_second", dark_second_color);
    _preferences.putInt("dark_screen", dark_screen_color);
    _preferences.putInt("dark_card", dark_card_color);
    _preferences.putInt("dark_text", dark_text_color);
    _preferences.putInt("dark_grey", dark_grey_color);
    _preferences.end();
}
ThemeState::~ThemeState() {}

DisplayState::DisplayState(Preferences &preferences) {
    this->_preferences = preferences;
    this->_preferences.begin(NAMESPACE);
    this->auto_brightness = _preferences.getBool("auto_bright", false);
    this->auto_theme_change = _preferences.getBool("auto_theme", false);
    this->brightness_level = _preferences.getUInt("brightness", 255);
    this->threshold = _preferences.getInt("threshold", 120);

    this->digital_main_screen =
        _preferences.getBool("dig_main_screen", DIGITAL_CLOCK_MAIN_SCREEN);
    this->_preferences.end();
}
void DisplayState::save_auto_brightness_enabled(bool enabled) {
    this->auto_brightness = enabled;
    _preferences.begin(NAMESPACE);
    _preferences.putBool("auto_bright", enabled);
    _preferences.end();
}

void DisplayState::save_auto_theme_change(bool change) {
    this->auto_theme_change = change;
    _preferences.begin(NAMESPACE);
    _preferences.putBool("auto_theme", change);
    _preferences.end();
}
void DisplayState::save_digital_main_screen(bool digital_main_screen) {
    this->digital_main_screen = digital_main_screen;
    _preferences.begin(NAMESPACE);
    _preferences.putBool("dig_main_screen", digital_main_screen);
    _preferences.end();
}
void DisplayState::save_brightness_level(unsigned int brightness_level) {
    this->brightness_level = brightness_level;
    _preferences.begin(NAMESPACE);
    _preferences.putUInt("brightness", brightness_level);
    _preferences.end();
}

void DisplayState::save_brightness_threshold(int threshold) {
    this->threshold = threshold;
    _preferences.begin(NAMESPACE);
    _preferences.putInt("threshold", threshold);
    _preferences.end();
}
DisplayState::~DisplayState() {}