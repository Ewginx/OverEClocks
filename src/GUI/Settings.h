#pragma once
#include "Preferences.h"
#include "Display/Display.h"
#include "GUI/DigitalClock.h"
#include "Translation/Translation.h"

class Settings
{
private:
    Display *_display;
    Preferences _preferences;
    int settings_panel_height = 640;

public:
    lv_obj_t *ui_SettingsKeyboard;

    lv_obj_t *home_screen;

    lv_obj_t *ui_SettingsScreen;
    lv_obj_t *ui_SettingsPanel;
    lv_obj_t *ui_DarkmodeLabel;
    lv_obj_t *ui_DarkmodeSwitch;
    lv_obj_t *ui_SettingsCityEdit;
    lv_obj_t *ui_SettingsCityLabel;
    lv_obj_t *ui_SettingsSSIDLabel;
    lv_obj_t *ui_SettingsSSIDEdit;
    lv_obj_t *ui_SettingsPasswordLabel;
    lv_obj_t *ui_SettingsPasswordEdit;
    lv_obj_t *ui_SettingsHomeButton;
    lv_obj_t *ui_SettingsHomeButtonLabel;
    lv_obj_t *ui_SettingsAPLabel;
    lv_obj_t *ui_SettingsIPLabel;

    lv_obj_t *brightnessSlider;
    lv_obj_t *autoBrightnessCheckbox;

    void set_display(Display *display);
    void set_preferences(Preferences &preferences);

    void init_settings_screen();
    void load_settings_screen(lv_obj_t *screen);
    
    void create_keyboard(lv_obj_t *target);
    void delete_keyboard();

    void settings_city_edit_event_cb(lv_event_t *e);
    void darkmode_switch_event_cb(lv_event_t *e);
    void settings_SSID_edit_event_cb(lv_event_t *e);
    void settings_password_edit_event_cb(lv_event_t *e);
    void settings_brightnessSlider_event_cb(lv_event_t *e);
    void settings_autoBrightness_checkbox_event_cb(lv_event_t *e);
    void keyboard_event_cb(lv_event_t *e);
    void home_button_event_cb(lv_event_t *e);

    Settings();
    ~Settings();
};


