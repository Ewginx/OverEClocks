#include "Settings.h"

static Settings *instance = NULL;

extern "C" void keyboardEventCallbackWrapper(lv_event_t *e) {
    instance->keyboardEventCallback(e);
}

extern "C" void homeButtonEventCallbackWrapper(lv_event_t *e) {
    instance->homeButtonEventCallback(e);
}

extern "C" void settingsCityTextAreaEventCallbackWrapper(lv_event_t *e) {
    instance->settingsCityTextAreaEventCallback(e);
}

extern "C" void settingsLanguageTextAreaEventCallbackWrapper(lv_event_t *e) {
    instance->settingsLanguageTextAreaEventCallback(e);
}

extern "C" void settingsSsidTextAreaEventCallbackWrapper(lv_event_t *e) {
    instance->settingsSsidTextAreaEventCallback(e);
}

extern "C" void settingsPasswordTextAreaEventCallbackWrapper(lv_event_t *e) {
    instance->settingsPasswordTextAreaEventCallback(e);
}

extern "C" void settingsBrightnessSliderEventCallbackWrapper(lv_event_t *e) {
    instance->settingsBrightnessSliderEventCallback(e);
}

extern "C" void settingsAutoBrightnessCheckboxEventCallbackWrapper(lv_event_t *e) {
    instance->settingsAutoBrightnessCheckboxEventCallback(e);
}

extern "C" void weatherSwitchEventCallbackWrapper(lv_event_t *e) {
    instance->weatherSwitchEventCallback(e);
}

extern "C" void wifiButtonEventCallbackWrapper(lv_event_t *e) {
    instance->wifiButtonEventCallback(e);
}

extern "C" void weatherButtonEventCallbackWrapper(lv_event_t *e) {
    instance->weatherButtonEventCallback(e);
}

extern "C" void updateWeatherGuiStateCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->updateWeatherGuiState();
}

extern "C" void updateIpAddressLabelCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->setIpAddressLabel(instance->stateApp->wifiState->currentIpAddress.c_str());
}

Settings::Settings(StateApp *stateApp) {
    instance = this;

    this->stateApp = stateApp;
    this->createSettingsScreen();

    lv_msg_subscribe(MSG_UPDATE_WEATHER_GUI, updateWeatherGuiStateCallbackWrapper, NULL);
    lv_msg_subscribe(MSG_UPDATE_IP_ADDRESS, updateIpAddressLabelCallbackWrapper, NULL);
}

void Settings::loadSettingsScreen(lv_obj_t *screen) {
    this->homeScreen = screen;
    lv_slider_set_value(this->brightnessSlider,
                        this->stateApp->displayState->brightnessLevel, LV_ANIM_OFF);
    lv_scr_load_anim(this->settingsScreen, LV_SCR_LOAD_ANIM_NONE, SCREEN_CHANGE_ANIM_TIME,
                     0, false);
}

void Settings::createKeyboard(lv_obj_t *target) {
    if (this->keyboard == NULL) {
        this->keyboard = lv_keyboard_create(this->settingsScreen);
        lv_obj_set_size(this->keyboard, lv_pct(100), 160);
        lv_obj_set_pos(this->keyboard, 0, 0);
        lv_obj_set_align(this->keyboard, LV_ALIGN_BOTTOM_LEFT);
        lv_keyboard_set_textarea(this->keyboard, target);
        lv_obj_add_event_cb(this->keyboard, keyboardEventCallbackWrapper, LV_EVENT_CANCEL,
                            NULL);
        lv_obj_add_flag(this->keyboard, LV_OBJ_FLAG_EVENT_BUBBLE);

    } else {
        lv_keyboard_set_textarea(this->keyboard, target);
    }
}

void Settings::deleteKeyboard() {
    if (this->keyboard != NULL) {
        lv_keyboard_set_textarea(this->keyboard, NULL);
        lv_obj_remove_event_cb(this->keyboard, keyboardEventCallbackWrapper);
        lv_obj_del(this->keyboard);
        lv_obj_set_height(this->settingsPanel, this->settingsPanelHeight);
        this->keyboard = NULL;
    }
}

void Settings::setIpAddressLabel(const char *ipAddress) {
    lv_label_set_text_fmt(this->ipAddressLabel, "%s %s",
                          settings_translation[access_point_ip], ipAddress);
}

void Settings::wifiButtonEventCallback(lv_event_t *e) {
    lv_msg_send(MSG_WIFI_RECONNECT, NULL);
}

