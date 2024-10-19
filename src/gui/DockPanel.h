#pragma once
#include "Config/Config.h"
#include "lvgl.h"

class DockPanel {

  public:
    lv_obj_t *batteryLabel;
    lv_obj_t *chargingLabel;
    lv_obj_t *settingsButton;
    lv_obj_t *settingsButtonLabel;
    lv_obj_t *temperatureLabel;
    lv_obj_t *humidityLabel;
    lv_obj_t *wiFiLabel;

    bool batteryCharging = false;

    void setBatteryLevelLabel(int batteryLevel);
    void setBatteryChargingIcon(bool charge);
    void setTemperatureLabel(float temperature);
    void setHumidityLabel(int humidity);
    void setDefaultValues();
    void show();
    void hide();
    void changeDockPanelParent(lv_obj_t *newParent);
    void changeWifiConnectionIcon(bool wifiEnabled);
    DockPanel(lv_obj_t *parentPanel);
    ~DockPanel();

    private:
        void createDockPanelWidgets(lv_obj_t *panel);
};
