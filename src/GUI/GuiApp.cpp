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
extern "C" void darkmode_switch_event_cb_wrapper(lv_event_t *e) {
    instance->darkmode_switch_event_cb(e);
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
GuiApp::GuiApp(/* args */) {
    instance = this;
    alarm_clock = new AlarmClock();
    digital_clock = new DigitalClock();
    analog_clock = new AnalogClock();
    weather = new Weather();
    settings = new Settings();
    _screen_timer = NULL;
    _dock_panel_timer =
        lv_timer_create(dock_panel_timer_cb_wrapper, DOCK_PANEL_HIDE_PERIOD, NULL);
    lv_timer_set_repeat_count(this->_dock_panel_timer, 1);
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
    lv_obj_add_event_cb(settings->darkmodeSwitch, darkmode_switch_event_cb_wrapper,
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
};

void GuiApp::init_gui() { lv_scr_load(digital_clock->digitalClockScreen); };

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

void GuiApp::switch_darktheme(bool darktheme_enabled) {
    lv_disp_t *disp = lv_disp_get_default();
    if (darktheme_enabled) {
        this->set_dark_theme(disp);
    } else {
        this->set_light_theme(disp);
    }
}

void GuiApp::screen_load_event_cb(lv_event_t *e) {
    if (lv_scr_act() != this->settings->settingsScreen) {
        this->dock_panel->change_dock_parent(lv_scr_act());
    }
    if (this->_screen_timer != NULL) {
        lv_timer_del(this->_screen_timer);
        this->_screen_timer = NULL;
    }
    if (lv_scr_act() != instance->analog_clock->analogClockScreen &
        lv_scr_act() != instance->digital_clock->digitalClockScreen) {
        this->_screen_timer =
            lv_timer_create(screen_timer_cb_wrapper, SCREEN_SWAP_PERIOD, NULL);
        lv_timer_set_repeat_count(this->_screen_timer, 1);
    }
}

void GuiApp::user_activity_event_cb(lv_event_t *e) {
    Serial.println("Timer was reset");
    if (this->_screen_timer != NULL) {
        lv_timer_reset(this->_screen_timer);
    }
    if (this->_dock_panel_timer == NULL) {
        this->_dock_panel_timer =
            lv_timer_create(dock_panel_timer_cb_wrapper, DOCK_PANEL_HIDE_PERIOD, NULL);
        lv_timer_set_repeat_count(this->_dock_panel_timer, 1);
    }
    if (this->_dock_panel_timer != NULL) {
        this->dock_panel->show();
        lv_timer_reset(this->_dock_panel_timer);
    }
}

void GuiApp::screen_timer_cb(lv_timer_t *timer) {
    Serial.println("Screen timer callback fired");
    this->_screen_timer = NULL;
    if (lv_scr_act() != this->analog_clock->analogClockScreen &
        lv_scr_act() != this->digital_clock->digitalClockScreen) {
        Serial.println("Screen was swapped to DigitalClock");
        lv_scr_load_anim(this->digital_clock->digitalClockScreen,
                         LV_SCR_LOAD_ANIM_FADE_IN, SCREEN_CHANGE_ANIM_TIME, 0, false);
    }
}

void GuiApp::dock_panel_timer_cb(lv_timer_t *timer) {
    Serial.println("Dock panel timer callback fired");
    lv_timer_del(this->_dock_panel_timer);
    this->_dock_panel_timer = NULL;
    this->dock_panel->hide();
}

void GuiApp::settings_button_event_cb(lv_event_t *e) {
    settings->load_settings_screen(lv_scr_act());
}

void GuiApp::darkmode_switch_event_cb(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    lv_disp_t *disp = lv_disp_get_default();
    if (event_code == LV_EVENT_VALUE_CHANGED) {
        if (lv_obj_has_state(target, LV_STATE_CHECKED)) {
            this->set_dark_theme(disp);
        } else {
            this->set_light_theme(disp);
        }
        this->settings->save_darkmode_to_nvs();
    }
}
void GuiApp::set_light_theme(lv_disp_t *display) {
    lv_theme_t *theme =
        lv_theme_default_init(display, lv_palette_main(LV_PALETTE_CYAN),
                              lv_palette_main(LV_PALETTE_CYAN), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(display, theme);

    lv_obj_set_style_text_color(dock_panel->settingsButtonLabel, lv_color_black(), 0);
    lv_obj_set_style_shadow_opa(dock_panel->settingsButton, 0, 0);
    lv_obj_set_style_shadow_opa(alarm_clock->weekdaysButton, 0, 0);
    lv_obj_set_style_shadow_opa(alarm_clock->weekendsButton, 0, 0);
    lv_obj_set_style_shadow_opa(alarm_clock->oneOffButton, 0, 0);
    lv_obj_set_style_text_color(alarm_clock->weekdaysButtonLabel, lv_color_black(), 0);
    lv_obj_set_style_text_color(alarm_clock->weekendsButtonLabel, lv_color_black(), 0);
    lv_obj_set_style_text_color(alarm_clock->oneOffButtonLabel, lv_color_black(), 0);
}
void GuiApp::set_dark_theme(lv_disp_t *display) {
    lv_theme_t *theme =
        lv_theme_default_init(display, lv_palette_main(LV_PALETTE_TEAL),
                              lv_palette_main(LV_PALETTE_TEAL), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(display, theme);
    lv_obj_set_style_text_color(dock_panel->settingsButtonLabel, lv_color_white(), 0);
    lv_obj_set_style_text_color(alarm_clock->weekdaysButtonLabel, lv_color_white(), 0);
    lv_obj_set_style_text_color(alarm_clock->weekendsButtonLabel, lv_color_white(), 0);
    lv_obj_set_style_text_color(alarm_clock->oneOffButtonLabel, lv_color_white(), 0);
}
void GuiApp::create_loading_screen() {
    loading_screen = lv_obj_create(NULL);
    loading_spinner = lv_spinner_create(loading_screen, 500, 60);
    lv_obj_set_size(loading_spinner, 100, 100);
    lv_obj_center(loading_spinner);
    lv_scr_load(loading_screen);
}
void GuiApp::delete_loading_screen() { lv_obj_del(loading_screen); }
void GuiApp::set_screens_to_default_values() {
    this->digital_clock->set_default_values();
    this->analog_clock->set_default_values();
    this->weather->set_no_data_values();
}
GuiApp::~GuiApp() {}