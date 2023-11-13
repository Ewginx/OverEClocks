#include "GUI/Weather.h"

Weather::Weather() {

    weatherScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(weatherScreen, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    weatherPanel = lv_obj_create(weatherScreen);
    lv_obj_set_size(weatherPanel, lv_pct(100), lv_pct(100));
    lv_obj_set_pos(weatherPanel, 0, 0);
    lv_obj_set_align(weatherPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(weatherPanel, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(weatherPanel, lv_color_hex(0xFFFFFF),
                              LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(weatherPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherImage = lv_img_create(weatherPanel);
    lv_img_set_src(weatherImage, &img_day_rain_png);
    lv_obj_set_pos(weatherImage, 10, 30);
    lv_obj_set_align(weatherImage, LV_ALIGN_TOP_LEFT);

    windLabel = lv_label_create(weatherPanel);
    lv_label_set_text_fmt(windLabel, WIND_SYMBOL " %s %s " NO_DATA_SYMBOL, NO_DATA_SYMBOL,
                          weather_translation[wind_speed_uom]);
    lv_obj_set_pos(windLabel, 165, -40);
    lv_obj_set_align(windLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(windLabel, &weather_icon_font_full_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherCityLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherCityLabel, 200, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(weatherCityLabel, 20, 35);
    lv_obj_set_align(weatherCityLabel, LV_ALIGN_TOP_MID);
    lv_label_set_text_fmt(weatherCityLabel, "%s", NO_DATA_SYMBOL);
    lv_label_set_long_mode(weatherCityLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_text_font(weatherCityLabel, &montserrat_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherTemperatureLabel = lv_label_create(weatherPanel);
    lv_obj_set_pos(weatherTemperatureLabel, 20, -45);
    lv_obj_set_align(weatherTemperatureLabel, LV_ALIGN_CENTER);
    lv_label_set_text_fmt(weatherTemperatureLabel, "%s°C", NO_DATA_SYMBOL);
    lv_obj_set_style_text_font(weatherTemperatureLabel, &temperature_font_48,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    feelsLikeLabel = lv_label_create(weatherPanel);
    lv_obj_set_pos(feelsLikeLabel, 40, 5);
    lv_obj_set_align(feelsLikeLabel, LV_ALIGN_CENTER);
    lv_label_set_text_fmt(feelsLikeLabel, "%s %s°C", weather_translation[feels_like],
                          NO_DATA_SYMBOL);
    lv_obj_set_style_text_font(feelsLikeLabel, &montserrat_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(feelsLikeLabel, LV_TEXT_ALIGN_CENTER,
                                LV_PART_MAIN | LV_STATE_DEFAULT);

    briefingLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(briefingLabel, 320, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(briefingLabel, 0, 35);
    lv_obj_set_align(briefingLabel, LV_ALIGN_CENTER);
    lv_label_set_long_mode(briefingLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(briefingLabel, NO_DATA_SYMBOL);
    lv_obj_set_style_text_font(briefingLabel, &montserrat_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(briefingLabel, LV_TEXT_ALIGN_CENTER,
                                LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherMaxTempLabel = lv_label_create(weatherPanel);
    lv_label_set_text_fmt(weatherMaxTempLabel, TEMP_UP_SYMBOL " %s°C", NO_DATA_SYMBOL);
    lv_obj_set_style_text_font(weatherMaxTempLabel, &weather_icon_font_full_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_pos(weatherMaxTempLabel, -170, 65);
    lv_obj_set_align(weatherMaxTempLabel, LV_ALIGN_CENTER);

    weatherMinTempLabel = lv_label_create(weatherPanel);
    lv_label_set_text_fmt(weatherMinTempLabel, TEMP_DOWN_SYMBOL " %s°C", NO_DATA_SYMBOL);
    lv_obj_set_style_text_font(weatherMinTempLabel, &weather_icon_font_full_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align_to(weatherMinTempLabel, weatherMaxTempLabel, LV_ALIGN_OUT_RIGHT_MID, 15,
                    -1);

    weatherProbabilityLabel = lv_label_create(weatherPanel);
    lv_label_set_text_fmt(weatherProbabilityLabel, SNOWFLAKE_SYMBOL " %s%%",
                          NO_DATA_SYMBOL);
    lv_obj_set_style_text_font(weatherProbabilityLabel, &weather_icon_font_full_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align_to(weatherProbabilityLabel, weatherMinTempLabel, LV_ALIGN_OUT_RIGHT_MID,
                    15, -1);

    weatherHumidityLabel = lv_label_create(weatherPanel);
    lv_label_set_text_fmt(weatherHumidityLabel, HUMIDITY_SYMBOL " %s%%", NO_DATA_SYMBOL);
    lv_obj_set_style_text_font(weatherHumidityLabel, &weather_icon_font_full_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align_to(weatherHumidityLabel, weatherProbabilityLabel, LV_ALIGN_OUT_RIGHT_MID,
                    15, -1);

    weatherPressureLabel = lv_label_create(weatherPanel);
    lv_label_set_text_fmt(weatherPressureLabel, PRESSURE_SYMBOL " %s %s", NO_DATA_SYMBOL,
                          weather_translation[pressure_uom]);
    lv_obj_set_style_text_font(weatherPressureLabel, &weather_icon_font_full_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align_to(weatherPressureLabel, weatherHumidityLabel, LV_ALIGN_OUT_RIGHT_MID,
                    15, -1);

    weatherTimesLabel = lv_label_create(weatherPanel);
    lv_obj_set_pos(weatherTimesLabel, 0, 93);
    lv_obj_set_align(weatherTimesLabel, LV_ALIGN_CENTER);
    lv_label_set_text_fmt(weatherTimesLabel,
                          "%s%s%s%s",
                          weather_translation[night], weather_translation[morning],
                          weather_translation[afternoon], weather_translation[evening]);
    lv_obj_set_style_text_align(weatherTimesLabel, LV_TEXT_ALIGN_CENTER,
                                LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherFirstTempLabel = lv_label_create(weatherPanel);
    lv_obj_set_pos(weatherFirstTempLabel, 55, -10);
    lv_obj_set_align(weatherFirstTempLabel, LV_ALIGN_BOTTOM_LEFT);
    lv_label_set_text_fmt(weatherFirstTempLabel, "%s°C", NO_DATA_SYMBOL);

    weatherSecondTempLabel = lv_label_create(weatherPanel);
    lv_obj_align_to(weatherSecondTempLabel, weatherFirstTempLabel, LV_ALIGN_OUT_RIGHT_TOP,
                    65, 0);
    lv_label_set_text_fmt(weatherSecondTempLabel, "%s°C", NO_DATA_SYMBOL);

    weatherThirdTempLabel = lv_label_create(weatherPanel);
    lv_obj_align_to(weatherThirdTempLabel, weatherSecondTempLabel, LV_ALIGN_OUT_RIGHT_TOP,
                    65, 0);
    lv_label_set_text_fmt(weatherThirdTempLabel, "%s°C", NO_DATA_SYMBOL);

    weatherFourthTempLabel = lv_label_create(weatherPanel);
    lv_obj_align_to(weatherFourthTempLabel, weatherThirdTempLabel, LV_ALIGN_OUT_RIGHT_TOP,
                    65, 0);
    lv_label_set_text_fmt(weatherFourthTempLabel, "%s°C", NO_DATA_SYMBOL);
}
Weather::~Weather(){};
