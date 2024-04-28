#include "StateApp.h"

static StateApp *instance = NULL;

StateApp::StateApp() {
    instance = this;
    this->_preferences.begin(NAMESPACE);
    wifi_state = new WiFiState(_preferences);
    weather_state = new WeatherState(_preferences);
    alarm_state = new AlarmState(_preferences);
    theme_state = new ThemeState(_preferences);
    display_state = new DisplayState(_preferences);
    time_state = new TimeState(_preferences);
    sound_state = new SoundState(_preferences);
    rgb_state = new RGBState(_preferences);
    battery_state = new BatteryState();
}
StateApp::~StateApp() {}

void WiFiState::save_ssid(const char *ssid) {
    this->ssid = ssid;
    this->_preferences.putString("ssid", ssid);
}
void WiFiState::save_password(const char *password) {
    this->password = password;
    this->_preferences.putString("password", password);
}
void WiFiState::save_ip_and_gateway_addresses(const char *ip_address,
                                              const char *gateway_address) {
    this->ip_address = ip_address;
    this->gateway_address = gateway_address;
    this->_preferences.putString("ip_address", ip_address);
    this->_preferences.putString("gateway_address", gateway_address);
}
void WiFiState::save_ap_login(const char *login) {
    this->ap_login = login;
    this->_preferences.putString("ap_login", login);
}
void WiFiState::save_ap_password(const char *password) {
    this->ap_password = password;
    this->_preferences.putString("ap_password", password);
}
WiFiState::WiFiState(Preferences &preferences) : _preferences(preferences) {
    this->ssid = this->_preferences.getString("ssid", WIFI_SSID);
    this->password = this->_preferences.getString("password", WIFI_PASSWORD);
    this->ap_login = this->_preferences.getString("ap_login", ACCESS_POINT_LOGIN);
    this->ap_password =
        this->_preferences.getString("ap_password", ACCESS_POINT_PASSWORD);
    this->ip_address = this->_preferences.getString("ip_address", "192.168.3.50");
    this->gateway_address =
        this->_preferences.getString("gateway_address", "192.168.3.1");
}
WiFiState::~WiFiState() {}

WeatherState::WeatherState(Preferences &preferences) : _preferences(preferences) {
    this->weather_enabled = _preferences.getBool("weather_enab", false);
    this->city = _preferences.getString("city", WEATHER_CITY);
    this->language = _preferences.getString("language", "");
    this->api_key = _preferences.getString("api_key", API_KEY);
    this->request_period =
        _preferences.getInt("request_period", WEATHER_API_POLLING_INTERVAL_MILLISECONDS);
}
void WeatherState::save_weather_enabled(bool enabled) {
    this->weather_enabled = enabled;
    _preferences.putBool("weather_enab", enabled);
}
void WeatherState::save_city(const char *city) {
    this->city = city;
    _preferences.putString("city", city);
}
void WeatherState::save_language(const char *language) {
    this->language = language;
    _preferences.putString("language", language);
}
void WeatherState::save_api_key(const char *api_key) {
    this->api_key = api_key;
    _preferences.putString("api_key", api_key);
}
void WeatherState::save_request_period(int request_period) {
    this->request_period = request_period;
    _preferences.putInt("request_period", request_period);
}

WeatherState::~WeatherState() {}

AlarmState::AlarmState(Preferences &preferences) : _preferences(preferences) {
    this->alarm_ringing = false;
    this->weekdays_switch_enabled = _preferences.getBool("weekdays_sw", false);
    this->weekends_switch_enabled = _preferences.getBool("weekends_sw", false);
    this->oneOff_switch_enabled = _preferences.getBool("oneOff_sw", false);
    this->weekdays_time = _preferences.getString("weekdays_time", "08:00");
    this->weekends_time = _preferences.getString("weekends_time", "09:15");
    this->oneOff_time = _preferences.getString("oneOff_time", "13:23");
}

