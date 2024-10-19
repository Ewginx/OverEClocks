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

void SoundApp::setVolume() { player.setVolume(this->stateApp->soundState->volumeLevel); }

void SoundApp::playAlarmSound() {
    if (!stateApp->soundState->soundOn)
        return;
    this->resetPlayer();
    lv_timer_resume(this->soundLoopTimer);
    this->setVolume();
    player.playFolderTrack(1, this->stateApp->soundState->alarmTrackNumber);
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
    if (this->canPlaySounds(stateApp->soundState->eeSoundOn)) {
        this->playSoundFromFolderOnce(easterEggTrackFolder,
                                      stateApp->soundState->eeTrackNumber);
    }
}

bool SoundApp::canPlaySounds(bool soundOn) {
    if (stateApp->soundState->soundOn & !stateApp->alarmState->alarmIsRinging &
        soundOn)
        return true;
    return false;
}

void SoundApp::playPlugInSound() {
    if (this->canPlaySounds(stateApp->soundState->plugInSoundOn))
        this->playSoundFromFolderOnce(easterEggTrackFolder,
                                      stateApp->soundState->plugInTrackNumber);
}

void SoundApp::playSoundFromFolderOnce(uint8_t folder, uint8_t track) {
    if (this->isPlayerOffline()) {
        return;
    }
    this->resetPlayer();
    player.setVolume(this->stateApp->soundState->volumeLevel);
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
    player.playFolderTrack(this->alarmTrackFolder, this->stateApp->soundState->alarmTrackNumber);
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
    digitalWrite(PLAYER_USB_CONTROL_PIN, this->stateApp->soundState->enablePlayerUsb);
}

SoundApp::~SoundApp() {}