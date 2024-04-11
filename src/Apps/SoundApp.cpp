#include "SoundApp.h"

static SoundApp *instance = NULL;
extern "C" void alarm_ringing_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->play_alarm_sound();
}
extern "C" void stop_sound_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->stop_sound();
}
extern "C" void play_ee_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->play_ee_sound();
}
extern "C" void play_plug_in_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->play_plug_in_sound();
}
extern "C" void handle_usb_cb_wrapper(void *subscriber, lv_msg_t *msg) {
    instance->handle_player_usb();
}
extern "C" void loop_alarm_sound_cb_wrapper(lv_timer_t *timer) {
    instance->loop_alarm_sound();
}

void SoundApp::play_alarm_sound() {
    if (!_state_app->sound_state->sound_on)
        return;
    this->reset_player();
    lv_timer_resume(this->sound_loop_timer);
    this->set_volume();
    player.playFolderTrack(1, _state_app->sound_state->alarm_track);
}

void SoundApp::loop_alarm_sound() {
    if (this->is_player_offline()) {
        this->stop_sound();
        return;
    };
    if (this->is_playing()) {
        return;
    }
    this->set_volume();
    player.playFolderTrack(alarm_track_folder, _state_app->sound_state->alarm_track);
}

void SoundApp::set_volume() { player.setVolume(_state_app->sound_state->volume_level); }

void SoundApp::stop_sound() {
    player.stop();
    lv_timer_pause(this->sound_loop_timer);
}

void SoundApp::reset_player() {
    if (this->need_to_reset) {
        player.reset(false);
        this->need_to_reset = false;
    }
}

void SoundApp::play_ee_sound() {
    if (this->can_play_sounds(_state_app->sound_state->ee_sound_on)) {
        this->play_sound_from_folder_once(easter_egg_track_folder,
                                          _state_app->sound_state->ee_track);
    }
}

void SoundApp::play_plug_in_sound() {
    if (this->can_play_sounds(_state_app->sound_state->plug_sound_on))
        this->play_sound_from_folder_once(easter_egg_track_folder,
                                          _state_app->sound_state->plug_track);
}

void SoundApp::play_sound_from_folder_once(int folder, int track) {
    if (this->is_player_offline()) {
        return;
    }
    this->reset_player();
    player.setVolume(_state_app->sound_state->volume_level);
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

bool SoundApp::is_player_offline() {
    if (player.getTotalTrackCount() == 0) {
        this->need_to_reset = true;
        return true;
    }
    return false;
}

bool SoundApp::can_play_sounds(bool sound_on) {
    if (_state_app->sound_state->sound_on & !_state_app->alarm_state->alarm_ringing &
        sound_on)
        return true;
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
    player.setPlaybackSource(DfMp3_PlaySource_Sd);
    this->set_volume();
}

void SoundApp::handle_player_usb() {
    digitalWrite(PLAYER_USB_CONTROL_PIN,
                 this->_state_app->sound_state->enable_player_usb);
}

SoundApp::SoundApp(StateApp *state_app) : player(Serial2) {
    instance = this;
    this->_state_app = state_app;
    this->sound_loop_timer = lv_timer_create(loop_alarm_sound_cb_wrapper, 900, NULL);
    pinMode(PLAYER_USB_CONTROL_PIN, OUTPUT);
    digitalWrite(PLAYER_USB_CONTROL_PIN, LOW);
    lv_timer_pause(this->sound_loop_timer);
    lv_msg_subscribe(MSG_ALARM_PLAY, alarm_ringing_cb_wrapper, NULL);
    lv_msg_subscribe(MSG_SOUND_STOP, stop_sound_cb_wrapper, NULL);
    lv_msg_subscribe(MSG_PLAY_EE, play_ee_cb_wrapper, NULL);
    lv_msg_subscribe(MSG_USB_CONNECTED, play_plug_in_cb_wrapper, NULL);
    lv_msg_subscribe(MSG_USB_DISCONNECTED, stop_sound_cb_wrapper, NULL);
    lv_msg_subscribe(MSG_HANDLE_PLAYER_USB, handle_usb_cb_wrapper, NULL);
}

SoundApp::~SoundApp() {}