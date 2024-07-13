#include "StateApp.h"

static StateApp *instance = NULL;

StateApp::StateApp() {
    instance = this;
    this->preferences.begin(NAMESPACE);
    wifiState = new WiFiState(preferences);
    weatherState = new WeatherState(preferences);
    alarmState = new AlarmState(preferences);
    themeState = new ThemeState(preferences);
    displayState = new DisplayState(preferences);
    timeState = new TimeState(preferences);
    soundState = new SoundState(preferences);
    rgbState = new RGBState(preferences);
    batteryState = new BatteryState();
    microclimateState = new MicroclimateState();
}
StateApp::~StateApp() {}

void WiFiState::saveSsid(const char *ssid) {
    this->ssid = ssid;
    this->preferences.putString("ssid", ssid);
}
void WiFiState::savePassword(const char *password) {
    this->password = password;
    this->preferences.putString("password", password);
}
void WiFiState::saveIpAndGatewayAddresses(const char *ipAddress,
                                              const char *gatewayAddress) {
    this->ipAddress = ipAddress;
    this->gatewayAddress = gatewayAddress;
    this->preferences.putString("ip_address", ipAddress);
    this->preferences.putString("gateway_address", gatewayAddress);
}
void WiFiState::saveApLogin(const char *login) {
    this->apLogin = login;
    this->preferences.putString("ap_login", login);
}
void WiFiState::saveApPassword(const char *password) {
    this->apPassword = password;
    this->preferences.putString("ap_password", password);
}
WiFiState::WiFiState(Preferences &preferences) : preferences(preferences) {
    this->ssid = this->preferences.getString("ssid", WIFI_SSID);
    this->password = this->preferences.getString("password", WIFI_PASSWORD);
    this->apLogin = this->preferences.getString("ap_login", ACCESS_POINT_LOGIN);
    this->apPassword = this->preferences.getString("ap_password", ACCESS_POINT_PASSWORD);
    this->ipAddress = this->preferences.getString("ip_address", "192.168.3.50");
    this->gatewayAddress = this->preferences.getString("gateway_address", "192.168.3.1");
}
WiFiState::~WiFiState() {}

WeatherState::WeatherState(Preferences &preferences) : preferences(preferences) {
    this->weatherEnabled = preferences.getBool("weather_enab", false);
    this->city = preferences.getString("city", WEATHER_CITY);
    this->language = preferences.getString("language", "");
    this->apiKey = preferences.getString("api_key", API_KEY);
    this->requestPeriod =
        preferences.getInt("request_period", WEATHER_API_POLLING_INTERVAL_MILLISECONDS);
}
void WeatherState::saveWeatherEnabled(bool weatherEnabled) {
    this->weatherEnabled = weatherEnabled;
    preferences.putBool("weather_enab", weatherEnabled);
}
void WeatherState::saveCity(const char *city) {
    this->city = city;
    preferences.putString("city", city);
}
void WeatherState::saveLanguage(const char *language) {
    this->language = language;
    preferences.putString("language", language);
}
void WeatherState::saveApiKey(const char *apiKey) {
    this->apiKey = apiKey;
    preferences.putString("api_key", apiKey);
}
void WeatherState::saveRequestPeriod(int requestPeriod) {
    this->requestPeriod = requestPeriod;
    preferences.putInt("request_period", requestPeriod);
}

WeatherState::~WeatherState() {}

AlarmState::AlarmState(Preferences &preferences) : preferences(preferences) {
    this->alarmIsRinging = false;
    this->weekdaysSwitchEnabled = preferences.getBool("weekdays_sw", false);
    this->weekendsSwitchEnabled = preferences.getBool("weekends_sw", false);
    this->oneOffSwitchEnabled = preferences.getBool("oneOff_sw", false);
    this->weekdaysTime = preferences.getString("weekdays_time", "08:00");
    this->weekendsTime = preferences.getString("weekends_time", "09:15");
    this->oneOffTime = preferences.getString("oneOff_time", "13:23");
}

void AlarmState::saveAlarmSwitchStates(bool weekdaysSwitchEnabled,
                                       bool weekendsSwitchEnabled,
                                       bool oneOffSwitchEnabled) {
    this->weekdaysSwitchEnabled = weekdaysSwitchEnabled;
    this->weekendsSwitchEnabled = weekendsSwitchEnabled;
    this->oneOffSwitchEnabled = oneOffSwitchEnabled;

    preferences.putBool("weekdays_sw", weekdaysSwitchEnabled);
    preferences.putBool("weekends_sw", weekendsSwitchEnabled);
    preferences.putBool("oneOff_sw", oneOffSwitchEnabled);
}
void AlarmState::saveAlarmTimes(const char *weekdaysTime, const char *weekendsTime,
                                const char *oneOffTime) {
    this->weekdaysTime = weekdaysTime;
    this->weekendsTime = weekendsTime;
    this->oneOffTime = oneOffTime;

    preferences.putString("weekdays_time", weekdaysTime);
    preferences.putString("weekends_time", weekendsTime);
    preferences.putString("oneOff_time", oneOffTime);
}
AlarmState::~AlarmState() {}

