#include "BatteryApp.h"

static BatteryApp *instance = NULL;

extern "C" void measureBatteryLevelCallbackWrapper(lv_timer_t *timer) {
    instance->measureBatteryLevelAndSendMsg();
}
extern "C" void batteryChargeStatusCallbackWrapper(lv_timer_t *timer) {
    instance->checkChargeStatusAndSendMsg();
}

BatteryApp::BatteryApp(StateApp *stateApp) {
    instance = this;
    this->stateApp = stateApp;

    pinMode(BATTERY_CHARGE_PIN, INPUT);
    pinMode(BATTERY_LEVEL_PIN, INPUT);
}

void BatteryApp::setup() {
    this->measureBatteryLevelAndSendMsg();
    this->checkChargeStatusAndSendMsg();
    this->batteryLevelTimer =
        lv_timer_create(measureBatteryLevelCallbackWrapper, 2000, NULL);
    this->batteryChargingTimer =
        lv_timer_create(batteryChargeStatusCallbackWrapper, 700, NULL);
}

void BatteryApp::measureBatteryLevelAndSendMsg() {
    if (voltageSamplesCount > 4) {
        stateApp->batteryState->batteryVoltage = this->calculateBatteryVoltage();
        int batteryLevel =
            this->calculateBatteryPercentage(stateApp->batteryState->batteryVoltage);
        stateApp->batteryState->batteryLevel = batteryLevel;
        lv_msg_send(MSG_SHOW_BATTERY_LVL, static_cast<const void *>(&batteryLevel));
        voltageSamplesCount = 0;
    } else {
        this->getVoltageSample();
    }
}

float BatteryApp::calculateBatteryVoltage() {
    float batteryVoltage = 0.0f;
    int remainVoltagesCount = voltageSamplesCount;
    for (int i = 0; i < voltageSamplesCount; i++) {
        batteryVoltage = batteryVoltage + voltageSamples[i];
    }
    batteryVoltage = batteryVoltage / voltageSamplesCount;
    for (int i = 0; i < voltageSamplesCount; i++) {
        if (abs((batteryVoltage - voltageSamples[i])) > 0.03f) {
            voltageSamples[i] = 0.0f;
            remainVoltagesCount--;
        }
    }
    batteryVoltage = 0.0f;
    for (int i = 0; i < voltageSamplesCount; i++) {
        batteryVoltage = batteryVoltage + voltageSamples[i];
    }
    batteryVoltage = batteryVoltage / remainVoltagesCount;
    return batteryVoltage;
}

int BatteryApp::calculateBatteryPercentage(float batteryVoltage) {
    if (batteryVoltage <= BAT_MIN_VOLT) {
        return 0;
    } else if (batteryVoltage >= BAT_MAX_VOLT) {
        return 100;
    } else {
        return this->sigmoidal(batteryVoltage, BAT_MIN_VOLT, BAT_MAX_VOLT);
    }
}

int BatteryApp::sigmoidal(float voltage, float minVoltage, float maxVoltage) {
    // slow
    // uint8_t result = 110 - (110 / (1 + pow(1.468 * (voltage - minVoltage)/(maxVoltage -
    // minVoltage), 6)));

    // steep
    // uint8_t result = 102 - (102 / (1 + pow(1.621 * (voltage - minVoltage)/(maxVoltage -
    // minVoltage), 8.1)));

    // normal
    int result =
        105 - (105 / (1 + pow(1.724 * (voltage - minVoltage) / (maxVoltage - minVoltage),
                              5.5)));
    return result >= 100 ? 100 : result;
}

void BatteryApp::getVoltageSample() {
    float battery_voltage =
        (analogReadMilliVolts(BATTERY_LEVEL_PIN) * DIVIDER_RATIO) / 1000.0f;
    voltageSamples[voltageSamplesCount] = battery_voltage;
    voltageSamplesCount++;
}

void BatteryApp::checkChargeStatusAndSendMsg() {
    if (analogReadMilliVolts(BATTERY_CHARGE_PIN) > 2000) {
        if (!stateApp->batteryState->batteryCharging) {
            stateApp->batteryState->batteryCharging = true;
            lv_msg_send(MSG_USB_CONNECTED, NULL);
        }

    } else {
        if (stateApp->batteryState->batteryCharging) {
            stateApp->batteryState->batteryCharging = false;
            lv_msg_send(MSG_USB_DISCONNECTED, NULL);
        }
    }
}

BatteryApp::~BatteryApp() {}
