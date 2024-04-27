#include "BatteryApp.h"

static BatteryApp *instance = NULL;
extern "C" void measure_battery_level_cb_wrapper(lv_timer_t *timer) {
    instance->measure_battery_level_and_send_msg();
}
extern "C" void measure_battery_charge_cb_wrapper(lv_timer_t *timer) {
    instance->check_charge_status_and_send_msg();
    ;
}
void BatteryApp::measure_battery_level_and_send_msg() {
    int battery_level = this->calculate_battery_percentage(analogRead(BATTERY_LEVEL_PIN));
    this->_state_app->battery_state->battery_level = battery_level;
    lv_msg_send(MSG_SHOW_BATTERY_LVL, static_cast<const void *>(&battery_level));
}
int BatteryApp::calculate_battery_percentage(int battery_adc) {
    float battery_voltage =
        (analogReadMilliVolts(BATTERY_LEVEL_PIN) * DIVIDER_RATIO) / 1000.0f;
    Serial.println(analogReadMilliVolts(BATTERY_LEVEL_PIN));
    Serial.println(battery_voltage);
    if (battery_voltage <= BAT_MIN_VOLT) {
        return 0;
    } else if (battery_voltage >= BAT_MAX_VOLT) {
        return 100;
    } else {
        return this->sigmoidal(battery_voltage, BAT_MIN_VOLT, BAT_MAX_VOLT);
    }
}
void BatteryApp::check_charge_status_and_send_msg() {
    if (analogRead(BATTERY_CHARGE_PIN) > 2000) {
        if (!this->_state_app->battery_state->battery_charging) {
            this->_state_app->battery_state->battery_charging = true;
            lv_msg_send(MSG_USB_CONNECTED, NULL);
        }

    } else {
        if (this->_state_app->battery_state->battery_charging) {
            this->_state_app->battery_state->battery_charging = false;
            lv_msg_send(MSG_USB_DISCONNECTED, NULL);
        }
    }
}
uint8_t BatteryApp::sigmoidal(float voltage, float minVoltage,
                              float maxVoltage) {
    // slow
    // uint8_t result = 110 - (110 / (1 + pow(1.468 * (voltage - minVoltage)/(maxVoltage -
    // minVoltage), 6)));

    // steep
    // uint8_t result = 102 - (102 / (1 + pow(1.621 * (voltage - minVoltage)/(maxVoltage -
    // minVoltage), 8.1)));
    // normal
    uint8_t result =
        105 - (105 / (1 + pow(1.724 * (voltage - minVoltage) / (maxVoltage - minVoltage),
                              5.5)));
    return result >= 100 ? 100 : result;
}
void BatteryApp::setup_battery_app() {
    this->measure_battery_level_and_send_msg();
    this->check_charge_status_and_send_msg();
    this->battery_level_timer =
        lv_timer_create(measure_battery_level_cb_wrapper, 10000, NULL);
    this->battery_charge_timer =
        lv_timer_create(measure_battery_charge_cb_wrapper, 500, NULL);
}
BatteryApp::BatteryApp(StateApp *state_app) {
    instance = this;
    this->_state_app = state_app;
    pinMode(BATTERY_CHARGE_PIN, INPUT);
    pinMode(BATTERY_LEVEL_PIN, INPUT);
}

BatteryApp::~BatteryApp() {}