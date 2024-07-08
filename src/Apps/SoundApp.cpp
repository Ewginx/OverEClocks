#include "SoundApp.h"

static SoundApp *instance = NULL;

extern "C" void alarmRingingCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->playAlarmSound();
}
extern "C" void stopSoundCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->stopSound();
}
extern "C" void playEasterEggCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->playEasterEggSound();
}
extern "C" void playPlugInCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->playPlugInSound();
}
extern "C" void handlePlayerUsbCallbackWrapper(void *subscriber, lv_msg_t *msg) {
    instance->handlePlayerUsb();
}
extern "C" void loopAlarmSoundCallbackWrapper(lv_timer_t *timer) {
    instance->loopAlarmSound();
}

SoundApp::SoundApp(StateApp *stateApp) : player(Serial2) {
    instance = this;

    this->stateApp = stateApp;

    pinMode(PLAYER_USB_CONTROL_PIN, OUTPUT);
    digitalWrite(PLAYER_USB_CONTROL_PIN, LOW);

    soundLoopTimer = lv_timer_create(loopAlarmSoundCallbackWrapper, 900, NULL);
    lv_timer_pause(this->soundLoopTimer);

    lv_msg_subscribe(MSG_ALARM_PLAY, alarmRingingCallbackWrapper, NULL);
    lv_msg_subscribe(MSG_SOUND_STOP, stopSoundCallbackWrapper, NULL);
    lv_msg_subscribe(MSG_PLAY_EE, playEasterEggCallbackWrapper, NULL);
    lv_msg_subscribe(MSG_USB_CONNECTED, playPlugInCallbackWrapper, NULL);
    lv_msg_subscribe(MSG_USB_DISCONNECTED, stopSoundCallbackWrapper, NULL);
    lv_msg_subscribe(MSG_HANDLE_PLAYER_USB, handlePlayerUsbCallbackWrapper, NULL);
}

void SoundApp::setup() {
    player.begin();
    player.reset(false);
    player.setPlaybackSource(DfMp3_PlaySource_Sd);
    this->setVolume();
}

void SoundApp::setVolume() { player.setVolume(this->stateApp->sound_state->volume_level); }

void SoundApp::playAlarmSound() {
    if (!stateApp->sound_state->sound_on)
        return;
    this->resetPlayer();
    lv_timer_resume(this->soundLoopTimer);
    this->setVolume();
    player.playFolderTrack(1, this->stateApp->sound_state->alarm_track);
}

void SoundApp::resetPlayer() {
    if (needToResetMp3Module) {
        player.reset(false);
        needToResetMp3Module = false;
    }
}

void SoundApp::stopSound() {
    player.stop();
    lv_timer_pause(this->soundLoopTimer);
}

void SoundApp::playEasterEggSound() {
    if (this->canPlaySounds(stateApp->sound_state->ee_sound_on)) {
        this->playSoundFromFolderOnce(easterEggTrackFolder,
                                      stateApp->sound_state->ee_track);
    }
}

bool SoundApp::canPlaySounds(bool sound_on) {
    if (stateApp->sound_state->sound_on & !stateApp->alarm_state->alarm_ringing &
        sound_on)
        return true;
    return false;
}

void SoundApp::playPlugInSound() {
    if (this->canPlaySounds(stateApp->sound_state->plug_sound_on))
        this->playSoundFromFolderOnce(easterEggTrackFolder,
                                      stateApp->sound_state->plug_track);
}

void SoundApp::playSoundFromFolderOnce(uint8_t folder, uint8_t track) {
    if (this->isPlayerOffline()) {
        return;
    }
    this->resetPlayer();
    player.setVolume(this->stateApp->sound_state->volume_level);
    player.playFolderTrack(folder, track);
}

void SoundApp::loopAlarmSound() {
    if (this->isPlayerOffline()) {
        this->stopSound();
        return;
    };
    if (this->isPlaying()) {
        return;
    }
    this->setVolume();
    player.playFolderTrack(this->alarmTrackFolder, this->stateApp->sound_state->alarm_track);
}

bool SoundApp::isPlayerOffline() {
    if (player.getTotalTrackCount() == 0) {
        needToResetMp3Module = true;
        return true;
    }
    return false;
}

bool SoundApp::isPlaying() {
    DfMp3_Status current_status = player.getStatus();
    if (current_status.state == DfMp3_StatusState_Playing) {
        return true;
    }
    return false;
}

bool SoundApp::isIdling() {
    DfMp3_Status current_status = player.getStatus();
    if (current_status.state == DfMp3_StatusState_Idle) {
        return true;
    }
    return false;
}

int SoundApp::getTrackCountInAlarmFolder() {
    return player.getFolderTrackCount(alarmTrackFolder);
}

int SoundApp::getTrackCountInEasterEggFolder() {
    return player.getFolderTrackCount(easterEggTrackFolder);
}

void SoundApp::handlePlayerUsb() {
    digitalWrite(PLAYER_USB_CONTROL_PIN, this->stateApp->sound_state->enable_player_usb);
}

SoundApp::~SoundApp() {}