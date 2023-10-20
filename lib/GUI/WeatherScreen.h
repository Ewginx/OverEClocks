#pragma once
#include "lvgl.h"

class WeatherScreen{
public:
    lv_obj_t *ui_WeatherScreen;
    lv_obj_t *ui_WeatherPanel;
    lv_obj_t *ui_WeatherImage;
    lv_obj_t *ui_WeatherCity;
    lv_obj_t *ui_WeatherTemperatureLabel;
    lv_obj_t *ui_WeatherBriefingLabel;
    lv_obj_t *ui_WeatherWindLabel;
    lv_obj_t *ui_WeatherHumidityLabel;
    lv_obj_t *ui_WeatherMiscLabel;
    lv_obj_t *ui_WeatherFeelsLikeLabel;
    lv_obj_t *ui_WeatherTimesLabel;
    lv_obj_t *ui_WeatherFirstTempLabel;
    lv_obj_t *ui_WeatherSecondTempLabel;
    lv_obj_t *ui_WeatherThirdTempLabel;
    lv_obj_t *ui_WeatherFourthTempLabel;
    WeatherScreen();
    ~WeatherScreen();

};