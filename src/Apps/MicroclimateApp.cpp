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
    temperatureSensor.begin(Wire, SHT41_I2C_ADDR_44);
    temperatureSensor.softReset();
    uint32_t serialNumber = 0;
    int error = temperatureSensor.serialNumber(serialNumber);
    if (error != NO_ERROR) {
        Serial.print("Error trying to begin temperature sensor: ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return false;
    }else
    {
        Serial.println("Temperature sensor is connected.");
    }
    

    lv_timer_resume(this->temperatureSensorTimer);
    return true;
}

void MicroclimateApp::temperatureSensorTimerCallback() {
    this->getSensorReadings();
}
MicroclimateApp::~MicroclimateApp() {}

void MicroclimateApp::getSensorReadings() {
    float temperature = 0.0f;
    float humidity = 0.0f;
    error = temperatureSensor.measureMediumPrecision(
        temperature,
        humidity);
    if (error != NO_ERROR) {
        Serial.print("Error trying to get temperature: ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    } else {
        temperature = temperature - TEMPERATURE_OFFSET;
        this->stateApp->microclimateState->indoorTemperature = temperature;
        this->stateApp->microclimateState->indoorHumidity = (unsigned int) humidity;
        this->dockPanel->setTemperatureLabel(
            this->stateApp->microclimateState->indoorTemperature);
        this->dockPanel->setHumidityLabel(
            this->stateApp->microclimateState->indoorHumidity);
    }
}
