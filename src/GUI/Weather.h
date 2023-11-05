#pragma once
#include "lvgl.h"

class Weather{
public:
    lv_obj_t *weatherScreen;
    lv_obj_t *weatherPanel;
    lv_obj_t *weatherImage;
    lv_obj_t *weatherCityLabel;
    lv_obj_t *weatherTemperatureLabel;
    lv_obj_t *briefingLabel;
    lv_obj_t *windLabel;
    lv_obj_t *weatherHumidityLabel;
    lv_obj_t *weatherPressureLabel;
    lv_obj_t *weatherMaxTempLabel;
    lv_obj_t *weatherMinTempLabel;
    lv_obj_t *weatherProbabilityLabel;
    lv_obj_t *feelsLikeLabel;
    lv_obj_t *weatherTimesLabel;
    lv_obj_t *weatherFirstTempLabel;
    lv_obj_t *weatherSecondTempLabel;
    lv_obj_t *weatherThirdTempLabel;
    lv_obj_t *weatherFourthTempLabel;
    Weather();
    ~Weather();

};