ThemeState::ThemeState(Preferences &preferences) : preferences(preferences) {

    this->darkThemeEnabled = preferences.getBool("dark_theme", false);
    this->currentThemeIsDark = darkThemeEnabled;
    this->lightPrimaryColor = preferences.getInt("light_primary", 48340);
    this->lightSecondColor = preferences.getInt("light_second", 48340);
    this->lightScreenColor = preferences.getInt("light_screen", 16448250);
    this->lightCardColor = preferences.getInt("light_card", 16777215);
    this->lightTextColor = preferences.getInt("light_text", 2171169);
    this->lightGreyColor = preferences.getInt("light_grey", 15658734);

    this->darkPrimaryColor = preferences.getInt("dark_primary", 38535);
    this->darkSecondColor = preferences.getInt("dark_second", 38535);
    this->darkScreenColor = preferences.getInt("dark_screen", 1382170);
    this->darkCardColor = preferences.getInt("dark_card", 2632496);
    this->darkTextColor = preferences.getInt("dark_text", 16448250);
    this->darkGreyColor = preferences.getInt("dark_grey", 3093047);
}
void ThemeState::saveDarkThemeEnabled(bool darkThemeEnabled) {
    this->darkThemeEnabled = darkThemeEnabled;
    this->preferences.putBool("dark_theme", darkThemeEnabled);
}
void ThemeState::saveLightColors(ThemeStruct lightTheme) {
    this->lightPrimaryColor = lightTheme.primaryColor;
    this->lightSecondColor = lightTheme.secondColor;
    this->lightScreenColor = lightTheme.screenColor;
    this->lightCardColor = lightTheme.cardColor;
    this->lightTextColor = lightTheme.textColor;
    this->lightGreyColor = lightTheme.greyColor;

    preferences.putInt("light_primary", lightTheme.primaryColor);
    preferences.putInt("light_second", lightTheme.secondColor);
    preferences.putInt("light_screen", lightTheme.screenColor);
    preferences.putInt("light_card", lightTheme.cardColor);
    preferences.putInt("light_text", lightTheme.textColor);
    preferences.putInt("light_grey", lightTheme.greyColor);
}
void ThemeState::saveDarkColors(ThemeStruct darkTheme) {
    this->darkPrimaryColor = darkTheme.primaryColor;
    this->darkSecondColor = darkTheme.secondColor;
    this->darkScreenColor = darkTheme.screenColor;
    this->darkCardColor = darkTheme.cardColor;
    this->darkTextColor = darkTheme.textColor;
    this->darkGreyColor = darkTheme.greyColor;

    preferences.putInt("dark_primary", darkTheme.primaryColor);
    preferences.putInt("dark_second", darkTheme.secondColor);
    preferences.putInt("dark_screen", darkTheme.screenColor);
    preferences.putInt("dark_card", darkTheme.cardColor);
    preferences.putInt("dark_text", darkTheme.textColor);
    preferences.putInt("dark_grey", darkTheme.greyColor);
}
ThemeState::~ThemeState() {}

DisplayState::DisplayState(Preferences &preferences) : preferences(preferences) {

    this->autoBrightness = preferences.getBool("auto_bright", false);
    this->autoThemeChange = preferences.getBool("auto_theme", false);
    this->brightnessLevel = preferences.getUInt("brightness", 255);
    this->threshold = preferences.getInt("threshold", 120);
    this->mainScreenIsDigital =
        preferences.getBool("dig_main_screen", DIGITAL_CLOCK_MAIN_SCREEN);
}
void DisplayState::saveAutoBrightnessEnabled(bool autoBrightness) {
    this->autoBrightness = autoBrightness;
    preferences.putBool("auto_bright", autoBrightness);
}

void DisplayState::saveAutoThemeChange(bool autoThemeChange) {
    this->autoThemeChange = autoThemeChange;
    preferences.putBool("auto_theme", autoThemeChange);
}
void DisplayState::saveMainScreenIsDigital(bool mainScreenIsDigital) {
    this->mainScreenIsDigital = mainScreenIsDigital;
    preferences.putBool("dig_main_screen", mainScreenIsDigital);
}
void DisplayState::saveBrightnessLevel(unsigned int brightnessLevel) {
    this->brightnessLevel = brightnessLevel;
    preferences.putUInt("brightness", brightnessLevel);
}

