#include "DockPanel.h"

DockPanel::DockPanel(lv_obj_t *parent_panel) {
    lv_obj_t *panel = parent_panel;
    // lv_obj_t *panel = lv_obj_get_child(lv_scr_act(), 0);
    batteryLabel = lv_label_create(panel);
    lv_obj_set_pos(batteryLabel, 10, 0);
    lv_obj_set_align(batteryLabel, LV_ALIGN_TOP_LEFT);
    lv_obj_set_size(batteryLabel, 40, 30);
    lv_obj_set_style_text_font(batteryLabel, &dock_panel_font_20, 0);

    WiFiLabel = lv_label_create(panel);
    lv_obj_set_pos(WiFiLabel, 45, 0);
    lv_obj_set_align(WiFiLabel, LV_ALIGN_TOP_LEFT);
    lv_obj_set_size(WiFiLabel, 40, 30);
    lv_obj_set_style_text_font(WiFiLabel, &wifi_symbols_20, 0);

    temperatureLabel = lv_label_create(panel);
    lv_obj_set_pos(temperatureLabel, 75, 0);
    lv_obj_set_align(temperatureLabel, LV_ALIGN_TOP_LEFT);
    lv_obj_set_size(temperatureLabel, 60, 30);
    lv_obj_set_style_text_font(temperatureLabel, &dock_panel_font_20, 0);

    humidityLabel = lv_label_create(panel);
    lv_obj_set_pos(humidityLabel, 135, 0);
    lv_obj_set_align(humidityLabel, LV_ALIGN_TOP_LEFT);
    lv_obj_set_size(humidityLabel, 60, 30);
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

    this->set_default_values();
}
void DockPanel::change_dock_parent(lv_obj_t *new_parent) {
    lv_obj_set_parent(this->settingsButton, lv_obj_get_child(new_parent, 0));
    lv_obj_set_parent(this->batteryLabel, lv_obj_get_child(new_parent, 0));
    lv_obj_set_parent(this->WiFiLabel, lv_obj_get_child(new_parent, 0));
    lv_obj_set_parent(this->temperatureLabel, lv_obj_get_child(new_parent, 0));
    lv_obj_set_parent(this->humidityLabel, lv_obj_get_child(new_parent, 0));
}
void DockPanel::show_wifi_connection(bool wifi_enabled) {
    if (!wifi_enabled) {
        lv_label_set_text(WiFiLabel, WIFI_DISCONNECTED_SYMBOL);
    } else {
        lv_label_set_text(WiFiLabel, WIFI_CONNECTED_SYMBOL);
    }
}
void DockPanel::set_temperature(char *temperature) {
    char *temp = strcat(temperature, " °C");
    lv_label_set_text(temperatureLabel, temp);
}
void DockPanel::set_humidity(char *humidity) {
    char *hum = strcat(humidity, "%");
    lv_label_set_text(humidityLabel, hum);
}
void DockPanel::set_default_values() {
    lv_label_set_text(batteryLabel, LV_SYMBOL_BATTERY_2);
    lv_label_set_text(WiFiLabel, WIFI_DISCONNECTED_SYMBOL);
    lv_label_set_text(temperatureLabel, "25 °C");
    lv_label_set_text(humidityLabel, "45%");
}
DockPanel::~DockPanel() {}
