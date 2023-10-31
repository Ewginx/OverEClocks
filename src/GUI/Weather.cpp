#include "GUI/Weather.h"

Weather::Weather(){
    LV_IMG_DECLARE(img_day_rain_png);
    LV_IMG_DECLARE(img_day_clear_png);
    weatherScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(weatherScreen, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    weatherPanel = lv_obj_create(weatherScreen);
    lv_obj_set_size(weatherPanel, lv_pct(100), lv_pct(100));
    lv_obj_set_pos(weatherPanel, 0, 0);
    lv_obj_set_align(weatherPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(weatherPanel, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(weatherPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(weatherPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherImage = lv_img_create(weatherPanel);
    lv_img_set_src(weatherImage, &img_day_clear_png);
    lv_obj_set_size(weatherImage, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 128
    lv_obj_set_pos(weatherImage, -140, -71);
    lv_obj_set_align(weatherImage, LV_ALIGN_CENTER);
    lv_obj_add_flag(weatherImage, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(weatherImage, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    weatherCityLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherCityLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(weatherCityLabel, 0, -126);
    lv_obj_set_align(weatherCityLabel, LV_ALIGN_CENTER);
    lv_label_set_text(weatherCityLabel, "London, GB");
    lv_obj_set_style_text_font(weatherCityLabel, &montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherTemperatureLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherTemperatureLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(weatherTemperatureLabel, -5, -18);
    lv_obj_set_align(weatherTemperatureLabel, LV_ALIGN_CENTER);
    lv_label_set_text(weatherTemperatureLabel, "13°C");
    lv_obj_set_style_text_font(weatherTemperatureLabel, &montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

    briefingLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(briefingLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(briefingLabel, 1, 39);
    lv_obj_set_align(briefingLabel, LV_ALIGN_CENTER);
    lv_label_set_text(briefingLabel, "Scattered clouds. Light breeze");

    windLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(windLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 121
    lv_obj_set_pos(windLabel, 72, -96);
    lv_obj_set_align(windLabel, LV_ALIGN_CENTER);
    lv_label_set_text(windLabel, "3.1m/s WNW Gust: 2.3m/s");
    lv_obj_set_style_text_font(windLabel, &montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherHumidityLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherHumidityLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 121
    lv_obj_set_pos(weatherHumidityLabel, 72, -80);
    lv_obj_set_align(weatherHumidityLabel, LV_ALIGN_CENTER);
    lv_label_set_text(weatherHumidityLabel, "Humidity: 75%");
    lv_obj_set_style_text_font(weatherHumidityLabel, &montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherMiscLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherMiscLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 121
    lv_obj_set_pos(weatherMiscLabel, 120, -60);
    lv_obj_set_align(weatherMiscLabel, LV_ALIGN_CENTER);
    lv_label_set_long_mode(weatherMiscLabel, LV_LABEL_LONG_CLIP);
    lv_label_set_text(weatherMiscLabel, "Visibility: 10.0km UV: 3");
    lv_obj_set_style_text_font(weatherMiscLabel, &montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

    feelsLikeLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(feelsLikeLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 121
    lv_obj_set_pos(feelsLikeLabel, 140, -10);
    lv_obj_set_align(feelsLikeLabel, LV_ALIGN_CENTER);
    lv_label_set_long_mode(feelsLikeLabel, LV_LABEL_LONG_CLIP);
    lv_label_set_text(feelsLikeLabel, "Feels like: 13°C");
    lv_obj_set_style_text_font(feelsLikeLabel, &montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherTimesLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherTimesLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(weatherTimesLabel, -12, 80);
    lv_obj_set_align(weatherTimesLabel, LV_ALIGN_CENTER);
    lv_label_set_text(weatherTimesLabel, "13:00       16:00      19:00       21:00");

    weatherThirdTempLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherThirdTempLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(weatherThirdTempLabel, 125, 118);
    lv_obj_set_align(weatherThirdTempLabel, LV_ALIGN_CENTER);
    lv_label_set_text(weatherThirdTempLabel, "10°C");

    weatherSecondTempLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherSecondTempLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(weatherSecondTempLabel, -50, 118);
    lv_obj_set_align(weatherSecondTempLabel, LV_ALIGN_CENTER);
    lv_label_set_text(weatherSecondTempLabel, "10°C	");

    weatherFourthTempLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherFourthTempLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(weatherFourthTempLabel, 38, 118);
    lv_obj_set_align(weatherFourthTempLabel, LV_ALIGN_CENTER);
    lv_label_set_text(weatherFourthTempLabel, "10°C	");

    weatherFirstTempLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherFirstTempLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(weatherFirstTempLabel, -141, 119);
    lv_obj_set_align(weatherFirstTempLabel, LV_ALIGN_CENTER);
    lv_label_set_text(weatherFirstTempLabel, "7°C	");
}
Weather::~Weather(){};