#include "DockPanel.h"

static DockPanel *instance = NULL;

extern "C" void setBatteryLevelLabelCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    const int *payload = static_cast<const int *>(lv_msg_get_payload(msg));
    instance->setBatteryLevelLabel(*payload);
}

extern "C" void batteryChargingIconCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->setBatteryChargingIcon(true);
}

extern "C" void batteryIsNotChargingIconCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    lv_obj_add_flag(instance->chargingLabel, LV_OBJ_FLAG_HIDDEN);
    instance->setBatteryChargingIcon(false);
}

DockPanel::DockPanel(lv_obj_t *parentPanel) {
    instance = this;

    this->createDockPanelWidgets(parentPanel);
    this->setDefaultValues();

    lv_msg_subscribe(MSG_SHOW_BATTERY_LVL, setBatteryLevelLabelCallbackWrapper, NULL);
    lv_msg_subscribe(MSG_USB_CONNECTED, batteryChargingIconCallbackWrapper, NULL);
    lv_msg_subscribe(MSG_USB_DISCONNECTED, batteryIsNotChargingIconCallbackWrapper, NULL);
}

void DockPanel::hide() {
    lv_obj_add_flag(this->settingsButton, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(this->batteryLabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(this->wiFiLabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(this->chargingLabel, LV_OBJ_FLAG_HIDDEN);
}

void DockPanel::show() {
    lv_obj_clear_flag(this->settingsButton, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(this->batteryLabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(this->wiFiLabel, LV_OBJ_FLAG_HIDDEN);
    if (this->batteryCharging) {
        lv_obj_clear_flag(this->chargingLabel, LV_OBJ_FLAG_HIDDEN);
    }
}

void DockPanel::changeDockPanelParent(lv_obj_t *newParent) {
    lv_obj_set_parent(this->settingsButton, lv_obj_get_child(newParent, 0));
    lv_obj_set_parent(this->batteryLabel, lv_obj_get_child(newParent, 0));
    lv_obj_set_parent(this->chargingLabel, lv_obj_get_child(newParent, 0));
    lv_obj_set_parent(this->wiFiLabel, lv_obj_get_child(newParent, 0));
    lv_obj_set_parent(this->temperatureLabel, lv_obj_get_child(newParent, 0));
    lv_obj_set_parent(this->humidityLabel, lv_obj_get_child(newParent, 0));
}

void DockPanel::changeWifiConnectionIcon(bool wifiEnabled) {
    lv_label_set_text(wiFiLabel,
                      wifiEnabled ? WIFI_CONNECTED_SYMBOL : WIFI_DISCONNECTED_SYMBOL);
}

void DockPanel::setBatteryLevelLabel(int batteryLevel) {

    lv_label_set_text_fmt(batteryLabel, "%d %%", batteryLevel);
}

void DockPanel::setBatteryChargingIcon(bool charge) { this->batteryCharging = charge; }

void DockPanel::setTemperatureLabel(float temperature) {
    lv_label_set_text_fmt(temperatureLabel, "%.1f °C", temperature);
}

void DockPanel::setHumidityLabel(int humidity) {
    lv_label_set_text_fmt(humidityLabel, "%d %%", humidity);
}

void DockPanel::setDefaultValues() {
    lv_label_set_text(batteryLabel, "100%");
    lv_label_set_text(wiFiLabel, WIFI_DISCONNECTED_SYMBOL);
    lv_label_set_text(temperatureLabel, "-- °C");
    lv_label_set_text(humidityLabel, "-- %");
    lv_obj_add_flag(instance->chargingLabel, LV_OBJ_FLAG_HIDDEN);
}

DockPanel::~DockPanel() {}

void DockPanel::createDockPanelWidgets(lv_obj_t *panel) {
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

    wiFiLabel = lv_label_create(panel);
    lv_obj_set_pos(wiFiLabel, 379, 0);
    lv_obj_set_align(wiFiLabel, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_text_font(wiFiLabel, &wifi_symbols_20, 0);

    batteryLabel = lv_label_create(panel);
    lv_obj_align_to(batteryLabel, wiFiLabel, LV_ALIGN_OUT_LEFT_TOP, -30, -1);
    lv_obj_set_style_text_font(batteryLabel, &dock_panel_font_20, 0);

    chargingLabel = lv_label_create(panel);
    lv_obj_align_to(chargingLabel, batteryLabel, LV_ALIGN_OUT_LEFT_TOP, 7, 0);
    lv_obj_set_style_text_font(chargingLabel, &dock_panel_font_20, 0);
    lv_label_set_text(chargingLabel, CHARGE_SYMBOL);
}
