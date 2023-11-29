#include "GUI/Weather.h"

Weather::Weather() {
    weatherScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(weatherScreen, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    weatherPanel = lv_obj_create(weatherScreen);
    lv_obj_set_size(weatherPanel, lv_pct(100), lv_pct(100));
    lv_obj_set_pos(weatherPanel, 0, 0);
    lv_obj_set_align(weatherPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(weatherPanel, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(weatherPanel, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(weatherPanel, 0, 0);
    lv_obj_add_flag(weatherPanel, LV_OBJ_FLAG_EVENT_BUBBLE);

    weatherImage = lv_img_create(weatherPanel);
    lv_img_set_src(weatherImage, &img_day_rain_png);
    lv_obj_set_pos(weatherImage, 10, 30);
    lv_obj_set_align(weatherImage, LV_ALIGN_TOP_LEFT);

    windLabel = lv_label_create(weatherPanel);
    lv_obj_set_pos(windLabel, 165, -40);
    lv_obj_set_align(windLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(windLabel, &weather_font_18, 0);

    weatherCityLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherCityLabel, 200, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(weatherCityLabel, 20, 35);
    lv_obj_set_align(weatherCityLabel, LV_ALIGN_TOP_MID);

    lv_label_set_long_mode(weatherCityLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_text_font(weatherCityLabel, &font_18, 0);
    lv_obj_set_style_text_align(weatherCityLabel, LV_TEXT_ALIGN_CENTER, 0);

    weatherTemperatureLabel = lv_label_create(weatherPanel);
    lv_obj_set_pos(weatherTemperatureLabel, 20, -45);
    lv_obj_set_align(weatherTemperatureLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(weatherTemperatureLabel, &temperature_font_48, 0);

    feelsLikeLabel = lv_label_create(weatherPanel);
    lv_obj_set_pos(feelsLikeLabel, 40, 5);
    lv_obj_set_align(feelsLikeLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(feelsLikeLabel, &font_18, 0);
    lv_obj_set_style_text_align(feelsLikeLabel, LV_TEXT_ALIGN_CENTER, 0);

    briefingLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(briefingLabel, 320, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(briefingLabel, 0, 40);
    lv_obj_set_align(briefingLabel, LV_ALIGN_CENTER);
    lv_label_set_long_mode(briefingLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_text_font(briefingLabel, &font_18, 0);
    lv_obj_set_style_text_align(briefingLabel, LV_TEXT_ALIGN_CENTER, 0);

    weatherMaxTempLabel = lv_label_create(weatherPanel);
    lv_obj_set_style_text_font(weatherMaxTempLabel, &weather_font_18, 0);
    lv_obj_set_pos(weatherMaxTempLabel, -150, 75);
    lv_obj_set_align(weatherMaxTempLabel, LV_ALIGN_CENTER);

    weatherMinTempLabel = lv_label_create(weatherPanel);
    lv_obj_set_style_text_font(weatherMinTempLabel, &weather_font_18, 0);
    lv_obj_align_to(weatherMinTempLabel, weatherMaxTempLabel, LV_ALIGN_OUT_RIGHT_MID, 25,
                    -1);

    weatherProbabilityLabel = lv_label_create(weatherPanel);
    lv_obj_set_style_text_font(weatherProbabilityLabel, &weather_font_18, 0);
    lv_obj_align_to(weatherProbabilityLabel, weatherMinTempLabel, LV_ALIGN_OUT_RIGHT_MID,
                    35, -1);

    weatherHumidityLabel = lv_label_create(weatherPanel);
    lv_obj_set_style_text_font(weatherHumidityLabel, &weather_font_18, 0);
    lv_obj_align_to(weatherHumidityLabel, weatherProbabilityLabel, LV_ALIGN_OUT_RIGHT_MID,
                    30, -1);

    weatherPressureLabel = lv_label_create(weatherPanel);
    lv_obj_set_style_text_font(weatherPressureLabel, &weather_font_18, 0);
    lv_obj_align_to(weatherPressureLabel, weatherHumidityLabel, LV_ALIGN_OUT_RIGHT_MID,
                    25, -1);

    weatherTimesLabel = lv_label_create(weatherPanel);
    lv_obj_set_pos(weatherTimesLabel, 0, 103);
    lv_obj_set_align(weatherTimesLabel, LV_ALIGN_CENTER);
    lv_label_set_text_fmt(weatherTimesLabel, "%s%s%s%s", weather_translation[night],
                          weather_translation[morning], weather_translation[afternoon],
                          weather_translation[evening]);
    lv_obj_set_style_text_align(weatherTimesLabel, LV_TEXT_ALIGN_CENTER, 0);

    weatherFirstTempLabel = lv_label_create(weatherPanel);
    lv_obj_set_pos(weatherFirstTempLabel, 55, -5);
    lv_obj_set_align(weatherFirstTempLabel, LV_ALIGN_BOTTOM_LEFT);

    weatherSecondTempLabel = lv_label_create(weatherPanel);
    lv_obj_align_to(weatherSecondTempLabel, weatherFirstTempLabel, LV_ALIGN_OUT_RIGHT_TOP,
                    65, 0);

    weatherThirdTempLabel = lv_label_create(weatherPanel);
    lv_obj_align_to(weatherThirdTempLabel, weatherSecondTempLabel, LV_ALIGN_OUT_RIGHT_TOP,
                    65, 0);

    weatherFourthTempLabel = lv_label_create(weatherPanel);
    lv_obj_align_to(weatherFourthTempLabel, weatherThirdTempLabel, LV_ALIGN_OUT_RIGHT_TOP,
                    65, 0);
    this->set_no_data_values();
}
void Weather::set_no_data_values() {
    lv_label_set_text_fmt(windLabel, WIND_SYMBOL " %s %s " NO_DATA_SYMBOL, NO_DATA_SYMBOL,
                          weather_translation[wind_speed_uom]);
    lv_label_set_text_fmt(weatherCityLabel, "%s", weather_translation[no_data]);
    lv_label_set_text_fmt(weatherTemperatureLabel, "%s°C", "-");
    lv_label_set_text_fmt(feelsLikeLabel, "%s %s°C", weather_translation[feels_like],
                          weather_translation[no_data]);
    lv_label_set_text(briefingLabel, weather_translation[no_data]);
    lv_label_set_text_fmt(weatherMaxTempLabel, TEMP_UP_SYMBOL " %s°C", NO_DATA_SYMBOL);
    lv_label_set_text_fmt(weatherMinTempLabel, TEMP_DOWN_SYMBOL " %s°C", NO_DATA_SYMBOL);
    lv_label_set_text_fmt(weatherProbabilityLabel, SNOWFLAKE_SYMBOL " %s%%",
                          NO_DATA_SYMBOL);
    lv_label_set_text_fmt(weatherHumidityLabel, HUMIDITY_SYMBOL " %s%%", NO_DATA_SYMBOL);
    lv_label_set_text_fmt(weatherPressureLabel, PRESSURE_SYMBOL " %s %s", NO_DATA_SYMBOL,
                          weather_translation[pressure_uom]);
    lv_label_set_text(weatherFirstTempLabel, "  -°C");
    lv_label_set_text(weatherSecondTempLabel, "  -°C");
    lv_label_set_text(weatherThirdTempLabel, "  -°C");
    lv_label_set_text(weatherFourthTempLabel, "  -°C");
}
Weather::~Weather(){};
