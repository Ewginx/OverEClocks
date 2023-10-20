#include "WeatherScreen.h"

WeatherScreen::WeatherScreen(){
    LV_IMG_DECLARE(ui_img_day_rain_png);
    LV_IMG_DECLARE(ui_img_day_clear_png);
    ui_WeatherScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_WeatherScreen, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_WeatherPanel = lv_obj_create(ui_WeatherScreen);
    lv_obj_set_size(ui_WeatherPanel, lv_pct(100), lv_pct(100));
    lv_obj_set_pos(ui_WeatherPanel, 0, 0);
    lv_obj_set_align(ui_WeatherPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_WeatherPanel, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(ui_WeatherPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_WeatherPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_WeatherImage = lv_img_create(ui_WeatherPanel);
    lv_img_set_src(ui_WeatherImage, &ui_img_day_clear_png);
    lv_obj_set_size(ui_WeatherImage, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 128
    lv_obj_set_pos(ui_WeatherImage, -140, -71);
    lv_obj_set_align(ui_WeatherImage, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_WeatherImage, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_WeatherImage, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_WeatherCity = lv_label_create(ui_WeatherPanel);
    lv_obj_set_size(ui_WeatherCity, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(ui_WeatherCity, 0, -126);
    lv_obj_set_align(ui_WeatherCity, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WeatherCity, "London, GB");
    lv_obj_set_style_text_font(ui_WeatherCity, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_WeatherTemperatureLabel = lv_label_create(ui_WeatherPanel);
    lv_obj_set_size(ui_WeatherTemperatureLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(ui_WeatherTemperatureLabel, -5, -18);
    lv_obj_set_align(ui_WeatherTemperatureLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WeatherTemperatureLabel, "13°C");
    lv_obj_set_style_text_font(ui_WeatherTemperatureLabel, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_WeatherBriefingLabel = lv_label_create(ui_WeatherPanel);
    lv_obj_set_size(ui_WeatherBriefingLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(ui_WeatherBriefingLabel, 1, 39);
    lv_obj_set_align(ui_WeatherBriefingLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WeatherBriefingLabel, "Scattered clouds. Light breeze");

    ui_WeatherWindLabel = lv_label_create(ui_WeatherPanel);
    lv_obj_set_size(ui_WeatherWindLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 121
    lv_obj_set_pos(ui_WeatherWindLabel, 72, -96);
    lv_obj_set_align(ui_WeatherWindLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WeatherWindLabel, "3.1m/s WNW Gust: 2.3m/s");
    lv_obj_set_style_text_font(ui_WeatherWindLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_WeatherHumidityLabel = lv_label_create(ui_WeatherPanel);
    lv_obj_set_size(ui_WeatherHumidityLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 121
    lv_obj_set_pos(ui_WeatherHumidityLabel, 72, -80);
    lv_obj_set_align(ui_WeatherHumidityLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WeatherHumidityLabel, "Humidity: 75%");
    lv_obj_set_style_text_font(ui_WeatherHumidityLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_WeatherMiscLabel = lv_label_create(ui_WeatherPanel);
    lv_obj_set_size(ui_WeatherMiscLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 121
    lv_obj_set_pos(ui_WeatherMiscLabel, 120, -60);
    lv_obj_set_align(ui_WeatherMiscLabel, LV_ALIGN_CENTER);
    lv_label_set_long_mode(ui_WeatherMiscLabel, LV_LABEL_LONG_CLIP);
    lv_label_set_text(ui_WeatherMiscLabel, "Visibility: 10.0km UV: 3");
    lv_obj_set_style_text_font(ui_WeatherMiscLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_WeatherFeelsLikeLabel = lv_label_create(ui_WeatherPanel);
    lv_obj_set_size(ui_WeatherFeelsLikeLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 121
    lv_obj_set_pos(ui_WeatherFeelsLikeLabel, 140, -10);
    lv_obj_set_align(ui_WeatherFeelsLikeLabel, LV_ALIGN_CENTER);
    lv_label_set_long_mode(ui_WeatherFeelsLikeLabel, LV_LABEL_LONG_CLIP);
    lv_label_set_text(ui_WeatherFeelsLikeLabel, "Feels like: 13°C");
    lv_obj_set_style_text_font(ui_WeatherFeelsLikeLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_WeatherTimesLabel = lv_label_create(ui_WeatherPanel);
    lv_obj_set_size(ui_WeatherTimesLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(ui_WeatherTimesLabel, -12, 80);
    lv_obj_set_align(ui_WeatherTimesLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WeatherTimesLabel, "13:00       16:00      19:00       21:00");

    ui_WeatherThirdTempLabel = lv_label_create(ui_WeatherPanel);
    lv_obj_set_size(ui_WeatherThirdTempLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(ui_WeatherThirdTempLabel, 125, 118);
    lv_obj_set_align(ui_WeatherThirdTempLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WeatherThirdTempLabel, "10°C");

    ui_WeatherSecondTempLabel = lv_label_create(ui_WeatherPanel);
    lv_obj_set_size(ui_WeatherSecondTempLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(ui_WeatherSecondTempLabel, -50, 118);
    lv_obj_set_align(ui_WeatherSecondTempLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WeatherSecondTempLabel, "10°C	");

    ui_WeatherFourthTempLabel = lv_label_create(ui_WeatherPanel);
    lv_obj_set_size(ui_WeatherFourthTempLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(ui_WeatherFourthTempLabel, 38, 118);
    lv_obj_set_align(ui_WeatherFourthTempLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WeatherFourthTempLabel, "10°C	");

    ui_WeatherFirstTempLabel = lv_label_create(ui_WeatherPanel);
    lv_obj_set_size(ui_WeatherFirstTempLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(ui_WeatherFirstTempLabel, -141, 119);
    lv_obj_set_align(ui_WeatherFirstTempLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WeatherFirstTempLabel, "7°C	");
};