void AlarmState::save_alarm_switches_enabled(bool weekdays_enabled, bool weekends_enabled,
                                             bool oneOff_enabled) {
    this->weekdays_switch_enabled = weekdays_enabled;
    this->weekends_switch_enabled = weekends_enabled;
    this->oneOff_switch_enabled = oneOff_enabled;

    _preferences.putBool("weekdays_sw", weekdays_enabled);
    _preferences.putBool("weekends_sw", weekends_enabled);
    _preferences.putBool("oneOff_sw", oneOff_enabled);
}
void AlarmState::save_alarm_time(const char *weekdays_time, const char *weekends_time,
                                 const char *oneOff_time) {
    this->weekdays_time = weekdays_time;
    this->weekends_time = weekends_time;
    this->oneOff_time = oneOff_time;

    _preferences.putString("weekdays_time", weekdays_time);
    _preferences.putString("weekends_time", weekends_time);
    _preferences.putString("oneOff_time", oneOff_time);
}
AlarmState::~AlarmState() {}

ThemeState::ThemeState(Preferences &preferences) : _preferences(preferences) {

    this->dark_theme_enabled = _preferences.getBool("dark_theme", false);
    this->current_theme_is_dark = dark_theme_enabled;
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
}
void ThemeState::save_dark_theme_enabled(bool enabled) {
    this->dark_theme_enabled = enabled;
    this->_preferences.putBool("dark_theme", enabled);
}
void ThemeState::save_light_colors(ThemeStruct light_theme) {
    this->light_primary_color = light_theme.primary_color;
    this->light_second_color = light_theme.second_color;
    this->light_screen_color = light_theme.screen_color;
    this->light_card_color = light_theme.card_color;
    this->light_text_color = light_theme.text_color;
    this->light_grey_color = light_theme.grey_color;

    _preferences.putInt("light_primary", light_theme.primary_color);
    _preferences.putInt("light_second", light_theme.second_color);
    _preferences.putInt("light_screen", light_theme.screen_color);
    _preferences.putInt("light_card", light_theme.card_color);
    _preferences.putInt("light_text", light_theme.text_color);
    _preferences.putInt("light_grey", light_theme.grey_color);
}
void ThemeState::save_dark_colors(ThemeStruct dark_theme) {
    this->dark_primary_color = dark_theme.primary_color;
    this->dark_second_color = dark_theme.second_color;
    this->dark_screen_color = dark_theme.screen_color;
    this->dark_card_color = dark_theme.card_color;
    this->dark_text_color = dark_theme.text_color;
    this->dark_grey_color = dark_theme.grey_color;

    _preferences.putInt("dark_primary", dark_theme.primary_color);
    _preferences.putInt("dark_second", dark_theme.second_color);
    _preferences.putInt("dark_screen", dark_theme.screen_color);
    _preferences.putInt("dark_card", dark_theme.card_color);
    _preferences.putInt("dark_text", dark_theme.text_color);
    _preferences.putInt("dark_grey", dark_theme.grey_color);
}
ThemeState::~ThemeState() {}

DisplayState::DisplayState(Preferences &preferences) : _preferences(preferences) {

    this->auto_brightness = _preferences.getBool("auto_bright", false);
    this->auto_theme_change = _preferences.getBool("auto_theme", false);
    this->brightness_level = _preferences.getUInt("brightness", 255);
    this->threshold = _preferences.getInt("threshold", 120);
    this->digital_main_screen =
        _preferences.getBool("dig_main_screen", DIGITAL_CLOCK_MAIN_SCREEN);
}
void DisplayState::save_auto_brightness_enabled(bool enabled) {
    this->auto_brightness = enabled;
    _preferences.putBool("auto_bright", enabled);
}

void DisplayState::save_auto_theme_change(bool change) {
    this->auto_theme_change = change;
    _preferences.putBool("auto_theme", change);
}
void DisplayState::save_digital_main_screen(bool digital_main_screen) {
    this->digital_main_screen = digital_main_screen;
    _preferences.putBool("dig_main_screen", digital_main_screen);
}
void DisplayState::save_brightness_level(unsigned int brightness_level) {
    this->brightness_level = brightness_level;
    _preferences.putUInt("brightness", brightness_level);
}

void DisplayState::save_brightness_threshold(int threshold) {
    this->threshold = threshold;
    _preferences.putInt("threshold", threshold);
}
DisplayState::~DisplayState() {}

