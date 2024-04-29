#include "MicroclimateApp.h"
static MicroclimateApp *instance = NULL;
extern "C" void bme_timer_cb_wrapper(lv_timer_t *timer) { instance->bme_timer_cb(timer); }

float MicroclimateApp::get_temperature() {
    return this->_bme_sensor.readTemperature() - TEMPERATURE_OFFSET;
}

int MicroclimateApp::get_humidity() {
    return static_cast<int>(this->_bme_sensor.readHumidity());
}

void MicroclimateApp::bme_timer_cb(lv_timer_t *timer) {
    if (_bme_sensor.takeForcedMeasurement()) {
        this->_dock_panel->set_temperature(this->get_temperature());
        this->_dock_panel->set_humidity(this->get_humidity());
    }
}
bool MicroclimateApp::begin() {
    if (!_bme_sensor.begin(0x76)) {
        return false;
    } else {
        Serial.println("BME280 sensor is connected");
        _bme_sensor.setSampling(Adafruit_BME280::MODE_FORCED,
                                Adafruit_BME280::SAMPLING_X1,
                                Adafruit_BME280::SAMPLING_NONE,
                                Adafruit_BME280::SAMPLING_X1,
                                Adafruit_BME280::FILTER_OFF);
        return true;
    }
}
MicroclimateApp::MicroclimateApp(DockPanel *dock_panel) {
    instance = this;
    this->_dock_panel = dock_panel;
    this->_bme_timer = lv_timer_create(bme_timer_cb_wrapper, 10000, NULL);
}

MicroclimateApp::~MicroclimateApp() {}