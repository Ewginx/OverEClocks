#include "GUI/Settings.h"

static Settings *instance = NULL;

extern "C" void keyboard_event_cb_wrapper(lv_event_t *e) {
  instance->keyboard_event_cb(e);
}
extern "C" void home_button_event_cb_wrapper(lv_event_t *e) {
  instance->home_button_event_cb(e);
}

extern "C" void settings_city_edit_event_cb_wrapper(lv_event_t *e) {
  instance->settings_city_edit_event_cb(e);
}

extern "C" void settings_SSID_edit_event_cb_wrapper(lv_event_t *e) {
  instance->settings_SSID_edit_event_cb(e);
}

extern "C" void settings_password_edit_event_cb_wrapper(lv_event_t *e) {
  instance->settings_password_edit_event_cb(e);
}

extern "C" void settings_brightnessSlider_event_cb_wrapper(lv_event_t *e) {
  instance->settings_brightnessSlider_event_cb(e);
}
extern "C" void settings_autoBrightness_checkbox_event_cb_wrapper(lv_event_t *e) {
  instance->settings_autoBrightness_checkbox_event_cb(e);
}

Settings::Settings()
{   
    instance = this;
    this->init_settings_screen();

}

void Settings::load_settings_screen(lv_obj_t *screen)
{
    lv_theme_t * theme = lv_disp_get_theme(NULL);
    if (!theme->flags){
        lv_obj_add_state(this->ui_DarkmodeSwitch, LV_STATE_CHECKED);
    }
    lv_slider_set_value(this->brightnessSlider, this->_display->get_brightness(), LV_ANIM_OFF);
    this->home_screen = screen;
    lv_scr_load_anim( this->ui_SettingsScreen, LV_SCR_LOAD_ANIM_FADE_ON, SCREEN_CHANGE_ANIM_TIME, 0, false);
}


void Settings::create_keyboard(lv_obj_t *target)
{
    if (this->ui_SettingsKeyboard == NULL)
    {
        this->ui_SettingsKeyboard = lv_keyboard_create(this->ui_SettingsScreen);
        lv_obj_set_size(this->ui_SettingsKeyboard, lv_pct(100), 150);
        lv_obj_set_pos(this->ui_SettingsKeyboard, 0, 0);
        lv_obj_set_align(this->ui_SettingsKeyboard, LV_ALIGN_BOTTOM_LEFT);
        lv_keyboard_set_textarea(this->ui_SettingsKeyboard, target);
        lv_obj_add_event_cb(this->ui_SettingsKeyboard, keyboard_event_cb_wrapper, LV_EVENT_CANCEL, NULL);
    }
    else
    {
        lv_keyboard_set_textarea(this->ui_SettingsKeyboard, target);
    }
}

void Settings::delete_keyboard()
{
    if (this->ui_SettingsKeyboard != NULL)
    {
        lv_keyboard_set_textarea(this->ui_SettingsKeyboard, NULL);
        lv_obj_remove_event_cb(this->ui_SettingsKeyboard, keyboard_event_cb_wrapper);
        lv_obj_del(this->ui_SettingsKeyboard);
        lv_obj_set_height(this->ui_SettingsPanel, this->settings_panel_height);
        this->ui_SettingsKeyboard = NULL;
    }
}

void Settings::keyboard_event_cb(lv_event_t *e){
    this->delete_keyboard();
}

void Settings::home_button_event_cb(lv_event_t *e){
    lv_obj_t *target = lv_event_get_target(e);
    lv_event_send(this->ui_SettingsKeyboard, LV_EVENT_CANCEL, NULL);
    lv_scr_load_anim(this->home_screen, LV_SCR_LOAD_ANIM_FADE_ON, SCREEN_CHANGE_ANIM_TIME, 0, false);
}

void Settings::settings_city_edit_event_cb(lv_event_t *e){
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_FOCUSED || event_code == LV_EVENT_CLICKED)
    {
        lv_obj_set_height(this->ui_SettingsPanel, this->settings_panel_height / 4);
        lv_obj_readjust_scroll(this->ui_SettingsScreen, LV_ANIM_OFF);
        lv_obj_scroll_to_y(this->ui_SettingsPanel, lv_obj_get_y(lv_event_get_current_target(e)) - 80, LV_ANIM_ON);
        this->create_keyboard(lv_event_get_current_target(e));
    }
    if (event_code == LV_EVENT_READY)
    {
        this->delete_keyboard();
        // _preferences.begin(NAMESPACE);
        // _preferences.putString("city", lv_textarea_get_text(this->ui_SettingsCityEdit));
        // _preferences.end();
    }
}


