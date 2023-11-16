#include "GUI/Settings.h"

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

Settings::Settings() {
    instance = this;
    this->init_settings_screen();
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
        lv_obj_set_size(this->keyboard, lv_pct(100), 150);
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
        lv_obj_set_height(this->settingsPanel, this->_settings_panel_height / 4);
        lv_obj_readjust_scroll(this->settingsScreen, LV_ANIM_OFF);
        lv_obj_scroll_to_y(this->settingsPanel,
                           lv_obj_get_y(lv_event_get_current_target(e)) - 80, LV_ANIM_ON);
        this->create_keyboard(lv_event_get_current_target(e));
    }
    if (event_code == LV_EVENT_READY) {
        this->delete_keyboard();
        // _preferences.begin(NAMESPACE);
        // _preferences.putString("city",
        // lv_textarea_get_text(this->cityTextArea)); _preferences.end();
    }
}

void Settings::settings_SSIDTextArea_event_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_FOCUSED || event_code == LV_EVENT_CLICKED) {
        lv_obj_set_height(this->settingsPanel, this->_settings_panel_height / 4);
        lv_obj_readjust_scroll(this->settingsScreen, LV_ANIM_OFF);
        lv_obj_scroll_to_y(this->settingsPanel,
                           lv_obj_get_y(lv_event_get_current_target(e)) - 80, LV_ANIM_ON);
        this->create_keyboard(lv_event_get_current_target(e));
    }
    if (event_code == LV_EVENT_READY) {
        this->delete_keyboard();
        // _preferences.begin(NAMESPACE);
        // _preferences.putString("ssid", lv_textarea_get_text(SSIDTextArea));
        // _preferences.end();
    }
}

