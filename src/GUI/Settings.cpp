#include "GUI/Settings.h"
#include "Settings.h"

static Settings *instance = NULL;

extern "C" void keyboard_event_cb_wrapper(lv_event_t *e) {
    instance->keyboard_event_cb(e);
}
extern "C" void home_button_event_cb_wrapper(lv_event_t *e) {
    instance->home_button_event_cb(e);
}

extern "C" void settings_cityTextArea_event_cb_wrapper(lv_event_t *e) {
    instance->settings_cityTextArea_event_cb(e);
}

extern "C" void settings_SSIDTextArea_event_cb_wrapper(lv_event_t *e) {
    instance->settings_SSIDTextArea_event_cb(e);
}

extern "C" void settings_passwordTextArea_event_cb_wrapper(lv_event_t *e) {
    instance->settings_passwordTextArea_event_cb(e);
}

extern "C" void settings_brightnessSlider_event_cb_wrapper(lv_event_t *e) {
    instance->settings_brightnessSlider_event_cb(e);
}
extern "C" void settings_autoBrightness_checkbox_event_cb_wrapper(lv_event_t *e) {
    instance->settings_autoBrightness_checkbox_event_cb(e);
}
extern "C" void weather_switch_event_cb_wrapper(lv_event_t *e) {
    instance->weather_switch_event_cb(e);
}
extern "C" void wifi_button_event_cb_wrapper(lv_event_t *e) {
    instance->wifi_button_event_cb(e);
}

Settings::Settings() {
    instance = this;
    this->create_settings_screen();
}

void Settings::load_settings_screen(lv_obj_t *screen) {
    lv_theme_t *theme = lv_disp_get_theme(NULL);
    if (theme->flags) {
        lv_obj_add_state(this->darkmodeSwitch, LV_STATE_CHECKED);
    }
    lv_slider_set_value(this->brightnessSlider, this->_display->get_brightness(),
                        LV_ANIM_OFF);
    this->home_screen = screen;
    lv_scr_load_anim(this->settingsScreen, LV_SCR_LOAD_ANIM_FADE_ON,
                     SCREEN_CHANGE_ANIM_TIME, 0, false);
}

void Settings::create_keyboard(lv_obj_t *target) {
    if (this->keyboard == NULL) {
        this->keyboard = lv_keyboard_create(this->settingsScreen);
        lv_obj_set_size(this->keyboard, lv_pct(100), 130);
        lv_obj_set_pos(this->keyboard, 0, 0);
        lv_obj_set_align(this->keyboard, LV_ALIGN_BOTTOM_LEFT);
        lv_keyboard_set_textarea(this->keyboard, target);
        lv_obj_add_event_cb(this->keyboard, keyboard_event_cb_wrapper, LV_EVENT_CANCEL,
                            NULL);
    } else {
        lv_keyboard_set_textarea(this->keyboard, target);
    }
}

void Settings::delete_keyboard() {
    if (this->keyboard != NULL) {
        lv_keyboard_set_textarea(this->keyboard, NULL);
        lv_obj_remove_event_cb(this->keyboard, keyboard_event_cb_wrapper);
        lv_obj_del(this->keyboard);
        lv_obj_set_height(this->settingsPanel, this->_settings_panel_height);
        this->keyboard = NULL;
    }
}

void Settings::save_darkmode_to_nvs() {
    this->_preferences.begin(NAMESPACE);
    this->_preferences.putBool(
        "dark_theme",
        lv_obj_has_state(this->darkmodeSwitch, LV_STATE_CHECKED));
    this->_preferences.end();
}

