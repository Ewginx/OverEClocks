#include "ButtonApp.h"

static ButtonApp *instance = NULL;

extern "C" void measureTimeButtonIsPressedCallbackWrapper(lv_timer_t *timer) {
    instance->measureTimeButtonIsPressed();
}

ButtonApp::ButtonApp(StateApp *stateApp) {
    instance = this;

    this->stateApp = stateApp;

    pinMode(BUTTON_PIN, INPUT);

    this->buttonTimer =
        lv_timer_create(measureTimeButtonIsPressedCallbackWrapper, measureDelay, NULL);
}

void ButtonApp::measureTimeButtonIsPressed() {
    if (digitalRead(BUTTON_PIN) == HIGH) {
        elapsedTime += measureDelay;
        if (elapsedTime >= measureDelay * 10) {
            Serial.println("Button was long pressed");
            this->longPress();
        }
    } else {
        if (elapsedTime >= measureDelay * 2 & !wasPressed) {
            Serial.println("Button was short pressed");
            this->shortPress();
        } else {
            wasPressed = false;
            elapsedTime = 0;
        }
    }
}

void ButtonApp::longPress() {
    wasPressed = true;
    elapsedTime = 0;
    if (stateApp->alarmState->alarmIsRinging) {
        lv_msg_send(MSG_ALARM_STOP, NULL);
    } else {
        lv_msg_send(MSG_PLAY_EE, NULL);
    }
}

void ButtonApp::shortPress() {
    elapsedTime = 0;
    if (stateApp->alarmState->alarmIsRinging) {
        lv_msg_send(MSG_ALARM_SNOOZE, NULL);
    } else {
        lv_msg_send(MSG_SOUND_STOP, NULL);
    }
}

ButtonApp::~ButtonApp() {}