void Settings::weatherButtonEventCallback(lv_event_t *e) {
    lv_msg_send(MSG_WEATHER_UPDATE, NULL);
}

void Settings::weatherSwitchEventCallback(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_VALUE_CHANGED) {
        bool enabled = lv_obj_has_state(target, LV_STATE_CHECKED);
        if (enabled) {
            lv_obj_clear_state(this->weatherButton, LV_STATE_DISABLED);
        } else {
            lv_obj_add_state(this->weatherButton, LV_STATE_DISABLED);
        }
        this->stateApp->weatherState->saveWeatherEnabled(enabled);
        lv_msg_send(MSG_UPDATE_WEATHER_TASK, static_cast<const void *>(&enabled));
    }
}

void Settings::keyboardEventCallback(lv_event_t *e) { this->deleteKeyboard(); }

void Settings::homeButtonEventCallback(lv_event_t *e) {
    lv_obj_t *target = lv_event_get_target(e);
    lv_event_send(this->keyboard, LV_EVENT_CANCEL, NULL);
    lv_scr_load_anim(this->homeScreen, LV_SCR_LOAD_ANIM_NONE, SCREEN_CHANGE_ANIM_TIME, 0,
                     false);
}

void Settings::settingsCityTextAreaEventCallback(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_FOCUSED || event_code == LV_EVENT_CLICKED) {
        lv_obj_set_height(this->settingsPanel, this->settingsPanelHeight / 2);
        lv_obj_readjust_scroll(this->settingsScreen, LV_ANIM_OFF);
        lv_obj_scroll_to_y(this->settingsPanel,
                           lv_obj_get_y(lv_event_get_current_target(e)) - 80, LV_ANIM_ON);
        this->createKeyboard(lv_event_get_current_target(e));
    }
    if (event_code == LV_EVENT_READY) {
        this->deleteKeyboard();
        this->stateApp->weatherState->saveCity(lv_textarea_get_text(this->cityTextArea));
        lv_msg_send(MSG_WEATHER_CITY_CHANGED, NULL);
    }
}

void Settings::settingsLanguageTextAreaEventCallback(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_FOCUSED || event_code == LV_EVENT_CLICKED) {
        lv_obj_set_height(this->settingsPanel, this->settingsPanelHeight / 2);
        lv_obj_readjust_scroll(this->settingsScreen, LV_ANIM_OFF);
        lv_obj_scroll_to_y(this->settingsPanel,
                           lv_obj_get_y(lv_event_get_current_target(e)) - 80, LV_ANIM_ON);
        this->createKeyboard(lv_event_get_current_target(e));
    }
    if (event_code == LV_EVENT_READY) {
        this->deleteKeyboard();
        this->stateApp->weatherState->saveLanguage(
            lv_textarea_get_text(this->languageTextArea));
        lv_msg_send(MSG_WEATHER_LANGUAGE_CHANGED, NULL);
    }
}

void Settings::settingsSsidTextAreaEventCallback(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_FOCUSED || event_code == LV_EVENT_CLICKED) {
        lv_obj_set_height(this->settingsPanel, this->settingsPanelHeight / 2);
        lv_obj_readjust_scroll(this->settingsScreen, LV_ANIM_OFF);
        lv_obj_scroll_to_y(this->settingsPanel,
                           lv_obj_get_y(lv_event_get_current_target(e)) - 80, LV_ANIM_ON);
        this->createKeyboard(lv_event_get_current_target(e));
    }
    if (event_code == LV_EVENT_READY) {
        this->deleteKeyboard();
        this->stateApp->wifiState->saveSsid(lv_textarea_get_text(ssidTextArea));
    }
}

void Settings::settingsPasswordTextAreaEventCallback(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_FOCUSED || event_code == LV_EVENT_CLICKED) {
        lv_obj_set_height(this->settingsPanel, this->settingsPanelHeight / 2);
        lv_obj_readjust_scroll(this->settingsScreen, LV_ANIM_OFF);
        lv_obj_scroll_to_y(this->settingsPanel,
                           lv_obj_get_y(lv_event_get_current_target(e)) - 80, LV_ANIM_ON);
        this->createKeyboard(lv_event_get_current_target(e));
    }
    if (event_code == LV_EVENT_READY) {
        this->deleteKeyboard();
        this->stateApp->wifiState->savePassword(lv_textarea_get_text(passwordTextArea));
    }
}

void Settings::settingsBrightnessSliderEventCallback(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    this->stateApp->displayState->saveBrightnessLevel(
        lv_slider_get_value(this->brightnessSlider));
    lv_msg_send(MSG_BRIGHTNESS_CHANGED, NULL);
}

