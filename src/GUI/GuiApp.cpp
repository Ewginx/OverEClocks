#include "GuiApp.h"

static GuiApp *instance = NULL;

extern "C" void swipe_screen_event_cb_wrapper(lv_event_t *e) {
    instance->swipe_screen_event_cb(e);
}

extern "C" void screen_load_event_cb_wrapper(lv_event_t *e) {
    instance->screen_load_event_cb(e);
}
extern "C" void settings_button_event_cb_wrapper(lv_event_t *e) {
    instance->settings_button_event_cb(e);
}
extern "C" void theme_switch_event_cb_wrapper(lv_event_t *e) {
    instance->theme_switch_event_cb(e);
}
extern "C" void screen_timer_cb_wrapper(lv_timer_t *timer) {
    instance->screen_timer_cb(timer);
}
extern "C" void dock_panel_timer_cb_wrapper(lv_timer_t *timer) {
    instance->dock_panel_timer_cb(timer);
}
extern "C" void user_activity_event_cb_wrapper(lv_event_t *e) {
    instance->user_activity_event_cb(e);
}
extern "C" void change_theme_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    const bool *payload = static_cast<const bool *>(lv_msg_get_payload(msg));
    instance->switch_theme(*payload);
}
extern "C" void async_update_gif_src_cb(void *data) {
    instance->update_gif_img_src();
}
extern "C" void update_gif_src_cb(void *subscriber, lv_msg_t *msg) {
    lv_async_call(async_update_gif_src_cb, NULL);
}
extern "C" void async_update_clock_img_src_cb(void *data) {
    instance->update_clock_img_src();
}
extern "C" void update_clock_img_src_cb(void *subscriber, lv_msg_t *msg) {
    lv_async_call(async_update_clock_img_src_cb, NULL);
}

