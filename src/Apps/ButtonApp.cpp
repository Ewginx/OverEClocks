#include "ButtonApp.h"

static ButtonApp *instance = NULL;

extern "C" void measure_time_button_press_cb_wrapper(lv_timer_t *timer) {
    instance->measure_time_button_press();
}

void ButtonApp::measure_time_button_press() {
    if (digitalRead(BUTTON_PIN) == HIGH) {
        Serial.println("BUTTON IS PRESSED");
        this->elapsed_time += 200;
        if (this->elapsed_time >= 4000) {
            Serial.println("BUTTON WAS LONG PRESSED");
            this->was_pressed = true;
            this->elapsed_time = 0;
            if (this->_state_app->alarm_state->alarm_ringing) {
                lv_msg_send(MSG_ALARM_STOP, NULL);
            } else {
                lv_msg_send(MSG_PLAY_EE, NULL);
            }
            return;
        }

    } else {
        if (this->elapsed_time >= 400 & !this->was_pressed) {
            Serial.println("BUTTON WAS SHORT PRESSED");
            this->elapsed_time = 0;
            lv_msg_send(MSG_SHORT_PRESS, NULL);
            return;
        } else {
            this->was_pressed = false;
            this->elapsed_time = 0;
        }
    }
}

ButtonApp::ButtonApp(StateApp *state_app) {
    instance = this;
    this->_state_app = state_app;
    pinMode(BUTTON_PIN, INPUT);
    this->button_timer = lv_timer_create(measure_time_button_press_cb_wrapper, 200, NULL);
}

ButtonApp::~ButtonApp() {}