void Settings::settingsAutoBrightnessCheckboxEventCallback(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    bool checked = lv_obj_has_state(this->autoBrightnessCheckbox, LV_STATE_CHECKED);
    if (checked) {
        lv_obj_add_state(this->brightnessSlider, LV_STATE_DISABLED);
    } else {
        lv_obj_clear_state(this->brightnessSlider, LV_STATE_DISABLED);
    }
    this->stateApp->displayState->saveAutoBrightnessEnabled(checked);
    lv_msg_send(MSG_AUTO_BRIGHTNESS, NULL);
}

void Settings::setWeatherSettings(const char *city, const char *language) {
    lv_textarea_add_text(this->cityTextArea, city);
    lv_textarea_add_text(this->languageTextArea, language);
}

void Settings::setWifiSettings(const char *ssid, const char *password) {
    lv_textarea_add_text(this->ssidTextArea, ssid);
    lv_textarea_add_text(this->passwordTextArea, password);
}

void Settings::setThemeSwitch(bool darkThemeEnabled) {
    lv_obj_add_state(this->themeSwitch,
                     darkThemeEnabled ? LV_STATE_CHECKED : LV_STATE_DEFAULT);
}

void Settings::setBrightnessSliderValue(u_int32_t sliderValue, bool withAnimation) {
    lv_slider_set_value(this->brightnessSlider, sliderValue,
                        withAnimation ? LV_ANIM_ON : LV_ANIM_OFF);
}

void Settings::setBrightnessCheckbox(bool autoBrightnessEnabled) {
    if (autoBrightnessEnabled) {
        lv_obj_add_state(this->brightnessSlider, LV_STATE_DISABLED);
        lv_obj_add_state(this->autoBrightnessCheckbox, LV_STATE_CHECKED);
    } else {
        lv_obj_clear_state(this->brightnessSlider, LV_STATE_DISABLED);
        lv_obj_clear_state(this->autoBrightnessCheckbox, LV_STATE_CHECKED);
    }
}

void Settings::updateWeatherGuiState() {
    lv_textarea_set_text(this->cityTextArea, this->stateApp->weatherState->city.c_str());
    lv_textarea_set_text(this->languageTextArea,
                         this->stateApp->weatherState->language.c_str());
    if (!this->stateApp->wifiState->wifiIsConnected) {
        lv_obj_clear_state(this->weatherSwitch, LV_STATE_CHECKED);
        lv_obj_add_state(this->weatherSwitch, LV_STATE_DISABLED);
        lv_obj_add_state(this->weatherButton, LV_STATE_DISABLED);
    } else {
        if (this->stateApp->weatherState->weatherEnabled) {
            lv_obj_add_state(this->weatherSwitch, LV_STATE_CHECKED);
            lv_obj_clear_state(this->weatherButton, LV_STATE_DISABLED);
            lv_obj_clear_state(this->weatherSwitch, LV_STATE_DISABLED);

        } else {
            lv_obj_clear_state(this->weatherSwitch, LV_STATE_CHECKED);
            lv_obj_add_state(this->weatherButton, LV_STATE_DISABLED);
        }
    }
}

