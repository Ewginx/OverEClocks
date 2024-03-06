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
        player.playFolderTrack(1, _state_app->sound_state->alarm_track_number);
        // Serial.println(player.getFolderTrackCount(1));
        // Serial.println(player.getTotalFolderCount()); // doesn't work, at least with SD
        // card Serial.println(player.getTotalTrackCount());
    }
    player.setVolume(_state_app->sound_state->volume_level);
}

void SoundApp::loop_alarm_sound() {
    player.setVolume(_state_app->sound_state->volume_level);
    if (this->is_idling()) {
        player.playFolderTrack(alarm_track_folder,
                               _state_app->sound_state->alarm_track_number);
    }
}

void SoundApp::stop_sound() {
    player.stop();
    if (this->sound_loop_timer != NULL & !_state_app->alarm_state->alarm_ringing) {
        lv_timer_del(this->sound_loop_timer);
        this->sound_loop_timer = NULL;
    }
}

void SoundApp::play_ee() {
    if (!_state_app->sound_state->sound_on || _state_app->alarm_state->alarm_ringing)
        return;
    player.setVolume(_state_app->sound_state->volume_level);
    player.playFolderTrack(easter_egg_track_folder,
                           _state_app->sound_state->ee_track_number);
}

void SoundApp::play_sound_once(int folder, int track) {
    player.playFolderTrack(folder, track);
}

bool SoundApp::is_playing() {
    DfMp3_Status current_status = player.getStatus();
    if (current_status.state == DfMp3_StatusState_Playing) {
        return true;
    }
    return false;
}

bool SoundApp::is_idling() {
    DfMp3_Status current_status = player.getStatus();
    if (current_status.state == DfMp3_StatusState_Idle) {
        return true;
    }
    return false;
}

int SoundApp::get_track_count_in_alarm_folder() {
    return player.getFolderTrackCount(alarm_track_folder);
}

int SoundApp::get_track_count_in_ee_folder() {
    return player.getFolderTrackCount(easter_egg_track_folder);
}

void SoundApp::setup_player() {
    player.begin();
    player.reset(false);
    player.setVolume(_state_app->sound_state->volume_level);
}

SoundApp::SoundApp(StateApp *state_app) : player(Serial2) {
    instance = this;
    this->_state_app = state_app;
    lv_msg_subscribe(MSG_ALARM_PLAY, alarm_ringing_cb_wrapper, NULL);
    lv_msg_subscribe(MSG_SOUND_STOP, stop_sound_cb_wrapper, NULL);
    lv_msg_subscribe(MSG_PLAY_EE, play_ee_cb_wrapper, NULL);
}

SoundApp::~SoundApp() {}