void Settings::set_ipAddressLabel(int ip0, int ip1, int ip2, int ip3) {
    lv_label_set_text_fmt(this->ipAddressLabel, "%s %d.%d.%d.%d",
                          settings_translation[access_point_ip], ip0, ip1, ip2, ip3);
}
void Settings::wifi_button_event_cb(lv_event_t *e) {
    lv_msg_send(MSG_WIFI_RECONNECT, NULL);
}
void Settings::weather_switch_event_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_VALUE_CHANGED) {
        bool enabled = lv_obj_has_state(target, LV_STATE_CHECKED);
        this->_preferences.begin(NAMESPACE);
        this->_preferences.putBool("weather_enab", enabled);
        this->_preferences.end();
        lv_msg_send(MSG_WEATHER_ENABLED, static_cast<const void *>(&enabled));
    }
}
void Settings::keyboard_event_cb(lv_event_t *e) { this->delete_keyboard(); }

void Settings::home_button_event_cb(lv_event_t *e) {
    lv_obj_t *target = lv_event_get_target(e);
    lv_event_send(this->keyboard, LV_EVENT_CANCEL, NULL);
    lv_scr_load_anim(this->home_screen, LV_SCR_LOAD_ANIM_FADE_ON, SCREEN_CHANGE_ANIM_TIME,
                     0, false);
}

void Settings::settings_cityTextArea_event_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_FOCUSED || event_code == LV_EVENT_CLICKED) {
        lv_obj_set_height(this->settingsPanel, this->_settings_panel_height / 2);
        lv_obj_readjust_scroll(this->settingsScreen, LV_ANIM_OFF);
        lv_obj_scroll_to_y(this->settingsPanel,
                           lv_obj_get_y(lv_event_get_current_target(e)) - 80, LV_ANIM_ON);
        this->create_keyboard(lv_event_get_current_target(e));
    }
    if (event_code == LV_EVENT_READY) {
        this->delete_keyboard();
        _preferences.begin(NAMESPACE);
        _preferences.putString("city", lv_textarea_get_text(this->cityTextArea));
        _preferences.end();
        lv_msg_send(MSG_WEATHER_SETTINGS_CHANGED, NULL);
    }
}

void Settings::settings_SSIDTextArea_event_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_FOCUSED || event_code == LV_EVENT_CLICKED) {
        lv_obj_set_height(this->settingsPanel, this->_settings_panel_height / 2);
        lv_obj_readjust_scroll(this->settingsScreen, LV_ANIM_OFF);
        lv_obj_scroll_to_y(this->settingsPanel,
                           lv_obj_get_y(lv_event_get_current_target(e)) - 80, LV_ANIM_ON);
        this->create_keyboard(lv_event_get_current_target(e));
    }
    if (event_code == LV_EVENT_READY) {
        this->delete_keyboard();
        _preferences.begin(NAMESPACE);
        _preferences.putString("ssid", lv_textarea_get_text(SSIDTextArea));
        _preferences.end();
    }
}

