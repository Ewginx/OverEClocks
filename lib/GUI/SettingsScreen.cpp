#include "SettingsScreen.h"

SettingsScreen *instance;

extern "C" void keyboard_event_cb_wrapper(lv_event_t *e) {
  instance->keyboard_event_cb(e);
}

extern "C" void home_button_event_cb_wrapper(lv_event_t *e) {
  instance->home_button_event_cb(e);
}

extern "C" void settings_city_edit_event_cb_wrapper(lv_event_t *e) {
  instance->settings_city_edit_event_cb(e);
}

extern "C" void darkmode_switch_event_cb_wrapper(lv_event_t *e) {
  instance->darkmode_switch_event_cb(e);
}

extern "C" void settings_SSID_edit_event_cb_wrapper(lv_event_t *e) {
  instance->settings_SSID_edit_event_cb(e);
}

extern "C" void settings_password_edit_event_cb_wrapper(lv_event_t *e) {
  instance->settings_password_edit_event_cb(e);
}

SettingsScreen::SettingsScreen(lv_obj_t *screen)
{   home_screen = screen;
    instance = this;
    ui_SettingsKeyboard = NULL;
    ui_SettingsScreen = lv_obj_create(NULL);
    lv_obj_add_flag(ui_SettingsScreen, LV_OBJ_FLAG_IGNORE_LAYOUT); /// Flags
    lv_obj_set_scroll_dir(ui_SettingsScreen, LV_DIR_VER);
    lv_obj_clear_flag(ui_SettingsScreen, LV_OBJ_FLAG_SCROLL_ELASTIC);

    ui_SettingsPanel = lv_obj_create(ui_SettingsScreen);
    lv_obj_set_size(ui_SettingsPanel, lv_pct(100), this->settings_panel_height);
    lv_obj_set_style_bg_color(ui_SettingsPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SettingsPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(ui_SettingsPanel, LV_OBJ_FLAG_SCROLL_ELASTIC);

    ui_DarkmodeLabel = lv_label_create(ui_SettingsPanel);
    lv_obj_set_size(ui_DarkmodeLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(ui_DarkmodeLabel, 35, 15);
    lv_obj_set_align(ui_DarkmodeLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(ui_DarkmodeLabel, "Dark Mode");
    lv_obj_set_style_text_font(ui_DarkmodeLabel, &lv_font_montserrat_18, LV_PART_MAIN);

    ui_DarkmodeSwitch = lv_switch_create(ui_SettingsPanel);
    lv_obj_set_size(ui_DarkmodeSwitch, 50, 25);
    lv_obj_set_pos(ui_DarkmodeSwitch, 175, 15);
    lv_obj_set_align(ui_DarkmodeSwitch, LV_ALIGN_TOP_LEFT);

    ui_SettingsCityEdit = lv_textarea_create(ui_SettingsPanel);
    lv_obj_set_size(ui_SettingsCityEdit, 250, LV_SIZE_CONTENT); /// 33
    lv_obj_set_pos(ui_SettingsCityEdit, 180, this->settings_panel_height / 5);
    lv_obj_set_align(ui_SettingsCityEdit, LV_ALIGN_TOP_LEFT);
    lv_textarea_set_max_length(ui_SettingsCityEdit, 100);
    lv_textarea_set_placeholder_text(ui_SettingsCityEdit, "City, country code ISO-81118");
    lv_textarea_set_one_line(ui_SettingsCityEdit, true);

    ui_SettingsCityLabel = lv_label_create(ui_SettingsPanel);
    lv_obj_set_size(ui_SettingsCityLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(ui_SettingsCityLabel, 20, this->settings_panel_height / 5);
    lv_obj_set_align(ui_SettingsCityLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(ui_SettingsCityLabel, "City and\ncountry code");
    lv_obj_set_style_text_font(ui_SettingsCityLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_SettingsCityLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsSSIDEdit = lv_textarea_create(ui_SettingsPanel);
    lv_obj_set_size(ui_SettingsSSIDEdit, 250, LV_SIZE_CONTENT);
    lv_obj_set_pos(ui_SettingsSSIDEdit, 180, this->settings_panel_height / 5 + 60);
    lv_obj_set_align(ui_SettingsSSIDEdit, LV_ALIGN_TOP_LEFT);
    lv_textarea_set_max_length(ui_SettingsSSIDEdit, 50);
    lv_textarea_set_placeholder_text(ui_SettingsSSIDEdit, "WiFi SSID");
    lv_textarea_set_one_line(ui_SettingsSSIDEdit, true);

    ui_SettingsSSIDLabel = lv_label_create(ui_SettingsPanel);
    lv_obj_set_size(ui_SettingsSSIDLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(ui_SettingsSSIDLabel, 40, this->settings_panel_height / 5 + 70);
    lv_obj_set_align(ui_SettingsSSIDLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(ui_SettingsSSIDLabel, "WiFi SSID");
    lv_obj_set_style_text_font(ui_SettingsSSIDLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsPasswordEdit = lv_textarea_create(ui_SettingsPanel);
    lv_obj_set_size(ui_SettingsPasswordEdit, 250, LV_SIZE_CONTENT); /// 33
    lv_obj_set_pos(ui_SettingsPasswordEdit, 180, this->settings_panel_height / 5 + 60 * 2);
    lv_obj_set_align(ui_SettingsPasswordEdit, LV_ALIGN_TOP_LEFT);
    lv_textarea_set_max_length(ui_SettingsPasswordEdit, 100);
    lv_textarea_set_placeholder_text(ui_SettingsPasswordEdit, "WiFi Password");
    lv_textarea_set_one_line(ui_SettingsPasswordEdit, true);
    lv_textarea_set_password_mode(ui_SettingsPasswordEdit, true);

    ui_SettingsPasswordLabel = lv_label_create(ui_SettingsPanel);
    lv_obj_set_size(ui_SettingsPasswordLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(ui_SettingsPasswordLabel, 20, this->settings_panel_height / 5 + 65 * 2);
    lv_obj_set_align(ui_SettingsPasswordLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(ui_SettingsPasswordLabel, "WiFi Password");
    lv_obj_set_style_text_font(ui_SettingsPasswordLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsAPLabel = lv_label_create(ui_SettingsPanel);
    lv_obj_set_size(ui_SettingsAPLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(ui_SettingsAPLabel, 80, this->settings_panel_height - 60);
    lv_obj_set_align(ui_SettingsAPLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(ui_SettingsAPLabel, "AccessPoint IP:");
    lv_obj_set_style_text_font(ui_SettingsAPLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsIPLabel = lv_label_create(ui_SettingsPanel);
    lv_obj_set_size(ui_SettingsIPLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(ui_SettingsIPLabel, 230, this->settings_panel_height - 60);
    lv_obj_set_align(ui_SettingsIPLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(ui_SettingsIPLabel, "192.120.12.99");
    lv_obj_set_style_text_font(ui_SettingsIPLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsHomeButton = lv_btn_create(ui_SettingsPanel);
    lv_obj_set_size(ui_SettingsHomeButton, 35, 35);
    lv_obj_set_pos(ui_SettingsHomeButton, 401, -6);
    lv_obj_set_align(ui_SettingsHomeButton, LV_ALIGN_TOP_LEFT);

    ui_SettingsHomeButtonLabel = lv_label_create(ui_SettingsHomeButton);
    lv_obj_set_align(ui_SettingsHomeButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_SettingsHomeButtonLabel, LV_SYMBOL_HOME);
    lv_obj_set_style_text_align(ui_SettingsHomeButtonLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_theme_t * theme = lv_disp_get_theme(NULL);
    if (!theme->flags){
        lv_obj_add_state(ui_DarkmodeSwitch, LV_STATE_CHECKED);
    }
    lv_obj_add_event_cb(ui_DarkmodeSwitch, darkmode_switch_event_cb_wrapper, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SettingsCityEdit, settings_city_edit_event_cb_wrapper, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SettingsSSIDEdit, settings_SSID_edit_event_cb_wrapper, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SettingsPasswordEdit, settings_password_edit_event_cb_wrapper, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SettingsHomeButton, home_button_event_cb_wrapper, LV_EVENT_CLICKED, NULL);


}

void SettingsScreen::create_keyboard(lv_obj_t *target)
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

void SettingsScreen::delete_keyboard()
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

void SettingsScreen::keyboard_event_cb(lv_event_t *e){
    this->delete_keyboard();
}

void SettingsScreen::home_button_event_cb(lv_event_t *e){
    lv_obj_t *target = lv_event_get_target(e);
    lv_event_send(this->ui_SettingsKeyboard, LV_EVENT_CANCEL, NULL);
    lv_scr_load_anim(this->home_screen, LV_SCR_LOAD_ANIM_FADE_ON, SCREEN_CHANGE_ANIM_TIME, 0, true);
    this->~SettingsScreen();
}

void SettingsScreen::settings_city_edit_event_cb(lv_event_t *e){
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
        lv_textarea_set_text(this->ui_SettingsCityEdit, "ABOBA");
    }
}

void SettingsScreen::darkmode_switch_event_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_VALUE_CHANGED)
    {
        lv_disp_t *disp = lv_disp_get_default();
        if (lv_obj_has_state(target, LV_STATE_CHECKED))
        {
            lv_theme_t *theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_CYAN), lv_palette_main(LV_PALETTE_NONE),
                                                      false, LV_FONT_DEFAULT);
            lv_disp_set_theme(disp, theme);
            // lv_obj_set_style_text_color(ui_SettingsButtonLabel, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_set_style_shadow_opa(ui_SettingsButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_set_style_shadow_opa(ui_AlarmWorkingDayButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_set_style_shadow_opa(ui_AlarmWeekendDayButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_set_style_shadow_opa(ui_AlarmOneOffButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_set_style_text_color(ui_AlarmWorkingDayButtonLabel, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_set_style_text_color(ui_AlarmWeekendDayButtonLabel, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_set_style_text_color(ui_AlarmOneOffButtonLabel, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        else
        {
            lv_theme_t *theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_TEAL), lv_palette_main(LV_PALETTE_TEAL),
                                                      true, LV_FONT_DEFAULT);
            lv_disp_set_theme(disp, theme);
            // lv_obj_set_style_text_color(ui_SettingsButtonLabel, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_set_style_text_color(ui_AlarmWorkingDayButtonLabel, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_set_style_text_color(ui_AlarmWeekendDayButtonLabel, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_obj_set_style_text_color(ui_AlarmOneOffButtonLabel, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

void SettingsScreen::settings_SSID_edit_event_cb(lv_event_t *e)
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
        lv_textarea_set_text(this->ui_SettingsSSIDEdit, "ABOBA");
    }
}

void SettingsScreen::settings_password_edit_event_cb(lv_event_t *e)
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
        lv_textarea_set_text(this->ui_SettingsPasswordEdit, "ABOBA");
    }
}

SettingsScreen::~SettingsScreen()
{
    if (ui_SettingsScreen != NULL)
    {
        lv_obj_remove_event_cb(this->ui_DarkmodeSwitch, darkmode_switch_event_cb_wrapper);
        lv_obj_remove_event_cb(this->ui_SettingsCityEdit, settings_city_edit_event_cb_wrapper);
        lv_obj_remove_event_cb(this->ui_SettingsSSIDEdit, settings_SSID_edit_event_cb_wrapper);
        lv_obj_remove_event_cb(this->ui_SettingsPasswordEdit, settings_password_edit_event_cb_wrapper);
        lv_obj_remove_event_cb(this->ui_SettingsHomeButton, home_button_event_cb_wrapper);
        this->ui_SettingsScreen = NULL;
        this->ui_SettingsPanel = NULL;
        this->ui_DarkmodeLabel = NULL;
        this->ui_DarkmodeSwitch = NULL;
        this->ui_SettingsCityEdit = NULL;
        this->ui_SettingsCityLabel = NULL;
        this->ui_SettingsSSIDEdit = NULL;
        this->ui_SettingsSSIDLabel = NULL;
        this->ui_SettingsPasswordEdit = NULL;
        this->ui_SettingsPasswordLabel = NULL;
        this->ui_SettingsAPLabel = NULL;
        this->ui_SettingsIPLabel = NULL;
        this->ui_SettingsHomeButton = NULL;
        this->ui_SettingsHomeButtonLabel = NULL;
    }
}