void Settings::settings_passwordTextArea_event_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_FOCUSED || event_code == LV_EVENT_CLICKED) {
        lv_obj_set_height(this->settingsPanel, this->_settings_panel_height / 4);
        lv_obj_readjust_scroll(this->settingsScreen, LV_ANIM_OFF);
        lv_obj_scroll_to_y(this->settingsPanel,
                           lv_obj_get_y(lv_event_get_current_target(e)) - 80, LV_ANIM_ON);
        this->create_keyboard(lv_event_get_current_target(e));
    }
    if (event_code == LV_EVENT_READY) {
        this->delete_keyboard();
        // _preferences.begin(NAMESPACE);
        // _preferences.putString("password",
        // lv_textarea_get_text(passwordTextArea)); _preferences.end();
    }
}
void Settings::settings_brightnessSlider_event_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    this->_display->set_brightness((uint8_t)lv_slider_get_value(this->brightnessSlider));
    // _preferences.begin(NAMESPACE);
    // _preferences.putUChar("brightness_level",
    // (uint8_t)lv_slider_get_value(this->brightnessSlider)); _preferences.end();
}
void Settings::settings_autoBrightness_checkbox_event_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    bool checked =
        lv_obj_get_state(this->autoBrightnessCheckbox) & LV_STATE_CHECKED ? true : false;
    // _preferences.begin(NAMESPACE);
    // _preferences.putBool("auto_brightness", checked);
    // _preferences.end();
    if (checked) {

    } else {
    }
}
void Settings::set_display(Display *display) { _display = display; }
void Settings::set_preferences(Preferences &preferences) { _preferences = preferences; }
void Settings::init_settings_screen() {
    this->keyboard = NULL;
    this->settingsScreen = lv_obj_create(NULL);
    lv_obj_add_flag(this->settingsScreen, LV_OBJ_FLAG_IGNORE_LAYOUT); /// Flags
    lv_obj_set_scroll_dir(this->settingsScreen, LV_DIR_VER);
    lv_obj_clear_flag(this->settingsScreen, LV_OBJ_FLAG_SCROLL_ELASTIC);

    this->settingsPanel = lv_obj_create(this->settingsScreen);
    lv_obj_set_size(this->settingsPanel, lv_pct(100), this->_settings_panel_height);
    lv_obj_set_style_bg_color(this->settingsPanel, lv_color_hex(0xFFFFFF),
                              LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(this->settingsPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(this->settingsPanel, LV_OBJ_FLAG_SCROLL_ELASTIC);

    this->darkmodeLabel = lv_label_create(this->settingsPanel);
    lv_obj_set_size(this->darkmodeLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
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
                    10, 35);
    lv_slider_set_range(this->brightnessSlider, 5, 255);

    this->autoBrightnessCheckbox = lv_checkbox_create(this->settingsPanel);
    lv_obj_align_to(this->autoBrightnessCheckbox, this->brightnessSlider,
                    LV_ALIGN_OUT_RIGHT_MID, 50, 0);
    lv_checkbox_set_text(this->autoBrightnessCheckbox,
                         settings_translation[auto_brightness]);
    lv_obj_set_style_text_font(this->autoBrightnessCheckbox, &font_18, 0);
    lv_obj_set_style_text_font(this->autoBrightnessCheckbox, &font_18, LV_PART_INDICATOR | LV_STATE_CHECKED);

    this->cityTextArea = lv_textarea_create(this->settingsPanel);
    lv_obj_set_size(this->cityTextArea, 250, LV_SIZE_CONTENT); /// 33
    lv_obj_set_pos(this->cityTextArea, 180, this->_settings_panel_height / 5);
    lv_obj_set_align(this->cityTextArea, LV_ALIGN_TOP_LEFT);
    lv_textarea_set_max_length(this->cityTextArea, 100);
    lv_textarea_set_placeholder_text(this->cityTextArea,
                                     settings_translation[city_placeholder]);
    lv_textarea_set_one_line(this->cityTextArea, true);

    this->cityLabel = lv_label_create(this->settingsPanel);
    lv_obj_set_size(this->cityLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(this->cityLabel, 20, this->_settings_panel_height / 5);
    lv_obj_set_align(this->cityLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(this->cityLabel, settings_translation[city]);
    lv_obj_set_style_text_font(this->cityLabel, &font_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(this->cityLabel, LV_TEXT_ALIGN_CENTER,
                                LV_PART_MAIN | LV_STATE_DEFAULT);

    this->SSIDTextArea = lv_textarea_create(this->settingsPanel);
    lv_obj_set_size(this->SSIDTextArea, 250, LV_SIZE_CONTENT);
    lv_obj_set_pos(this->SSIDTextArea, 180, this->_settings_panel_height / 5 + 60);
    lv_obj_set_align(this->SSIDTextArea, LV_ALIGN_TOP_LEFT);
    lv_textarea_set_max_length(this->SSIDTextArea, 50);
    lv_textarea_set_placeholder_text(this->SSIDTextArea,
                                     settings_translation[wifi_ssid_placeholder]);
    lv_textarea_set_one_line(this->SSIDTextArea, true);

    this->SSIDLabel = lv_label_create(this->settingsPanel);
    lv_obj_set_size(this->SSIDLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(this->SSIDLabel, 40, this->_settings_panel_height / 5 + 70);
    lv_obj_set_align(this->SSIDLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(this->SSIDLabel, settings_translation[wifi_ssid]);
    lv_obj_set_style_text_font(this->SSIDLabel, &font_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    this->passwordTextArea = lv_textarea_create(this->settingsPanel);
    lv_obj_set_size(this->passwordTextArea, 250, LV_SIZE_CONTENT); /// 33
    lv_obj_set_pos(this->passwordTextArea, 180,
                   this->_settings_panel_height / 5 + 60 * 2);
    lv_obj_set_align(this->passwordTextArea, LV_ALIGN_TOP_LEFT);
    lv_textarea_set_max_length(this->passwordTextArea, 100);
    lv_textarea_set_placeholder_text(this->passwordTextArea,
                                     settings_translation[wifi_password_placeholder]);
    lv_textarea_set_one_line(this->passwordTextArea, true);
    lv_textarea_set_password_mode(this->passwordTextArea, true);

    this->passwordLabel = lv_label_create(this->settingsPanel);
    lv_obj_set_size(this->passwordLabel, LV_SIZE_CONTENT,
                    LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(this->passwordLabel, 20, this->_settings_panel_height / 5 + 65 * 2);
    lv_obj_set_align(this->passwordLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(this->passwordLabel, settings_translation[wifi_password]);
    lv_obj_set_style_text_font(this->passwordLabel, &font_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    this->APLabel = lv_label_create(this->settingsPanel);
    lv_obj_set_size(this->APLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(this->APLabel, 80, this->_settings_panel_height - 60);
    lv_obj_set_align(this->APLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(this->APLabel, settings_translation[access_point_ip]);
    lv_obj_set_style_text_font(this->APLabel, &font_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    this->IPLabel = lv_label_create(this->settingsPanel);
    lv_obj_set_size(this->IPLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(this->IPLabel, 230, this->_settings_panel_height - 60);
    lv_obj_set_align(this->IPLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(this->IPLabel, "192.120.12.99");
    lv_obj_set_style_text_font(this->IPLabel, &font_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    this->homeButton = lv_btn_create(this->settingsPanel);
    lv_obj_set_size(this->homeButton, 35, 35);
    lv_obj_set_pos(this->homeButton, 401, -6);
    lv_obj_set_align(this->homeButton, LV_ALIGN_TOP_LEFT);

    this->homeButtonLabel = lv_label_create(this->homeButton);
    lv_obj_set_align(homeButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(this->homeButtonLabel, LV_SYMBOL_HOME);
    lv_obj_set_style_text_align(this->homeButtonLabel, LV_TEXT_ALIGN_CENTER,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(this->homeButtonLabel, &lv_font_montserrat_20,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

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
    //     this->IPLabel = NULL;
    //     this->homeButton = NULL;
    //     this->homeButtonLabel = NULL;
    // }
}