void Settings::createSettingsScreen() {
    this->keyboard = NULL;
    this->settingsScreen = lv_obj_create(NULL);
    lv_obj_add_flag(this->settingsScreen, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_scroll_dir(this->settingsScreen, LV_DIR_VER);
    lv_obj_clear_flag(this->settingsScreen, LV_OBJ_FLAG_SCROLL_ELASTIC);

    this->settingsPanel = lv_obj_create(this->settingsScreen);
    lv_obj_set_size(this->settingsPanel, lv_pct(100), this->settingsPanelHeight);
    lv_obj_clear_flag(this->settingsPanel, LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_add_flag(this->settingsPanel, LV_OBJ_FLAG_EVENT_BUBBLE);

    this->themeLabel = lv_label_create(this->settingsPanel);
    lv_obj_set_pos(this->themeLabel, 35, 15);
    lv_obj_set_align(this->themeLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(this->themeLabel, settings_translation[theme]);
    lv_obj_set_style_text_font(this->themeLabel, &font_18, LV_PART_MAIN);

    this->themeSwitch = lv_switch_create(this->settingsPanel);
    lv_obj_set_size(this->themeSwitch, 50, 25);
    lv_obj_set_pos(this->themeSwitch, 175, 15);
    lv_obj_set_align(this->themeSwitch, LV_ALIGN_TOP_LEFT);
    lv_obj_add_flag(this->themeSwitch, LV_OBJ_FLAG_EVENT_BUBBLE);

    this->brightnessSlider = lv_slider_create(this->settingsPanel);
    lv_obj_align_to(this->brightnessSlider, this->themeLabel, LV_ALIGN_OUT_BOTTOM_LEFT,
                    10, 30);
    lv_slider_set_range(this->brightnessSlider, 1, 255);
    lv_obj_add_flag(this->brightnessSlider, LV_OBJ_FLAG_EVENT_BUBBLE);

    this->autoBrightnessCheckbox = lv_checkbox_create(this->settingsPanel);
    lv_obj_align_to(this->autoBrightnessCheckbox, this->brightnessSlider,
                    LV_ALIGN_OUT_RIGHT_MID, 50, 0);
    lv_checkbox_set_text(this->autoBrightnessCheckbox,
                         settings_translation[auto_brightness]);
    lv_obj_set_style_text_font(this->autoBrightnessCheckbox, &font_18, 0);
    lv_obj_set_style_text_font(this->autoBrightnessCheckbox, &font_18,
                               LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_add_flag(this->autoBrightnessCheckbox, LV_OBJ_FLAG_EVENT_BUBBLE);

    this->cityLabel = lv_label_create(this->settingsPanel);
    lv_obj_set_size(this->cityLabel, 120, 45);
    lv_obj_set_pos(this->cityLabel, 10, this->settingsPanelHeight / 5 + 25);
    lv_obj_set_align(this->cityLabel, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_text_align(this->cityLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(this->cityLabel, settings_translation[city]);
    lv_obj_set_style_text_font(this->cityLabel, &font_18, 0);
    lv_obj_set_style_text_align(this->cityLabel, LV_TEXT_ALIGN_CENTER, 0);

    this->cityTextArea = lv_textarea_create(this->settingsPanel);
    lv_obj_set_size(this->cityTextArea, 200, LV_SIZE_CONTENT);
    lv_obj_align_to(this->cityTextArea, this->cityLabel, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
    lv_textarea_set_max_length(this->cityTextArea, 100);
    lv_textarea_set_placeholder_text(this->cityTextArea,
                                     settings_translation[city_placeholder]);
    lv_textarea_set_one_line(this->cityTextArea, true);
    lv_obj_add_flag(this->cityTextArea, LV_OBJ_FLAG_EVENT_BUBBLE);

    this->languageTextArea = lv_textarea_create(this->settingsPanel);
    lv_obj_set_size(this->languageTextArea, 60, LV_SIZE_CONTENT);
    lv_obj_align_to(this->languageTextArea, this->cityTextArea, LV_ALIGN_OUT_RIGHT_MID,
                    10, 0);
    lv_textarea_set_max_length(this->languageTextArea, 100);
    lv_textarea_set_placeholder_text(this->languageTextArea,
                                     settings_translation[language_placeholder]);
    lv_textarea_set_one_line(this->languageTextArea, true);
    lv_obj_add_flag(this->languageTextArea, LV_OBJ_FLAG_EVENT_BUBBLE);

    this->ssidLabel = lv_label_create(this->settingsPanel);
    lv_obj_set_size(this->ssidLabel, 120, 45);
    lv_obj_set_align(this->ssidLabel, LV_ALIGN_TOP_LEFT);
    lv_obj_align_to(this->ssidLabel, this->cityLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 25);
    lv_label_set_text(this->ssidLabel, settings_translation[wifi_ssid]);
    lv_obj_set_style_text_align(this->ssidLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(this->ssidLabel, &font_18, 0);

    this->ssidTextArea = lv_textarea_create(this->settingsPanel);
    lv_obj_set_size(this->ssidTextArea, 250, LV_SIZE_CONTENT);
    lv_obj_align_to(this->ssidTextArea, this->ssidLabel, LV_ALIGN_OUT_RIGHT_MID, 20, -10);
    lv_textarea_set_max_length(this->ssidTextArea, 50);
    lv_textarea_set_placeholder_text(this->ssidTextArea,
                                     settings_translation[wifi_ssid_placeholder]);
    lv_textarea_set_one_line(this->ssidTextArea, true);
    lv_obj_add_flag(this->ssidTextArea, LV_OBJ_FLAG_EVENT_BUBBLE);

    this->passwordLabel = lv_label_create(this->settingsPanel);
    lv_obj_set_size(this->passwordLabel, 120, 45);
    lv_obj_align_to(this->passwordLabel, this->ssidLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0,
                    15);
    lv_label_set_text(this->passwordLabel, settings_translation[wifi_password]);
    lv_obj_set_style_text_align(this->passwordLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(this->passwordLabel, &font_18, 0);

    this->passwordTextArea = lv_textarea_create(this->settingsPanel);
    lv_obj_set_size(this->passwordTextArea, 250, LV_SIZE_CONTENT);
    lv_obj_align_to(this->passwordTextArea, this->passwordLabel, LV_ALIGN_OUT_RIGHT_MID,
                    20, -10);
    lv_textarea_set_max_length(this->passwordTextArea, 100);
    lv_textarea_set_placeholder_text(this->passwordTextArea,
                                     settings_translation[wifi_password_placeholder]);
    lv_textarea_set_one_line(this->passwordTextArea, true);
    lv_textarea_set_password_mode(this->passwordTextArea, true);
    lv_obj_add_flag(this->passwordTextArea, LV_OBJ_FLAG_EVENT_BUBBLE);

    this->weatherLabel = lv_label_create(this->settingsPanel);
    lv_obj_align_to(this->weatherLabel, this->passwordLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0,
                    10);
    lv_label_set_text(this->weatherLabel, settings_translation[update_weather]);
    lv_obj_set_style_text_font(this->weatherLabel, &font_18, 0);

    this->weatherSwitch = lv_switch_create(this->settingsPanel);
    lv_obj_set_size(this->weatherSwitch, 50, 25);
    lv_obj_align_to(this->weatherSwitch, this->weatherLabel, LV_ALIGN_OUT_RIGHT_MID, 30,
                    0);
    lv_obj_add_flag(this->weatherSwitch, LV_OBJ_FLAG_EVENT_BUBBLE);

    this->wifiButton = lv_btn_create(this->settingsPanel);
    lv_obj_set_size(this->wifiButton, 200, 35);
    lv_obj_align_to(this->wifiButton, this->weatherLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0,
                    15);
    lv_obj_add_flag(this->wifiButton, LV_OBJ_FLAG_EVENT_BUBBLE);

    this->wifiButtonLabel = lv_label_create(this->wifiButton);
    lv_obj_set_align(this->wifiButtonLabel, LV_ALIGN_CENTER);
    lv_obj_set_size(this->wifiButtonLabel, lv_obj_get_width(this->wifiButton),
                    LV_SIZE_CONTENT);
    lv_label_set_text(this->wifiButtonLabel, settings_translation[reconnect_wifi]);
    lv_obj_set_style_text_font(this->wifiButtonLabel, &font_18, 0);
    lv_label_set_long_mode(this->wifiButtonLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);

    this->weatherButton = lv_btn_create(this->settingsPanel);
    lv_obj_set_size(this->weatherButton, 200, 35);
    lv_obj_align_to(this->weatherButton, this->wifiButton, LV_ALIGN_OUT_RIGHT_MID, 25, 0);
    lv_obj_add_flag(this->weatherButton, LV_OBJ_FLAG_EVENT_BUBBLE);

    this->weatherButtonLabel = lv_label_create(this->weatherButton);
    lv_obj_set_align(this->weatherButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(this->weatherButtonLabel, settings_translation[update_weather]);
    lv_obj_set_style_text_font(this->weatherButtonLabel, &font_18, 0);

    this->ipAddressLabel = lv_label_create(this->settingsPanel);
    lv_obj_set_pos(this->ipAddressLabel, 80, this->settingsPanelHeight - 60);
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
    lv_obj_set_style_text_font(this->homeButtonLabel, &dock_panel_font_20, 0);

    lv_obj_add_event_cb(this->cityTextArea, settingsCityTextAreaEventCallbackWrapper,
                        LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(this->languageTextArea,
                        settingsLanguageTextAreaEventCallbackWrapper, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(this->ssidTextArea, settingsSsidTextAreaEventCallbackWrapper,
                        LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(this->passwordTextArea,
                        settingsPasswordTextAreaEventCallbackWrapper, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(this->brightnessSlider,
                        settingsBrightnessSliderEventCallbackWrapper,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(this->autoBrightnessCheckbox,
                        settingsAutoBrightnessCheckboxEventCallbackWrapper,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(this->homeButton, homeButtonEventCallbackWrapper,
                        LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(this->weatherSwitch, weatherSwitchEventCallbackWrapper,
                        LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(this->wifiButton, wifiButtonEventCallbackWrapper,
                        LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(this->weatherButton, weatherButtonEventCallbackWrapper,
                        LV_EVENT_PRESSED, NULL);
}

Settings::~Settings() {}