TimeState::TimeState(Preferences &preferences) : _preferences(preferences) {
    this->timezone_posix = _preferences.getString("timezone", TIMEZONE);
}
void TimeState::save_timezone(const char *timezone_posix) {
    this->timezone_posix = timezone_posix;
    this->_preferences.putString("timezone", timezone_posix);
}
TimeState::~TimeState() {}

SoundState::SoundState(Preferences &preferences) : _preferences(preferences) {
    this->volume_level = _preferences.getInt("volume_level", 15);
    this->alarm_track = _preferences.getInt("alarm_track", 1);
    this->ee_track = _preferences.getInt("ee_track", 1);
    this->plug_track = _preferences.getInt("plug_track", 1);
    this->plug_sound_on = _preferences.getBool("plug_sound_on", true);
    this->ee_sound_on = _preferences.getBool("ee_sound_on", true);
    this->sound_on = _preferences.getBool("sound_on", true);
}
void SoundState::save_volume_level(short int volume_level) {
    this->volume_level = volume_level;
    this->_preferences.putInt("volume_level", volume_level);
}
void SoundState::save_alarm_track(short int alarm_track) {
    this->alarm_track = alarm_track;
    this->_preferences.putInt("alarm_track", alarm_track);
}
void SoundState::save_ee_track(short int ee_track) {
    this->ee_track = ee_track;
    this->_preferences.putInt("ee_track", ee_track);
}
void SoundState::save_plug_track(int plug_track) {
    this->plug_track = plug_track;
    this->_preferences.putInt("plug_track", plug_track);
}
void SoundState::save_plug_sound_enabled(bool plug_sound_on) {
    this->plug_sound_on = plug_sound_on;
    this->_preferences.putBool("plug_sound_on", plug_sound_on);
}
void SoundState::save_ee_sound_enabled(bool ee_sound_on) {
    this->ee_sound_on = ee_sound_on;
    this->_preferences.putBool("ee_sound_on", ee_sound_on);
}
void SoundState::save_sound_on(bool sound_on) {
    this->sound_on = sound_on;
    this->_preferences.putBool("sound_on", sound_on);
}
SoundState::~SoundState() {}

RGBState::RGBState(Preferences &preferences) : _preferences(preferences) {
    this->first_rgb_color = _preferences.getInt("first_rgb", 10814460); // 16777215 white
    this->second_rgb_color = _preferences.getInt("second_rgb", 0);
    this->third_rgb_color = _preferences.getInt("third_rgb", 0);

    this->enabled = _preferences.getBool("rgb_enabled", true);
    this->turn_off_at_night = _preferences.getBool("rgb_night", true);
    this->delay = _preferences.getInt("rgb_delay", 300);
    this->effect = _preferences.getInt("rgb_effect", 2);
    this->brightness = this->_preferences.getInt("rgb_bright", 255);
}
void RGBState::save_rgb_color(int first_rgb_color, int second_rgb_color,
                              int third_rgb_color) {
    this->first_rgb_color = first_rgb_color;
    this->second_rgb_color = second_rgb_color;
    this->third_rgb_color = third_rgb_color;
    this->_preferences.putInt("first_rgb", first_rgb_color);
    this->_preferences.putInt("second_rgb", second_rgb_color);
    this->_preferences.putInt("third_rgb", third_rgb_color);
}
void RGBState::save_rgb_enabled(bool enabled) {
    this->enabled = enabled;
    this->_preferences.putBool("rgb_enabled", enabled);
}
void RGBState::save_rgb_night(bool turn_off_at_night) {
    this->turn_off_at_night = turn_off_at_night;
    this->_preferences.putBool("rgb_night", turn_off_at_night);
}
void RGBState::save_rgb_effect(short int effect) {
    this->effect = effect;
    this->_preferences.putInt("rgb_effect", effect);
}
void RGBState::save_rgb_delay(int delay) {
    this->delay = delay;
    this->_preferences.putInt("rgb_delay", delay);
}
void RGBState::save_brightness(int brightness) {
    this->brightness = brightness;
    this->_preferences.putInt("rgb_bright", brightness);
}
RGBState::~RGBState() {}

BatteryState::BatteryState() {}

BatteryState::~BatteryState() {}