GuiApp::GuiApp(StateApp *state_app) {
    instance = this;
    this->state_app = state_app;
    this->state_app->theme_state->dark_theme_enabled ? this->init_dark_theme()
                                                     : this->init_light_theme();
    alarm_clock = new AlarmClock(this->state_app);
    digital_clock = new DigitalClock();
    analog_clock = new AnalogClock();
    weather = new Weather();
    settings = new Settings(this->state_app);

    this->_screen_timer =
        lv_timer_create(screen_timer_cb_wrapper, SCREEN_SWAP_PERIOD, NULL);
    lv_timer_pause(this->_screen_timer);

    _dock_panel_timer =
        lv_timer_create(dock_panel_timer_cb_wrapper, DOCK_PANEL_HIDE_PERIOD, NULL);

    dock_panel = new DockPanel(digital_clock->digitalClockPanel);
    lv_obj_add_event_cb(digital_clock->digitalClockScreen, swipe_screen_event_cb_wrapper,
                        LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(weather->weatherScreen, swipe_screen_event_cb_wrapper,
                        LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(analog_clock->analogClockScreen, swipe_screen_event_cb_wrapper,
                        LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(alarm_clock->alarmScreen, swipe_screen_event_cb_wrapper,
                        LV_EVENT_GESTURE, NULL);

    lv_obj_add_event_cb(digital_clock->digitalClockScreen, screen_load_event_cb_wrapper,
                        LV_EVENT_SCREEN_LOADED, NULL);
    lv_obj_add_event_cb(weather->weatherScreen, screen_load_event_cb_wrapper,
                        LV_EVENT_SCREEN_LOADED, NULL);
    lv_obj_add_event_cb(analog_clock->analogClockScreen, screen_load_event_cb_wrapper,
                        LV_EVENT_SCREEN_LOADED, NULL);
    lv_obj_add_event_cb(alarm_clock->alarmScreen, screen_load_event_cb_wrapper,
                        LV_EVENT_SCREEN_LOADED, NULL);
    lv_obj_add_event_cb(settings->settingsScreen, screen_load_event_cb_wrapper,
                        LV_EVENT_SCREEN_LOADED, NULL);

    lv_obj_add_event_cb(dock_panel->settingsButton, settings_button_event_cb_wrapper,
                        LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(settings->themeSwitch, theme_switch_event_cb_wrapper,
                        LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_add_event_cb(alarm_clock->alarmScreen, user_activity_event_cb_wrapper,
                        LV_EVENT_PRESSING, NULL);
    lv_obj_add_event_cb(weather->weatherScreen, user_activity_event_cb_wrapper,
                        LV_EVENT_PRESSING, NULL);
    lv_obj_add_event_cb(settings->settingsScreen, user_activity_event_cb_wrapper,
                        LV_EVENT_PRESSING, NULL);
    lv_obj_add_event_cb(analog_clock->analogClockScreen, user_activity_event_cb_wrapper,
                        LV_EVENT_PRESSING, NULL);
    lv_obj_add_event_cb(digital_clock->digitalClockScreen, user_activity_event_cb_wrapper,
                        LV_EVENT_PRESSING, NULL);
    lv_msg_subscribe(MSG_CHANGE_THEME, change_theme_cb_wrapper, NULL);
    lv_msg_subscribe(MSG_UPDATE_GIF_SRC, update_gif_src_cb, NULL);
    lv_msg_subscribe(MSG_UPDATE_CLOCK_IMG_SRC, update_clock_img_src_cb, NULL);

    this->dock_panel->show();
};

void GuiApp::setup_gui() {
    this->switch_theme(this->state_app->theme_state->dark_theme_enabled);
    this->alarm_clock->set_alarm_clock_gui();
    this->settings->set_wifi_settings(this->state_app->wifi_state->ssid.c_str(),
                                      this->state_app->wifi_state->password.c_str());
    this->settings->set_weather_settings(
        this->state_app->weather_state->city.c_str(),
        this->state_app->weather_state->language.c_str());
    this->settings->set_brightness_slider(
        this->state_app->display_state->brightness_level);
    this->settings->set_brightness_checkbox(
        this->state_app->display_state->auto_brightness);
    this->settings->set_theme_switch(this->state_app->theme_state->dark_theme_enabled);
    this->create_gif_img();
}

void GuiApp::create_gif_img() {
    this->gif_image = lv_gif_create(this->state_app->display_state->digital_main_screen
                                        ? this->digital_clock->digitalClockPanel
                                        : this->analog_clock->analogClockPanel);
    lv_gif_set_src(this->gif_image, "F:/gif/gif.gif");
    lv_obj_set_width(this->gif_image, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(this->gif_image, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(this->gif_image, LV_ALIGN_TOP_LEFT);
    lv_obj_set_pos(this->gif_image, 20, 210);
}

void GuiApp::set_gif_parent() {
    lv_obj_set_parent(this->gif_image, this->state_app->display_state->digital_main_screen
                                           ? this->digital_clock->digitalClockPanel
                                           : this->analog_clock->analogClockPanel);
}

void GuiApp::update_gif_img_src() {
    lv_gif_set_src(this->gif_image, "F:/gif/gif.gif");
    lv_gif_restart(this->gif_image);
}

void GuiApp::update_clock_img_src() {
    this->analog_clock->set_analog_clock_img_src();
}

void GuiApp::load_default_screen() {
    if (this->state_app->display_state->digital_main_screen) {
        lv_scr_load(digital_clock->digitalClockScreen);
    } else {
        lv_scr_load(analog_clock->analogClockScreen);
    }
};

void GuiApp::swipe_screen_event_cb(lv_event_t *e) {
    lv_obj_t *target = lv_event_get_target(e);
    if (target == this->digital_clock->digitalClockScreen) {
        this->swipe_digital_clock_screen();
    } else if (target == this->weather->weatherScreen) {
        this->swipe_weather_screen();
    } else if (target == this->analog_clock->analogClockScreen) {
        this->swipe_analog_clock_screen();
    } else if (target == this->alarm_clock->alarmScreen) {
        this->swipe_alarm_screen();
    }
}

void GuiApp::swipe_digital_clock_screen() {
    if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_scr_load_anim(this->weather->weatherScreen, LV_SCR_LOAD_ANIM_MOVE_LEFT,
                         SCREEN_CHANGE_ANIM_TIME, 0, false);
    } else if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_scr_load_anim(this->analog_clock->analogClockScreen,
                         LV_SCR_LOAD_ANIM_MOVE_RIGHT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
}

void GuiApp::swipe_analog_clock_screen() {
    if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_scr_load_anim(this->digital_clock->digitalClockScreen,
                         LV_SCR_LOAD_ANIM_MOVE_LEFT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    } else if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_scr_load_anim(this->alarm_clock->alarmScreen, LV_SCR_LOAD_ANIM_MOVE_RIGHT,
                         SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
}
void GuiApp::swipe_weather_screen() {
    if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_scr_load_anim(this->digital_clock->digitalClockScreen,
                         LV_SCR_LOAD_ANIM_MOVE_RIGHT, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
}

void GuiApp::swipe_alarm_screen() {
    if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_scr_load_anim(this->analog_clock->analogClockScreen,
                         LV_SCR_LOAD_ANIM_MOVE_LEFT, SCREEN_CHANGE_ANIM_TIME, 0, false);
        this->alarm_clock->delete_roller_modal_panel();
    }
}

void GuiApp::switch_theme(bool darktheme_enabled) {
    if (darktheme_enabled) {
        this->switch_to_dark_theme();
    } else {
        this->switch_to_light_theme();
    }
}

void GuiApp::screen_load_event_cb(lv_event_t *e) {
    if (lv_scr_act() != this->settings->settingsScreen) {
        this->dock_panel->change_dock_parent(lv_scr_act());
    }
    if (lv_scr_act() != this->analog_clock->analogClockScreen &
        lv_scr_act() != this->digital_clock->digitalClockScreen) {
        lv_timer_reset(this->_screen_timer);
        lv_timer_resume(this->_screen_timer);
    } else {
        lv_timer_reset(this->_screen_timer);
        lv_timer_pause(this->_screen_timer);
    }
}

void GuiApp::user_activity_event_cb(lv_event_t *e) {
    lv_timer_reset(this->_screen_timer);
    lv_timer_resume(this->_screen_timer);
    lv_timer_reset(this->_dock_panel_timer);
    lv_timer_resume(this->_dock_panel_timer);
    this->dock_panel->show();
}

void GuiApp::screen_timer_cb(lv_timer_t *timer) {
    if (lv_scr_act() != this->analog_clock->analogClockScreen &
        lv_scr_act() != this->digital_clock->digitalClockScreen) {
        if (this->state_app->display_state->digital_main_screen) {
            lv_scr_load_anim(this->digital_clock->digitalClockScreen,
                             LV_SCR_LOAD_ANIM_FADE_IN, SCREEN_CHANGE_ANIM_TIME, 0, false);
        } else {
            lv_scr_load_anim(this->analog_clock->analogClockScreen,
                             LV_SCR_LOAD_ANIM_FADE_IN, SCREEN_CHANGE_ANIM_TIME, 0, false);
        }
    }
}

void GuiApp::dock_panel_timer_cb(lv_timer_t *timer) {
    lv_timer_reset(this->_dock_panel_timer);
    lv_timer_pause(this->_dock_panel_timer);
    this->dock_panel->hide();
}

void GuiApp::settings_button_event_cb(lv_event_t *e) {
    settings->load_settings_screen(lv_scr_act());
}

void GuiApp::theme_switch_event_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    lv_disp_t *disp = lv_disp_get_default();
    if (event_code == LV_EVENT_VALUE_CHANGED) {
        this->switch_theme(lv_obj_has_state(target, LV_STATE_CHECKED));
        this->state_app->theme_state->save_dark_theme_enabled(
            lv_obj_has_state(target, LV_STATE_CHECKED));
    }
}
void GuiApp::switch_to_light_theme() {
    this->init_light_theme();

    lv_obj_set_style_text_color(dock_panel->settingsButtonLabel, lv_color_black(), 0);
    lv_obj_set_style_shadow_opa(dock_panel->settingsButton, 0, 0);
    lv_obj_set_style_shadow_opa(alarm_clock->weekdaysButton, 0, 0);
    lv_obj_set_style_shadow_opa(alarm_clock->weekendsButton, 0, 0);
    lv_obj_set_style_shadow_opa(alarm_clock->oneOffButton, 0, 0);
    lv_obj_set_style_text_color(alarm_clock->weekdaysButtonLabel, lv_color_black(), 0);
    lv_obj_set_style_text_color(alarm_clock->weekendsButtonLabel, lv_color_black(), 0);
    lv_obj_set_style_text_color(alarm_clock->oneOffButtonLabel, lv_color_black(), 0);
}
void GuiApp::switch_to_dark_theme() {
    this->init_dark_theme();

    lv_obj_set_style_text_color(dock_panel->settingsButtonLabel, lv_color_white(), 0);
    lv_obj_set_style_text_color(alarm_clock->weekdaysButtonLabel, lv_color_white(), 0);
    lv_obj_set_style_text_color(alarm_clock->weekendsButtonLabel, lv_color_white(), 0);
    lv_obj_set_style_text_color(alarm_clock->oneOffButtonLabel, lv_color_white(), 0);
}
void GuiApp::init_light_theme() {
    lv_color_t primary_color =
        lv_color_hex(this->state_app->theme_state->light_primary_color);
    lv_color_t second_color =
        lv_color_hex(this->state_app->theme_state->light_second_color);
    lv_color_t color_screen =
        lv_color_hex(this->state_app->theme_state->light_screen_color);
    lv_color_t color_card = lv_color_hex(this->state_app->theme_state->light_card_color);
    lv_color_t color_text = lv_color_hex(this->state_app->theme_state->light_text_color);
    lv_color_t color_grey = lv_color_hex(this->state_app->theme_state->light_grey_color);

    lv_theme_t *theme =
        theme_custom_init(NULL, primary_color, second_color, color_screen, color_card,
                          color_text, color_grey, true, LV_FONT_DEFAULT);
    lv_disp_set_theme(NULL, theme);
}
void GuiApp::init_dark_theme() {
    lv_color_t primary_color =
        lv_color_hex(this->state_app->theme_state->dark_primary_color);
    lv_color_t second_color =
        lv_color_hex(this->state_app->theme_state->dark_second_color);
    lv_color_t color_screen =
        lv_color_hex(this->state_app->theme_state->dark_screen_color);
    lv_color_t color_card = lv_color_hex(this->state_app->theme_state->dark_card_color);
    lv_color_t color_text = lv_color_hex(this->state_app->theme_state->dark_text_color);
    lv_color_t color_grey = lv_color_hex(this->state_app->theme_state->dark_grey_color);

    lv_theme_t *theme =
        theme_custom_init(NULL, primary_color, second_color, color_screen, color_card,
                          color_text, color_grey, true, LV_FONT_DEFAULT);
    lv_disp_set_theme(NULL, theme);
}
void GuiApp::create_loading_screen() {
    loading_screen = lv_obj_create(NULL);
    loading_spinner = lv_spinner_create(loading_screen, 500, 60);
    lv_obj_set_size(loading_spinner, 100, 100);
    lv_obj_center(loading_spinner);
    lv_scr_load(loading_screen);
}
void GuiApp::delete_loading_screen() { lv_obj_del(loading_screen); }
GuiApp::~GuiApp() {}