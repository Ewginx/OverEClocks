#include "MicroclimateApp.h"
static MicroclimateApp *instance = NULL;
extern "C" void temp_sensor_timer_cb_wrapper(lv_timer_t *timer) {
    instance->temp_sensor_timer_cb(timer);
}

float MicroclimateApp::get_temperature() { return this->_temp_sensor.getTemperature() - TEMPERATURE_OFFSET; }

int MicroclimateApp::get_humidity() {
    return static_cast<int>(this->_temp_sensor.getHumidity());
}

void MicroclimateApp::temp_sensor_timer_cb(lv_timer_t *timer) {
    if (this->_temp_sensor.dataReady()) {
        bool success = this->_temp_sensor.readData();
        this->_temp_sensor.requestData();
        if (success) {
            this->_state_app->microclimate_state->indoor_humidity = this->get_humidity();
            this->_state_app->microclimate_state->indoor_temperature =
                this->get_temperature();
            this->_dock_panel->set_temperature(
                this->_state_app->microclimate_state->indoor_temperature);
            this->_dock_panel->set_humidity(
                this->_state_app->microclimate_state->indoor_humidity);
        } else {
            Serial.println("Failed read");
        }
    }
}
bool MicroclimateApp::begin() {
    if (!_temp_sensor.begin()) {
        Serial.println("Can't find the temp sensor");
        return false;
    };
    Serial.println("Temp sensor is connected");
    uint16_t stat = _temp_sensor.readStatus();
    Serial.print("Temp sensor status: ");
    Serial.println(stat, HEX);
    _temp_sensor.requestData();
    lv_timer_resume(this->_temp_sensor_timer);
    return true;
}
MicroclimateApp::MicroclimateApp(DockPanel *dock_panel, StateApp *state_app) {
    instance = this;
    this->_state_app = state_app;
    this->_dock_panel = dock_panel;
    this->_temp_sensor_timer = lv_timer_create(temp_sensor_timer_cb_wrapper, 10000, NULL);
    lv_timer_pause(this->_temp_sensor_timer);
}

MicroclimateApp::~MicroclimateApp() {}