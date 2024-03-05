#include "SoundApp.h"

static SoundApp *instance = NULL;
extern "C" void alarm_ringing_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->play_alarm_sound();
}
extern "C" void stop_sound_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->stop_sound();
}
extern "C" void play_ee_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->play_ee();
}
extern "C" void loop_alarm_sound_cb_wrapper(lv_timer_t *timer) {
    instance->loop_alarm_sound();
}

void SoundApp::play_alarm_sound() {
    if (!_state_app->sound_state->sound_on)
        return;
    if (this->sound_loop_timer == NULL) {
        sound_loop_timer = lv_timer_create(loop_alarm_sound_cb_wrapper, 400, NULL);
        player.playFolder(1, _state_app->sound_state->alarm_track_number);
        Serial.println(player.numFolders());
    }
    player.volume(_state_app->sound_state->volume_level);
}

void SoundApp::loop_alarm_sound() {
    if (!player.isPlaying()) {
        player.playFolder(1, _state_app->sound_state->alarm_track_number);
    }
}

void SoundApp::stop_sound() {
    // player.stop();
    // if (this->sound_loop_timer != NULL & !_state_app->alarm_state->alarm_ringing) {
    //     lv_timer_del(this->sound_loop_timer);
    //     this->sound_loop_timer = NULL;
    // }
}

void SoundApp::play_ee() {
    if (!_state_app->sound_state->sound_on || _state_app->alarm_state->alarm_ringing)
        return;
    // player.stop();
    // player.volume(_state_app->sound_state->volume_level);
    // player.playFolder(2, _state_app->sound_state->ee_track_number);
}

// int SoundApp::get_file_counts() { return player.readFileCountsInFolder(1); }

void SoundApp::setup_player() {
    Serial2.begin(9600);
    player.begin(Serial2);
    // player.reset();
    player.volume(
        _state_app->sound_state->volume_level); // Set volume value. From 0 to 30
    // player.printError();
}

SoundApp::SoundApp(StateApp *state_app) {
    instance = this;
    this->_state_app = state_app;
    lv_msg_subscribe(MSG_ALARM_PLAY, alarm_ringing_cb_wrapper, NULL);
    lv_msg_subscribe(MSG_SOUND_STOP, stop_sound_cb_wrapper, NULL);
    lv_msg_subscribe(MSG_PLAY_EE, play_ee_cb_wrapper, NULL);
}

SoundApp::~SoundApp() {}