void Settings::settings_SSID_edit_event_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_FOCUSED || event_code == LV_EVENT_CLICKED)
    {
        lv_obj_set_height(this->ui_SettingsPanel, this->settings_panel_height / 4);
        lv_obj_readjust_scroll(this->ui_SettingsScreen, LV_ANIM_OFF);
        lv_obj_scroll_to_y(this->ui_SettingsPanel, lv_obj_get_y(lv_event_get_current_target(e)) - 80, LV_ANIM_ON);
        this->create_keyboard(lv_event_get_current_target(e));
    }
    if (event_code == LV_EVENT_READY)
    {
        this->delete_keyboard();
        // _preferences.begin(NAMESPACE);
        // _preferences.putString("ssid", lv_textarea_get_text(ui_SettingsSSIDEdit));
        // _preferences.end();
    }
}

void Settings::settings_password_edit_event_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_FOCUSED || event_code == LV_EVENT_CLICKED)
    {
        lv_obj_set_height(this->ui_SettingsPanel, this->settings_panel_height / 4);
        lv_obj_readjust_scroll(this->ui_SettingsScreen, LV_ANIM_OFF);
        lv_obj_scroll_to_y(this->ui_SettingsPanel, lv_obj_get_y(lv_event_get_current_target(e)) - 80, LV_ANIM_ON);
        this->create_keyboard(lv_event_get_current_target(e));
    }
    if (event_code == LV_EVENT_READY)
    {
        this->delete_keyboard();
        // _preferences.begin(NAMESPACE);
        // _preferences.putString("password", lv_textarea_get_text(ui_SettingsPasswordEdit));
        // _preferences.end();
    }
}
void Settings::settings_brightnessSlider_event_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    this->_display->set_brightness((uint8_t)lv_slider_get_value(this->brightnessSlider));
    // _preferences.begin(NAMESPACE);
    // _preferences.putUChar("brightness_level", (uint8_t)lv_slider_get_value(this->brightnessSlider));
    // _preferences.end();
}
void Settings::settings_autoBrightness_checkbox_event_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    bool checked = lv_obj_get_state(this->autoBrightnessCheckbox) & LV_STATE_CHECKED ? true : false;
    // _preferences.begin(NAMESPACE);
    // _preferences.putBool("auto_brightness", checked);
    // _preferences.end();
    if(checked){

    }
    else{

    }
}
void Settings::set_display(Display *display)
{
    _display = display;
}
void Settings::set_preferences(Preferences &preferences)
{
    _preferences = preferences;
}
void Settings::init_settings_screen()
{   
    this->ui_SettingsKeyboard = NULL;
    this->ui_SettingsScreen = lv_obj_create(NULL);
    lv_obj_add_flag(this->ui_SettingsScreen, LV_OBJ_FLAG_IGNORE_LAYOUT); /// Flags
    lv_obj_set_scroll_dir(this->ui_SettingsScreen, LV_DIR_VER);
    lv_obj_clear_flag(this->ui_SettingsScreen, LV_OBJ_FLAG_SCROLL_ELASTIC);

    this->ui_SettingsPanel = lv_obj_create(this->ui_SettingsScreen);
    lv_obj_set_size(this->ui_SettingsPanel, lv_pct(100), this->settings_panel_height);
    lv_obj_set_style_bg_color(this->ui_SettingsPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(this->ui_SettingsPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(this->ui_SettingsPanel, LV_OBJ_FLAG_SCROLL_ELASTIC);

    this->ui_DarkmodeLabel = lv_label_create(this->ui_SettingsPanel);
    lv_obj_set_size(this->ui_DarkmodeLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(this->ui_DarkmodeLabel, 35, 15);
    lv_obj_set_align(this->ui_DarkmodeLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(this->ui_DarkmodeLabel, settings_translation[THEME]);
    lv_obj_set_style_text_font(this->ui_DarkmodeLabel, &montserrat_18, LV_PART_MAIN);

    this-> ui_DarkmodeSwitch = lv_switch_create(this->ui_SettingsPanel);
    lv_obj_set_size(this->ui_DarkmodeSwitch, 50, 25);
    lv_obj_set_pos(this->ui_DarkmodeSwitch, 175, 15);
    lv_obj_set_align(this->ui_DarkmodeSwitch, LV_ALIGN_TOP_LEFT);

    this->brightnessSlider = lv_slider_create(this->ui_SettingsPanel);
    lv_obj_align_to(this->brightnessSlider, this->ui_DarkmodeLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 10, 35);
    lv_slider_set_range(this->brightnessSlider, 5 , 255);

    this->autoBrightnessCheckbox = lv_checkbox_create(this->ui_SettingsPanel);
    lv_obj_align_to(this->autoBrightnessCheckbox, this->brightnessSlider, LV_ALIGN_OUT_RIGHT_MID, 50, 0);
    lv_obj_set_style_text_font(this->autoBrightnessCheckbox, &montserrat_18, LV_PART_MAIN);
    lv_checkbox_set_text(this->autoBrightnessCheckbox, settings_translation[AUTO]);

    this->ui_SettingsCityEdit = lv_textarea_create(this->ui_SettingsPanel);
    lv_obj_set_size(this->ui_SettingsCityEdit, 250, LV_SIZE_CONTENT); /// 33
    lv_obj_set_pos(this->ui_SettingsCityEdit, 180, this->settings_panel_height / 5);
    lv_obj_set_align(this->ui_SettingsCityEdit, LV_ALIGN_TOP_LEFT);
    lv_textarea_set_max_length(this->ui_SettingsCityEdit, 100);
    lv_textarea_set_placeholder_text(this->ui_SettingsCityEdit, settings_translation[CITY_PLACEHOLDER]);
    lv_textarea_set_one_line(this->ui_SettingsCityEdit, true);

    this->ui_SettingsCityLabel = lv_label_create(this->ui_SettingsPanel);
    lv_obj_set_size(this->ui_SettingsCityLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(this->ui_SettingsCityLabel, 20, this->settings_panel_height / 5);
    lv_obj_set_align(this->ui_SettingsCityLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(this->ui_SettingsCityLabel, settings_translation[CITY]);
    lv_obj_set_style_text_font(this->ui_SettingsCityLabel, &montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(this->ui_SettingsCityLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    this->ui_SettingsSSIDEdit = lv_textarea_create(this->ui_SettingsPanel);
    lv_obj_set_size(this->ui_SettingsSSIDEdit, 250, LV_SIZE_CONTENT);
    lv_obj_set_pos(this->ui_SettingsSSIDEdit, 180, this->settings_panel_height / 5 + 60);
    lv_obj_set_align(this->ui_SettingsSSIDEdit, LV_ALIGN_TOP_LEFT);
    lv_textarea_set_max_length(this->ui_SettingsSSIDEdit, 50);
    lv_textarea_set_placeholder_text(this->ui_SettingsSSIDEdit, settings_translation[WIFI_SSID_PLACEHOLDER]);
    lv_textarea_set_one_line(this->ui_SettingsSSIDEdit, true);

    this->ui_SettingsSSIDLabel = lv_label_create(this->ui_SettingsPanel);
    lv_obj_set_size(this->ui_SettingsSSIDLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(this->ui_SettingsSSIDLabel, 40, this->settings_panel_height / 5 + 70);
    lv_obj_set_align(this->ui_SettingsSSIDLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(this->ui_SettingsSSIDLabel, settings_translation[WIFI_SSID]);
    lv_obj_set_style_text_font(this->ui_SettingsSSIDLabel, &montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    this->ui_SettingsPasswordEdit = lv_textarea_create(this->ui_SettingsPanel);
    lv_obj_set_size(this->ui_SettingsPasswordEdit, 250, LV_SIZE_CONTENT); /// 33
    lv_obj_set_pos(this->ui_SettingsPasswordEdit, 180, this->settings_panel_height / 5 + 60 * 2);
    lv_obj_set_align(this->ui_SettingsPasswordEdit, LV_ALIGN_TOP_LEFT);
    lv_textarea_set_max_length(this->ui_SettingsPasswordEdit, 100);
    lv_textarea_set_placeholder_text(this->ui_SettingsPasswordEdit, settings_translation[WIFI_PASSWORD_PLACEHOLDER]);
    lv_textarea_set_one_line(this->ui_SettingsPasswordEdit, true);
    lv_textarea_set_password_mode(this->ui_SettingsPasswordEdit, true);

    this->ui_SettingsPasswordLabel = lv_label_create(this->ui_SettingsPanel);
    lv_obj_set_size(this->ui_SettingsPasswordLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(this->ui_SettingsPasswordLabel, 20, this->settings_panel_height / 5 + 65 * 2);
    lv_obj_set_align(this->ui_SettingsPasswordLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(this->ui_SettingsPasswordLabel, settings_translation[WIFI_PASSWORD]);
    lv_obj_set_style_text_font(this->ui_SettingsPasswordLabel, &montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    this->ui_SettingsAPLabel = lv_label_create(this->ui_SettingsPanel);
    lv_obj_set_size(this->ui_SettingsAPLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(this->ui_SettingsAPLabel, 80, this->settings_panel_height - 60);
    lv_obj_set_align(this->ui_SettingsAPLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(this->ui_SettingsAPLabel, settings_translation[ACCESS_POINT_IP]);
    lv_obj_set_style_text_font(this->ui_SettingsAPLabel, &montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    this->ui_SettingsIPLabel = lv_label_create(this->ui_SettingsPanel);
    lv_obj_set_size(this->ui_SettingsIPLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(this->ui_SettingsIPLabel, 230, this->settings_panel_height - 60);
    lv_obj_set_align(this->ui_SettingsIPLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(this->ui_SettingsIPLabel, "192.120.12.99");
    lv_obj_set_style_text_font(this->ui_SettingsIPLabel, &montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    this->ui_SettingsHomeButton = lv_btn_create(this->ui_SettingsPanel);
    lv_obj_set_size(this->ui_SettingsHomeButton, 35, 35);
    lv_obj_set_pos(this->ui_SettingsHomeButton, 401, -6);
    lv_obj_set_align(this->ui_SettingsHomeButton, LV_ALIGN_TOP_LEFT);

    this->ui_SettingsHomeButtonLabel = lv_label_create(this->ui_SettingsHomeButton);
    lv_obj_set_align(ui_SettingsHomeButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(this->ui_SettingsHomeButtonLabel, LV_SYMBOL_HOME);
    lv_obj_set_style_text_align(this->ui_SettingsHomeButtonLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(this->ui_SettingsHomeButtonLabel, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    
    lv_obj_add_event_cb(this->ui_SettingsCityEdit, settings_city_edit_event_cb_wrapper, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(this->ui_SettingsSSIDEdit, settings_SSID_edit_event_cb_wrapper, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(this->ui_SettingsPasswordEdit, settings_password_edit_event_cb_wrapper, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(this->brightnessSlider, settings_brightnessSlider_event_cb_wrapper, LV_EVENT_VALUE_CHANGED , NULL);
    lv_obj_add_event_cb(this->autoBrightnessCheckbox, settings_autoBrightness_checkbox_event_cb_wrapper, LV_EVENT_VALUE_CHANGED  , NULL);
    lv_obj_add_event_cb(this->ui_SettingsHomeButton, home_button_event_cb_wrapper, LV_EVENT_CLICKED, NULL);
}
Settings::~Settings()
{
    // if (ui_SettingsScreen != NULL)
    // {
    //     lv_obj_remove_event_cb(this->ui_SettingsCityEdit, settings_city_edit_event_cb_wrapper);
    //     lv_obj_remove_event_cb(this->ui_SettingsSSIDEdit, settings_SSID_edit_event_cb_wrapper);
    //     lv_obj_remove_event_cb(this->ui_SettingsPasswordEdit, settings_password_edit_event_cb_wrapper);
    //     lv_obj_remove_event_cb(this->ui_SettingsHomeButton, home_button_event_cb_wrapper);
    //     this->ui_SettingsScreen = NULL;
    //     this->ui_SettingsPanel = NULL;
    //     this->ui_DarkmodeLabel = NULL;
    //     this->ui_DarkmodeSwitch = NULL;
    //     this->ui_SettingsCityEdit = NULL;
    //     this->ui_SettingsCityLabel = NULL;
    //     this->ui_SettingsSSIDEdit = NULL;
    //     this->ui_SettingsSSIDLabel = NULL;
    //     this->ui_SettingsPasswordEdit = NULL;
    //     this->ui_SettingsPasswordLabel = NULL;
    //     this->ui_SettingsAPLabel = NULL;
    //     this->ui_SettingsIPLabel = NULL;
    //     this->ui_SettingsHomeButton = NULL;
    //     this->ui_SettingsHomeButtonLabel = NULL;
    // }
}
