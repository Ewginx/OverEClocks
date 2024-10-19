#pragma once
#include "Config/Config.h"
#include <Arduino.h>
#include <Preferences.h>

class WiFiState {
  private:
    Preferences &preferences;

  public:
    bool wifiIsConnected = false;

    String ssid;
    String password;
    String apLogin;
    String apPassword;
    String ipAddress;
    String gatewayAddress;
    String currentIpAddress;

    void saveSsid(const char *ssid);
    void savePassword(const char *password);
    void saveApLogin(const char *login);
    void saveApPassword(const char *password);
    void saveIpAndGatewayAddresses(const char *ipAddress,
                                       const char *gatewayAddress);

    WiFiState(Preferences &preferences);
    ~WiFiState();
};

class WeatherState {
  private:
    Preferences &preferences;

  public:
    bool weatherEnabled;

    String city;
    String cityEncoded;
    String language;
    String apiKey;
    int requestPeriod;

    void saveCity(const char *city);
    void saveLanguage(const char *language);
    void saveApiKey(const char *apiKey);
    void saveRequestPeriod(int requestPeriod);
    void saveWeatherEnabled(bool weatherEnabled);

    WeatherState(Preferences &preferences);
    ~WeatherState();
};

class AlarmState {
  private:
    Preferences &preferences;

  public:
    bool weekdaysSwitchEnabled;
    bool weekendsSwitchEnabled;
    bool oneOffSwitchEnabled;
    bool alarmIsRinging;

    String weekdaysTime;
    String weekendsTime;
    String oneOffTime;

    void saveAlarmSwitchStates(bool weekdaysSwitchEnabled, bool weekendsSwitchEnabled,
                               bool oneOffSwitchEnabled);
    void saveAlarmTimes(const char *weekdaysTime, const char *weekendsTime,
                        const char *oneOffTime);

    AlarmState(Preferences &preferences);
    ~AlarmState();
};

struct ThemeStruct {
    int primaryColor;
    int secondColor;
    int screenColor;
    int cardColor;
    int textColor;
    int greyColor;
};

class ThemeState {
  private:
    Preferences &preferences;

  public:
    bool currentThemeIsDark = false;

    bool darkThemeEnabled;

    int lightPrimaryColor;
    int lightSecondColor;
    int lightScreenColor;
    int lightCardColor;
    int lightTextColor;
    int lightGreyColor;

    int darkPrimaryColor;
    int darkSecondColor;
    int darkScreenColor;
    int darkCardColor;
    int darkTextColor;
    int darkGreyColor;

    void saveDarkThemeEnabled(bool darkThemeEnabled);
    void saveLightColors(ThemeStruct lightTheme);
    void saveDarkColors(ThemeStruct darkTheme);

    ThemeState(Preferences &preferences);
    ~ThemeState();
};

class DisplayState {
  private:
    Preferences &preferences;

  public:
    bool autoBrightness;
    unsigned int brightnessLevel;
    bool autoThemeChange;
    int threshold;

    bool mainScreenIsDigital;

    void saveAutoBrightnessEnabled(bool autoBrightness);
    void saveBrightnessLevel(unsigned int brightnessLevel);
    void saveAutoBrightnessThreshold(int threshold);
    void saveAutoThemeChange(bool autoThemeChange);
    void saveMainScreenIsDigital(bool mainScreenIsDigital);

    DisplayState(Preferences &preferences);
    ~DisplayState();
};

class TimeState {
  private:
    Preferences &preferences;

  public:
    String timezonePosix;
    bool isNight = false;
    bool timeIsSet = false;

    void saveTimezone(const char *timezonePosix);

    TimeState(Preferences &preferences);
    ~TimeState();
};

class SoundState {
  private:
    Preferences &preferences;

  public:
    bool soundOn;
    bool plugInSoundOn;
    bool eeSoundOn;

    bool enablePlayerUsb = false;

    int alarmTrackNumber;
    int eeTrackNumber;
    int plugInTrackNumber;

    int volumeLevel;

    void saveVolumeLevel(int volumeLevel);
    void saveAlarmTrackNumber(int alarmTrack);
    void saveEETrackNumber(int eeTrackNumber);
    void savePlugInTrackNumber(int plugInTrackNumber);
    void savePlugInSoundOn(bool plugInSoundOn);
    void saveEESoundOn(bool eeSoundOn);
    void saveSoundOn(bool soundOn);

    SoundState(Preferences &preferences);
    ~SoundState();
};

class RGBState {
  private:
    Preferences &preferences;

  public:
    bool enabled;
    bool turnOffAtNight;

    int effect;
    int brightness;
    int delay;

    int firstRgbColor;
    int secondRgbColor;
    int thirdRgbColor;

    void saveColors(int firstRgbColor, int secondRgbColor, int thirdRgbColor);
    void saveIsEnabled(bool enabled);
    void saveTurnOffAtNight(bool turnOffAtNight);
    void saveEffect(int effect);
    void saveDelay(int delay);
    void saveRgbBrightness(int brightness);

    RGBState(Preferences &preferences);
    ~RGBState();
};

class BatteryState {

  public:
    bool batteryCharging = false;
    int batteryLevel = 0;
    float batteryVoltage = 0.0f;

    BatteryState();
    ~BatteryState();
};

class MicroclimateState {
  public:
    float indoorTemperature = 0.0f;
    unsigned int indoorHumidity = 0;
};

class StateApp {
  private:
    Preferences preferences;

  public:
    WiFiState *wifiState;
    WeatherState *weatherState;
    AlarmState *alarmState;
    ThemeState *themeState;
    DisplayState *displayState;
    TimeState *timeState;
    SoundState *soundState;
    RGBState *rgbState;
    BatteryState *batteryState;
    MicroclimateState *microclimateState;

    StateApp();
    ~StateApp();
};
