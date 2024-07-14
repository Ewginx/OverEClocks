#pragma once
#include "Apps/StateApp.h"
#include "Display/Display.h"
#include "GUI/DigitalClock.h"
#include "Preferences.h"
#include "Translation/Translation.h"

class Settings {
  public:
    StateApp *stateApp;

    lv_obj_t *settingsScreen;

    lv_obj_t *themeSwitch;

    void setBrightnessSliderValue(u_int32_t sliderValue, bool withAnimation = false);
    void setBrightnessCheckbox(bool autoBrightnessEnabled);
    void setIpAddressLabel(const char *ipAddress);
    void setWeatherSettings(const char *city, const char *language);
    void setWifiSettings(const char *ssid, const char *password);
    void setThemeSwitch(bool darkThemeEnabled);

    void updateWeatherGuiState();

    void createSettingsScreen();
    void loadSettingsScreen(lv_obj_t *screen);

    void wifiButtonEventCallback(lv_event_t *e);
    void weatherButtonEventCallback(lv_event_t *e);
    void weatherSwitchEventCallback(lv_event_t *e);
    void settingsCityTextAreaEventCallback(lv_event_t *e);
    void settingsLanguageTextAreaEventCallback(lv_event_t *e);
    void settingsSsidTextAreaEventCallback(lv_event_t *e);
    void settingsPasswordTextAreaEventCallback(lv_event_t *e);
    void settingsBrightnessSliderEventCallback(lv_event_t *e);
    void settingsAutoBrightnessCheckboxEventCallback(lv_event_t *e);
    void keyboardEventCallback(lv_event_t *e);
    void homeButtonEventCallback(lv_event_t *e);

    Settings(StateApp *stateApp);
    ~Settings();

  private:
    lv_obj_t *keyboard;

    lv_obj_t *homeScreen;

    lv_obj_t *settingsPanel;

    lv_obj_t *themeLabel;

    lv_obj_t *cityTextArea;
    lv_obj_t *languageTextArea;
    lv_obj_t *cityLabel;

    lv_obj_t *ssidLabel;
    lv_obj_t *ssidTextArea;

    lv_obj_t *passwordLabel;
    lv_obj_t *passwordTextArea;

    lv_obj_t *wifiButtonLabel;
    lv_obj_t *wifiButton;
    lv_obj_t *weatherButtonLabel;
    lv_obj_t *weatherButton;

    lv_obj_t *weatherLabel;
    lv_obj_t *weatherSwitch;

    lv_obj_t *homeButton;
    lv_obj_t *homeButtonLabel;

    lv_obj_t *ipAddressLabel;

    lv_obj_t *brightnessSlider;
    lv_obj_t *autoBrightnessCheckbox;

    int settingsPanelHeight = 450;

    void createKeyboard(lv_obj_t *target);
    void deleteKeyboard();
};
