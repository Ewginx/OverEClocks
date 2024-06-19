#include "DockPanel.h"

static DockPanel *instance = NULL;

extern "C" void set_battery_level_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    const int *payload = static_cast<const int *>(lv_msg_get_payload(msg));
    instance->set_battery_level(*payload);
}
extern "C" void battery_charging_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->set_battery_charging(true);
}
extern "C" void battery_not_charging_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    lv_obj_add_flag(instance->chargingLabel, LV_OBJ_FLAG_HIDDEN);
    instance->set_battery_charging(false);
}

DockPanel::DockPanel(lv_obj_t *parent_panel) {
    instance = this;
    lv_obj_t *panel = parent_panel;

    temperatureLabel = lv_label_create(panel);
    lv_obj_set_pos(temperatureLabel, 5, 0);
    lv_obj_set_align(temperatureLabel, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_text_font(temperatureLabel, &dock_panel_font_20, 0);

    humidityLabel = lv_label_create(panel);
    lv_obj_align_to(humidityLabel, temperatureLabel, LV_ALIGN_OUT_RIGHT_TOP, 25, -1);
    lv_obj_set_style_text_font(humidityLabel, &dock_panel_font_20, 0);

    settingsButton = lv_btn_create(panel);
    lv_obj_set_size(settingsButton, 35, 35);
    lv_obj_set_pos(settingsButton, 401, -6);
    lv_obj_set_align(settingsButton, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_bg_opa(settingsButton, 0, 0);

    settingsButtonLabel = lv_label_create(settingsButton);
    lv_obj_set_align(settingsButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(settingsButtonLabel, LV_SYMBOL_SETTINGS);
    lv_obj_set_style_text_align(settingsButtonLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(settingsButtonLabel, &dock_panel_font_20, 0);

    WiFiLabel = lv_label_create(panel);
    lv_obj_set_pos(WiFiLabel, 379, 0);
    lv_obj_set_align(WiFiLabel, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_text_font(WiFiLabel, &wifi_symbols_20, 0);

    batteryLabel = lv_label_create(panel);
    lv_obj_align_to(batteryLabel, WiFiLabel, LV_ALIGN_OUT_LEFT_TOP, -30, -1);
    lv_obj_set_style_text_font(batteryLabel, &dock_panel_font_20, 0);

    chargingLabel = lv_label_create(panel);
    lv_obj_align_to(chargingLabel, batteryLabel, LV_ALIGN_OUT_LEFT_TOP, 7, 0);
    lv_obj_set_style_text_font(chargingLabel, &dock_panel_font_20, 0);
    lv_label_set_text(chargingLabel, CHARGE_SYMBOL);

    this->set_default_values();
    lv_msg_subscribe(MSG_SHOW_BATTERY_LVL, set_battery_level_cb_wrapper, NULL);
    lv_msg_subscribe(MSG_USB_CONNECTED, battery_charging_cb_wrapper, NULL);
    lv_msg_subscribe(MSG_USB_DISCONNECTED, battery_not_charging_cb_wrapper, NULL);
}

void DockPanel::hide() {
    lv_obj_add_flag(this->settingsButton, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(this->batteryLabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(this->WiFiLabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(this->chargingLabel, LV_OBJ_FLAG_HIDDEN);
}

void DockPanel::show() {
    lv_obj_clear_flag(this->settingsButton, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(this->batteryLabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(this->WiFiLabel, LV_OBJ_FLAG_HIDDEN);
    if (this->battery_charging) {
        lv_obj_clear_flag(this->chargingLabel, LV_OBJ_FLAG_HIDDEN);
    }
}

void DockPanel::change_dock_parent(lv_obj_t *new_parent) {
    lv_obj_set_parent(this->settingsButton, lv_obj_get_child(new_parent, 0));
    lv_obj_set_parent(this->batteryLabel, lv_obj_get_child(new_parent, 0));
    lv_obj_set_parent(this->chargingLabel, lv_obj_get_child(new_parent, 0));
    lv_obj_set_parent(this->WiFiLabel, lv_obj_get_child(new_parent, 0));
    lv_obj_set_parent(this->temperatureLabel, lv_obj_get_child(new_parent, 0));
    lv_obj_set_parent(this->humidityLabel, lv_obj_get_child(new_parent, 0));
}

void DockPanel::show_wifi_connection(bool wifi_enabled) {
    lv_label_set_text(WiFiLabel,
                      wifi_enabled ? WIFI_CONNECTED_SYMBOL : WIFI_DISCONNECTED_SYMBOL);
}

void DockPanel::set_battery_level(int battery_level) {

    lv_label_set_text_fmt(batteryLabel, "%d %%", battery_level);
}

void DockPanel::set_battery_charging(bool charge) { this->battery_charging = charge; }

void DockPanel::set_temperature(float temperature) {
    lv_label_set_text_fmt(temperatureLabel, "%.1f °C", temperature);
}

void DockPanel::set_humidity(int humidity) {
    lv_label_set_text_fmt(humidityLabel, "%d %%", humidity);
}

void DockPanel::set_default_values() {
    lv_label_set_text(batteryLabel, "100%");
    lv_label_set_text(WiFiLabel, WIFI_DISCONNECTED_SYMBOL);
    lv_label_set_text(temperatureLabel, "-- °C");
    lv_label_set_text(humidityLabel, "-- %");
    lv_obj_add_flag(instance->chargingLabel, LV_OBJ_FLAG_HIDDEN);
}

DockPanel::~DockPanel() {}
