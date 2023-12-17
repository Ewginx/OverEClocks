#include "MicroclimateApp.h"
static MicroclimateApp *instance = NULL;
extern "C" void bme_timer_cb_wrapper(lv_timer_t *timer) { instance->bme_timer_cb(timer); }

void MicroclimateApp::bme_timer_cb(lv_timer_t *timer) {
    this->_dock_panel->set_temperature(this->_bme_sensor.readTemperature() - 1);
    this->_dock_panel->set_humidity(static_cast<int>(this->_bme_sensor.readHumidity()));
}
bool MicroclimateApp::begin() {
    if (!_bme_sensor.begin(0x76)) {
        return false;
    } else {
        Serial.println("BME280 sensor is connected");
        return true;
    }
}
MicroclimateApp::MicroclimateApp(DockPanel *dock_panel) {
    instance = this;
    this->_dock_panel = dock_panel;
    this->_bme_timer = lv_timer_create(bme_timer_cb_wrapper, 600, NULL);
}

MicroclimateApp::~MicroclimateApp() {}