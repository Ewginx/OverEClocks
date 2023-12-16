#pragma once
#include "Apps/StateApp.h"
#include "Display/Display.h"
#include "GUI/DigitalClock.h"
#include "Preferences.h"
#include "Translation/Translation.h"

class Settings {
  private:
    Display *_display;
    StateApp *_state_app;
    int _settings_panel_height = 450;

  public:
    lv_obj_t *keyboard;

    lv_obj_t *home_screen;

    lv_obj_t *settingsScreen;
    lv_obj_t *settingsPanel;

    lv_obj_t *themeLabel;
    lv_obj_t *themeSwitch;

    lv_obj_t *cityTextArea;
    lv_obj_t *languageTextArea;
    lv_obj_t *cityLabel;

    lv_obj_t *SSIDLabel;
    lv_obj_t *SSIDTextArea;

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

    void set_display(Display *display);

    void set_weather_settings(const char *city, const char *language);
    void set_wifi_settings(const char *ssid, const char *password);
    void set_theme_switch(bool dark_theme_enabled);
    void set_brightness_slider(u_int32_t slider_value, bool with_anim = false);
    void set_brightness_checkbox(bool auto_brightness_enabled);

    void disable_weather_controls();
    void update_weather_controls_state(bool weather_enabled);

    void create_settings_screen();
    void load_settings_screen(lv_obj_t *screen);

    void create_keyboard(lv_obj_t *target);
    void delete_keyboard();
    
    void set_ipAddressLabel(int ip0, int ip1, int ip2, int ip3);

    void wifi_button_event_cb(lv_event_t *e);
    void weather_button_event_cb(lv_event_t *e);
    void weather_switch_event_cb(lv_event_t *e);
    void settings_cityTextArea_event_cb(lv_event_t *e);
    void settings_languageTextArea_event_cb(lv_event_t *e);
    void settings_SSIDTextArea_event_cb(lv_event_t *e);
    void settings_passwordTextArea_event_cb(lv_event_t *e);
    void settings_brightnessSlider_event_cb(lv_event_t *e);
    void settings_autoBrightness_checkbox_event_cb(lv_event_t *e);
    void keyboard_event_cb(lv_event_t *e);
    void home_button_event_cb(lv_event_t *e);

    Settings(StateApp *state_app);
    ~Settings();
};