void Settings::settings_passwordTextArea_event_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_FOCUSED || event_code == LV_EVENT_CLICKED) {
        lv_obj_set_height(this->settingsPanel, this->_settings_panel_height / 2);
        lv_obj_readjust_scroll(this->settingsScreen, LV_ANIM_OFF);
        lv_obj_scroll_to_y(this->settingsPanel,
                           lv_obj_get_y(lv_event_get_current_target(e)) - 80, LV_ANIM_ON);
        this->create_keyboard(lv_event_get_current_target(e));
    }
    if (event_code == LV_EVENT_READY) {
        this->delete_keyboard();
        _preferences.begin(NAMESPACE);
        _preferences.putString("password", lv_textarea_get_text(passwordTextArea));
        _preferences.end();
    }
}
void Settings::settings_brightnessSlider_event_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    this->_display->set_brightness((uint8_t)lv_slider_get_value(this->brightnessSlider));
    _preferences.begin(NAMESPACE);
    _preferences.putUInt("brightness", lv_slider_get_value(this->brightnessSlider));
    _preferences.end();
}
void Settings::settings_autoBrightness_checkbox_event_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    bool checked =
        lv_obj_has_state(this->autoBrightnessCheckbox, LV_STATE_CHECKED);
    _preferences.begin(NAMESPACE);
    _preferences.putBool(
        "auto_bright", checked);
    _preferences.end();
    lv_msg_send(MSG_AUTO_BRIGHTNESS, static_cast<const void *>(&checked));
}
void Settings::set_display(Display *display) { _display = display; }
void Settings::set_preferences(Preferences &preferences) { _preferences = preferences; }
void Settings::set_weather_settings(const char *city, bool weather_enabled) {
    lv_textarea_add_text(this->cityTextArea, city);
    lv_obj_add_state(this->weatherSwitch,
                     weather_enabled ? LV_STATE_CHECKED : LV_STATE_DEFAULT);
}
void Settings::set_wifi_settings(const char *ssid, const char *password) {
    lv_textarea_add_text(this->SSIDTextArea, ssid);
    lv_textarea_add_text(this->passwordTextArea, password);
}
void Settings::set_darktheme_switch(bool dark_theme_enabled) {
    lv_obj_add_state(this->darkmodeSwitch,
                     dark_theme_enabled ? LV_STATE_CHECKED : LV_STATE_DEFAULT);
}
void Settings::set_brightness_widgets(u_int32_t slider_value,
                                      bool auto_brightness_enabled) {
    lv_slider_set_value(this->brightnessSlider, slider_value, LV_ANIM_OFF);
    lv_obj_add_state(this->autoBrightnessCheckbox,
                     auto_brightness_enabled ? LV_STATE_CHECKED : LV_STATE_DEFAULT);
}
void Settings::disable_weather_switch() {
    lv_obj_clear_state(this->weatherSwitch, LV_STATE_CHECKED);
    lv_obj_add_state(this->weatherSwitch, LV_STATE_DISABLED);
}
void Settings::update_weather_switch_state(bool weather_enabled) {
    lv_obj_add_state(this->weatherSwitch,
                     weather_enabled ? LV_STATE_CHECKED : LV_STATE_DEFAULT);
}
void Settings::create_settings_screen() {
    this->keyboard = NULL;
    this->settingsScreen = lv_obj_create(NULL);
    lv_obj_add_flag(this->settingsScreen, LV_OBJ_FLAG_IGNORE_LAYOUT); /// Flags
    lv_obj_set_scroll_dir(this->settingsScreen, LV_DIR_VER);
    lv_obj_clear_flag(this->settingsScreen, LV_OBJ_FLAG_SCROLL_ELASTIC);

    this->settingsPanel = lv_obj_create(this->settingsScreen);
    lv_obj_set_size(this->settingsPanel, lv_pct(100), this->_settings_panel_height);
    lv_obj_set_style_bg_color(this->settingsPanel, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(this->settingsPanel, 0, 0);
    lv_obj_clear_flag(this->settingsPanel, LV_OBJ_FLAG_SCROLL_ELASTIC);

    this->darkmodeLabel = lv_label_create(this->settingsPanel);
    lv_obj_set_pos(this->darkmodeLabel, 35, 15);
    lv_obj_set_align(this->darkmodeLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(this->darkmodeLabel, settings_translation[theme]);
    lv_obj_set_style_text_font(this->darkmodeLabel, &font_18, LV_PART_MAIN);

    this->darkmodeSwitch = lv_switch_create(this->settingsPanel);
    lv_obj_set_size(this->darkmodeSwitch, 50, 25);
    lv_obj_set_pos(this->darkmodeSwitch, 175, 15);
    lv_obj_set_align(this->darkmodeSwitch, LV_ALIGN_TOP_LEFT);

    this->brightnessSlider = lv_slider_create(this->settingsPanel);
    lv_obj_align_to(this->brightnessSlider, this->darkmodeLabel, LV_ALIGN_OUT_BOTTOM_LEFT,
                    10, 30);
    lv_slider_set_range(this->brightnessSlider, 5, 255);

    this->autoBrightnessCheckbox = lv_checkbox_create(this->settingsPanel);
    lv_obj_align_to(this->autoBrightnessCheckbox, this->brightnessSlider,
                    LV_ALIGN_OUT_RIGHT_MID, 50, 0);
    lv_checkbox_set_text(this->autoBrightnessCheckbox,
                         settings_translation[auto_brightness]);
    lv_obj_set_style_text_font(this->autoBrightnessCheckbox, &font_18, 0);
    lv_obj_set_style_text_font(this->autoBrightnessCheckbox, &font_18,
                               LV_PART_INDICATOR | LV_STATE_CHECKED);

    this->cityLabel = lv_label_create(this->settingsPanel);
    lv_obj_set_size(this->cityLabel, 120, 45);
    lv_obj_set_pos(this->cityLabel, 20, this->_settings_panel_height / 5 + 25);
    lv_obj_set_align(this->cityLabel, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_text_align(this->cityLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(this->cityLabel, settings_translation[city]);
    lv_obj_set_style_text_font(this->cityLabel, &font_18, 0);
    lv_obj_set_style_text_align(this->cityLabel, LV_TEXT_ALIGN_CENTER, 0);

    this->cityTextArea = lv_textarea_create(this->settingsPanel);
    lv_obj_set_size(this->cityTextArea, 250, LV_SIZE_CONTENT); /// 33
    lv_obj_align_to(this->cityTextArea, this->cityLabel, LV_ALIGN_OUT_RIGHT_MID, 30, 0);
    lv_textarea_set_max_length(this->cityTextArea, 100);
    lv_textarea_set_placeholder_text(this->cityTextArea,
                                     settings_translation[city_placeholder]);
    lv_textarea_set_one_line(this->cityTextArea, true);

    this->SSIDLabel = lv_label_create(this->settingsPanel);
    lv_obj_set_size(this->SSIDLabel, 120, 45);
    lv_obj_set_align(this->SSIDLabel, LV_ALIGN_TOP_LEFT);
    lv_obj_align_to(this->SSIDLabel, this->cityLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 25);
    lv_label_set_text(this->SSIDLabel, settings_translation[wifi_ssid]);
    lv_obj_set_style_text_align(this->SSIDLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(this->SSIDLabel, &font_18, 0);

    this->SSIDTextArea = lv_textarea_create(this->settingsPanel);
    lv_obj_set_size(this->SSIDTextArea, 250, LV_SIZE_CONTENT);
    lv_obj_align_to(this->SSIDTextArea, this->SSIDLabel, LV_ALIGN_OUT_RIGHT_MID, 30, -10);
    lv_textarea_set_max_length(this->SSIDTextArea, 50);
    lv_textarea_set_placeholder_text(this->SSIDTextArea,
                                     settings_translation[wifi_ssid_placeholder]);
    lv_textarea_set_one_line(this->SSIDTextArea, true);

    this->passwordLabel = lv_label_create(this->settingsPanel);
    lv_obj_set_size(this->passwordLabel, 120, 45);
    lv_obj_align_to(this->passwordLabel, this->SSIDLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0,
                    15);
    lv_label_set_text(this->passwordLabel, settings_translation[wifi_password]);
    lv_obj_set_style_text_align(this->passwordLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(this->passwordLabel, &font_18, 0);

    this->passwordTextArea = lv_textarea_create(this->settingsPanel);
    lv_obj_set_size(this->passwordTextArea, 250, LV_SIZE_CONTENT); /// 33
    lv_obj_align_to(this->passwordTextArea, this->passwordLabel, LV_ALIGN_OUT_RIGHT_MID,
                    30, -10);
    lv_textarea_set_max_length(this->passwordTextArea, 100);
    lv_textarea_set_placeholder_text(this->passwordTextArea,
                                     settings_translation[wifi_password_placeholder]);
    lv_textarea_set_one_line(this->passwordTextArea, true);
    lv_textarea_set_password_mode(this->passwordTextArea, true);

    this->weatherLabel = lv_label_create(this->settingsPanel);
    lv_obj_align_to(this->weatherLabel, this->passwordLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0,
                    10);
    lv_label_set_text(this->weatherLabel, settings_translation[weather_enabled]);
    lv_obj_set_style_text_font(this->weatherLabel, &font_18, 0);

    this->weatherSwitch = lv_switch_create(this->settingsPanel);
    lv_obj_set_size(this->weatherSwitch, 50, 25);
    lv_obj_align_to(this->weatherSwitch, this->weatherLabel, LV_ALIGN_OUT_RIGHT_MID, 30,
                    0);

    this->wifiButton = lv_btn_create(this->settingsPanel);
    lv_obj_set_size(this->wifiButton, 240, 35);
    lv_obj_align_to(this->wifiButton, this->weatherLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0,
                    15);

    this->wifiButtonLabel = lv_label_create(this->wifiButton);
    lv_obj_set_align(this->wifiButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(this->wifiButtonLabel, settings_translation[reconnect_wifi]);
    lv_obj_set_style_text_font(this->wifiButtonLabel, &font_18, 0);

    this->ipAddressLabel = lv_label_create(this->settingsPanel);
    lv_obj_set_pos(this->ipAddressLabel, 80, this->_settings_panel_height - 60);
    lv_obj_set_align(this->ipAddressLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text_fmt(this->ipAddressLabel, "%s%s",
                          settings_translation[access_point_ip], "---.---.--.--");
    lv_obj_set_style_text_font(this->ipAddressLabel, &font_18, 0);

    this->homeButton = lv_btn_create(this->settingsPanel);
    lv_obj_set_size(this->homeButton, 35, 35);
    lv_obj_set_pos(this->homeButton, 401, -6);
    lv_obj_set_align(this->homeButton, LV_ALIGN_TOP_LEFT);

    this->homeButtonLabel = lv_label_create(this->homeButton);
    lv_obj_set_align(homeButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(this->homeButtonLabel, LV_SYMBOL_HOME);
    lv_obj_set_style_text_align(this->homeButtonLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(this->homeButtonLabel, &lv_font_montserrat_20, 0);

    lv_obj_add_event_cb(this->cityTextArea, settings_cityTextArea_event_cb_wrapper,
                        LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(this->SSIDTextArea, settings_SSIDTextArea_event_cb_wrapper,
                        LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(this->passwordTextArea,
                        settings_passwordTextArea_event_cb_wrapper, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(this->brightnessSlider,
                        settings_brightnessSlider_event_cb_wrapper,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(this->autoBrightnessCheckbox,
                        settings_autoBrightness_checkbox_event_cb_wrapper,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(this->homeButton, home_button_event_cb_wrapper, LV_EVENT_CLICKED,
                        NULL);
    lv_obj_add_event_cb(this->weatherSwitch, weather_switch_event_cb_wrapper,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(this->wifiButton, wifi_button_event_cb_wrapper, LV_EVENT_PRESSED,
                        NULL);
}
Settings::~Settings() {
    // if (settingsScreen != NULL)
    // {
    //     lv_obj_remove_event_cb(this->cityTextArea,
    //     settings_cityTextArea_event_cb_wrapper);
    //     lv_obj_remove_event_cb(this->SSIDTextArea,
    //     settings_SSIDTextArea_event_cb_wrapper);
    //     lv_obj_remove_event_cb(this->passwordTextArea,
    //     settings_passwordTextArea_event_cb_wrapper);
    //     lv_obj_remove_event_cb(this->homeButton,
    //     home_button_event_cb_wrapper); this->settingsScreen = NULL;
    //     this->settingsPanel = NULL;
    //     this->darkmodeLabel = NULL;
    //     this->darkmodeSwitch = NULL;
    //     this->cityTextArea = NULL;
    //     this->cityLabel = NULL;
    //     this->SSIDTextArea = NULL;
    //     this->SSIDLabel = NULL;
    //     this->passwordTextArea = NULL;
    //     this->passwordLabel = NULL;
    //     this->APLabel = NULL;
    //     this->ipAddressLabel = NULL;
    //     this->homeButton = NULL;
    //     this->homeButtonLabel = NULL;
    // }
}
