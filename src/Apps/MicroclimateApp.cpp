#include "MicroclimateApp.h"

static MicroclimateApp *instance = NULL;

extern "C" void temperatureSensorTimerCallbackWrapper(lv_timer_t *timer) {
    instance->temperatureSensorTimerCallback();
}

MicroclimateApp::MicroclimateApp(DockPanel *dockPanel, StateApp *stateApp) {
    instance = this;
    this->stateApp = stateApp;
    this->dockPanel = dockPanel;
    this->temperatureSensorTimer =
        lv_timer_create(temperatureSensorTimerCallbackWrapper, 10000, NULL);
    lv_timer_pause(this->temperatureSensorTimer);
}

bool MicroclimateApp::begin() {
    if (!temperatureSensor.begin()) {
        Serial.println("Can't find the temp sensor");
        return false;
    };
    Serial.println("Temp sensor is connected");
    uint16_t status = temperatureSensor.readStatus();
    Serial.print("Temp sensor status: ");
    Serial.println(status, HEX);
    temperatureSensor.requestData();
    lv_timer_resume(this->temperatureSensorTimer);
    return true;
}

float MicroclimateApp::getTemperature() {
    return this->temperatureSensor.getTemperature() - TEMPERATURE_OFFSET;
}

int MicroclimateApp::getHumidity() {
    return static_cast<int>(this->temperatureSensor.getHumidity());
}

void MicroclimateApp::temperatureSensorTimerCallback() {
    if (this->temperatureSensor.dataReady()) {
        bool success = this->temperatureSensor.readData();
        this->temperatureSensor.requestData();
        if (success) {
            this->stateApp->microclimateState->indoor_humidity = this->getHumidity();
            this->stateApp->microclimateState->indoor_temperature =
                this->getTemperature();
            this->dockPanel->set_temperature(
                this->stateApp->microclimateState->indoor_temperature);
            this->dockPanel->set_humidity(
                this->stateApp->microclimateState->indoor_humidity);
        } else {
            Serial.println("Failed read");
        }
    }
}
MicroclimateApp::~MicroclimateApp() {}