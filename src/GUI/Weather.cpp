#include "GUI/Weather.h"

Weather::Weather() {
    LV_IMG_DECLARE(img_day_rain_png);
    LV_IMG_DECLARE(img_day_clear_png);
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
    lv_label_set_text_fmt(windLabel, WIND_SYMBOL " %.1f км/ч " SW_WIND_SYMBOL, 15.1);
    lv_obj_set_pos(windLabel, 150, -40);
    lv_obj_set_align(windLabel, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(windLabel, &weather_icon_font_full_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherCityLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherCityLabel, 200, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(weatherCityLabel, 20, 35);
    lv_obj_set_align(weatherCityLabel, LV_ALIGN_TOP_MID);
    lv_label_set_text(weatherCityLabel, "Saint-Petersburg, Russia");
    lv_label_set_long_mode(weatherCityLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_text_font(weatherCityLabel, &montserrat_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherTemperatureLabel = lv_label_create(weatherPanel);
    lv_obj_set_pos(weatherTemperatureLabel, 20, -45);
    lv_obj_set_align(weatherTemperatureLabel, LV_ALIGN_CENTER);
    lv_label_set_text(weatherTemperatureLabel, "-13°C");
    lv_obj_set_style_text_font(weatherTemperatureLabel, &montserrat_48,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    feelsLikeLabel = lv_label_create(weatherPanel);
    lv_obj_set_pos(feelsLikeLabel, 40, 5);
    lv_obj_set_align(feelsLikeLabel, LV_ALIGN_CENTER);
    lv_label_set_text(feelsLikeLabel, "Ощущается как: -24°C");
    lv_obj_set_style_text_font(feelsLikeLabel, &montserrat_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    briefingLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(briefingLabel, 320, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(briefingLabel, 0, 35);
    lv_obj_set_align(briefingLabel, LV_ALIGN_CENTER);
    lv_label_set_long_mode(briefingLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(briefingLabel, "Переменная облачность. Возможен дождь.");
    lv_obj_set_style_text_font(briefingLabel, &montserrat_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherMaxTempLabel = lv_label_create(weatherPanel);
    lv_label_set_text_fmt(weatherMaxTempLabel, TEMP_UP_SYMBOL " %d°C", -8);
    lv_obj_set_style_text_font(weatherMaxTempLabel, &weather_icon_font_full_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_pos(weatherMaxTempLabel, -170, 65);
    lv_obj_set_align(weatherMaxTempLabel, LV_ALIGN_CENTER);

    weatherMinTempLabel = lv_label_create(weatherPanel);
    lv_label_set_text_fmt(weatherMinTempLabel, TEMP_DOWN_SYMBOL " %d°C", -19);
    lv_obj_set_style_text_font(weatherMinTempLabel, &weather_icon_font_full_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align_to(weatherMinTempLabel, weatherMaxTempLabel, LV_ALIGN_OUT_RIGHT_MID, 15,
                    -1);

    weatherProbabilityLabel = lv_label_create(weatherPanel);
    lv_label_set_text_fmt(weatherProbabilityLabel, SNOWFLAKE_SYMBOL " %d%%", 53);
    lv_obj_set_style_text_font(weatherProbabilityLabel, &weather_icon_font_full_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align_to(weatherProbabilityLabel, weatherMinTempLabel, LV_ALIGN_OUT_RIGHT_MID,
                    15, -1);

    weatherHumidityLabel = lv_label_create(weatherPanel);
    lv_label_set_text_fmt(weatherHumidityLabel, HUMIDITY_SYMBOL " %d%%", 76);
    lv_obj_set_style_text_font(weatherHumidityLabel, &weather_icon_font_full_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align_to(weatherHumidityLabel, weatherProbabilityLabel, LV_ALIGN_OUT_RIGHT_MID,
                    15, -1);

    weatherPressureLabel = lv_label_create(weatherPanel);
    lv_label_set_text_fmt(weatherPressureLabel, PRESSURE_SYMBOL " %d мбар", 1027);
    lv_obj_set_style_text_font(weatherPressureLabel, &weather_icon_font_full_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align_to(weatherPressureLabel, weatherHumidityLabel, LV_ALIGN_OUT_RIGHT_MID,
                    15, -1);

    weatherTimesLabel = lv_label_create(weatherPanel);
    lv_obj_set_pos(weatherTimesLabel, 0, 93);
    lv_obj_set_align(weatherTimesLabel, LV_ALIGN_CENTER);
    lv_label_set_text(weatherTimesLabel,
                      "Ночь               Утро               День               Вечер");

    weatherFirstTempLabel = lv_label_create(weatherPanel);
    lv_obj_set_pos(weatherFirstTempLabel, 55, -10);
    lv_obj_set_align(weatherFirstTempLabel, LV_ALIGN_BOTTOM_LEFT);
    lv_label_set_text(weatherFirstTempLabel, "-7°C");

    weatherSecondTempLabel = lv_label_create(weatherPanel);
    lv_obj_align_to(weatherSecondTempLabel, weatherFirstTempLabel, LV_ALIGN_OUT_RIGHT_TOP,
                    65, 0);
    lv_label_set_text(weatherSecondTempLabel, "-10°C");

    weatherThirdTempLabel = lv_label_create(weatherPanel);
    lv_obj_align_to(weatherThirdTempLabel, weatherSecondTempLabel, LV_ALIGN_OUT_RIGHT_TOP,
                    65, 0);
    lv_label_set_text(weatherThirdTempLabel, "-13°C");

    weatherFourthTempLabel = lv_label_create(weatherPanel);
    lv_obj_align_to(weatherFourthTempLabel, weatherThirdTempLabel, LV_ALIGN_OUT_RIGHT_TOP,
                    65, 0);
    lv_label_set_text(weatherFourthTempLabel, "-18°C");
}
Weather::~Weather(){};