void DisplayState::saveAutoBrightnessThreshold(int threshold) {
    this->threshold = threshold;
    preferences.putInt("threshold", threshold);
}
DisplayState::~DisplayState() {}

TimeState::TimeState(Preferences &preferences) : preferences(preferences) {
    this->timezonePosix = preferences.getString("timezone", TIMEZONE);
}
void TimeState::saveTimezone(const char *timezonePosix) {
    this->timezonePosix = timezonePosix;
    this->preferences.putString("timezone", timezonePosix);
}
TimeState::~TimeState() {}

SoundState::SoundState(Preferences &preferences) : preferences(preferences) {
    this->volumeLevel = preferences.getInt("volume_level", 15);
    this->alarmTrackNumber = preferences.getInt("alarm_track", 1);
    this->eeTrackNumber = preferences.getInt("ee_track", 1);
    this->plugInTrackNumber = preferences.getInt("plug_track", 1);
    this->plugInSoundOn = preferences.getBool("plug_sound_on", true);
    this->eeSoundOn = preferences.getBool("ee_sound_on", true);
    this->soundOn = preferences.getBool("sound_on", true);
}
void SoundState::saveVolumeLevel(int volumeLevel) {
    this->volumeLevel = volumeLevel;
    this->preferences.putInt("volume_level", volumeLevel);
}
void SoundState::saveAlarmTrackNumber(int alarmTrack) {
    this->alarmTrackNumber = alarmTrack;
    this->preferences.putInt("alarm_track", alarmTrack);
}
void SoundState::saveEETrackNumber(int plugInTrackNumber) {
    this->eeTrackNumber = plugInTrackNumber;
    this->preferences.putInt("ee_track", plugInTrackNumber);
}
void SoundState::savePlugInTrackNumber(int plug_track) {
    this->plugInTrackNumber = plug_track;
    this->preferences.putInt("plug_track", plug_track);
}
void SoundState::savePlugInSoundOn(bool plugInSoundOn) {
    this->plugInSoundOn = plugInSoundOn;
    this->preferences.putBool("plug_sound_on", plugInSoundOn);
}
void SoundState::saveEESoundOn(bool eeSoundOn) {
    this->eeSoundOn = eeSoundOn;
    this->preferences.putBool("ee_sound_on", eeSoundOn);
}
void SoundState::saveSoundOn(bool soundOn) {
    this->soundOn = soundOn;
    this->preferences.putBool("sound_on", soundOn);
}
SoundState::~SoundState() {}

RGBState::RGBState(Preferences &preferences) : preferences(preferences) {
    this->firstRgbColor = preferences.getInt("first_rgb", 10814460); // 16777215 white
    this->secondRgbColor = preferences.getInt("second_rgb", 0);
    this->thirdRgbColor = preferences.getInt("third_rgb", 0);

    this->enabled = preferences.getBool("rgb_enabled", true);
    this->turnOffAtNight = preferences.getBool("rgb_night", true);
    this->delay = preferences.getInt("rgb_delay", 300);
    this->effect = preferences.getInt("rgb_effect", 2);
    this->brightness = this->preferences.getInt("rgb_bright", 255);
}

void RGBState::saveColors(int firstRgbColor, int secondRgbColor, int thirdRgbColor) {
    this->firstRgbColor = firstRgbColor;
    this->secondRgbColor = secondRgbColor;
    this->thirdRgbColor = thirdRgbColor;
    this->preferences.putInt("first_rgb", firstRgbColor);
    this->preferences.putInt("second_rgb", secondRgbColor);
    this->preferences.putInt("third_rgb", thirdRgbColor);
}

void RGBState::saveIsEnabled(bool enabled) {
    this->enabled = enabled;
    this->preferences.putBool("rgb_enabled", enabled);
}

void RGBState::saveTurnOffAtNight(bool turnOffAtNight) {
    this->turnOffAtNight = turnOffAtNight;
    this->preferences.putBool("rgb_night", turnOffAtNight);
}

void RGBState::saveEffect(int effect) {
    this->effect = effect;
    this->preferences.putInt("rgb_effect", effect);
}

void RGBState::saveDelay(int delay) {
    this->delay = delay;
    this->preferences.putInt("rgb_delay", delay);
}

void RGBState::saveRgbBrightness(int brightness) {
    this->brightness = brightness;
    this->preferences.putInt("rgb_bright", brightness);
}

RGBState::~RGBState() {}

BatteryState::BatteryState() {}

BatteryState::~BatteryState() {}
