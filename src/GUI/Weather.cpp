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
    lv_img_set_src(weatherImage, &img_day_clear_png);
    lv_obj_set_size(weatherImage, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 128
    lv_obj_set_pos(weatherImage, 10, 30);
    lv_obj_set_align(weatherImage, LV_ALIGN_TOP_LEFT);

    windLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(windLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 121
    lv_obj_set_pos(windLabel, 140, -75);
    lv_obj_set_align(windLabel, LV_ALIGN_CENTER);
    lv_label_set_text(windLabel, "Ветер: 14.1 км/ч ЗСЗ");
    lv_obj_set_style_text_font(windLabel, &montserrat_14,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherHumidityLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherHumidityLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 121
    lv_obj_align_to(weatherHumidityLabel, windLabel, LV_ALIGN_BOTTOM_LEFT, 0, 25);
    lv_label_set_text(weatherHumidityLabel, "Влажность: 75%"); 
    lv_obj_set_style_text_font(weatherHumidityLabel, &montserrat_14,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherPressureLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherPressureLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 121
    lv_obj_align_to(weatherPressureLabel, weatherHumidityLabel, LV_ALIGN_BOTTOM_LEFT, 0,
                    25);
    lv_label_set_long_mode(weatherPressureLabel, LV_LABEL_LONG_CLIP);
    lv_label_set_text(weatherPressureLabel, "Давление: 1027 мбар");
    lv_obj_set_style_text_font(weatherPressureLabel, &montserrat_14,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherCityLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherCityLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(weatherCityLabel, 0, 30);
    lv_obj_set_align(weatherCityLabel, LV_ALIGN_TOP_MID);
    lv_label_set_text(weatherCityLabel, "city, RU");
    lv_label_set_long_mode(weatherCityLabel, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_font(weatherCityLabel, &montserrat_14,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherTemperatureLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherTemperatureLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(weatherTemperatureLabel, -10, -45);
    lv_obj_set_align(weatherTemperatureLabel, LV_ALIGN_CENTER);
    lv_label_set_text(weatherTemperatureLabel, "13°C");
    lv_obj_set_style_text_font(weatherTemperatureLabel, &montserrat_48,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    feelsLikeLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(feelsLikeLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 121
    lv_obj_set_pos(feelsLikeLabel, 40, 5);
    lv_obj_set_align(feelsLikeLabel, LV_ALIGN_CENTER);
    lv_label_set_text(feelsLikeLabel, "Ощущается как: -24°C");
    lv_obj_set_style_text_font(feelsLikeLabel, &montserrat_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    briefingLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(briefingLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(briefingLabel, 0, 35);
    lv_obj_set_align(briefingLabel, LV_ALIGN_CENTER);
    lv_label_set_text(briefingLabel, "Переменная облачность. Возможен дождь.");
    lv_obj_set_style_text_font(briefingLabel, &montserrat_14,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherMaxTempLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherMaxTempLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(weatherMaxTempLabel, -150, 65);
    lv_obj_set_align(weatherMaxTempLabel, LV_ALIGN_CENTER);
    lv_label_set_text(weatherMaxTempLabel, "Макс. t: -12°C");
    lv_obj_set_style_text_font(weatherMaxTempLabel, &montserrat_14,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherMinTempLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherMinTempLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_align_to(weatherMinTempLabel, weatherMaxTempLabel, LV_ALIGN_OUT_RIGHT_MID, 15,
                    -1);
    lv_label_set_text(weatherMinTempLabel, "Мин. t: -20°C");
    lv_obj_set_style_text_font(weatherMinTempLabel, &montserrat_14,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherProbabilityLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherProbabilityLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_align_to(weatherProbabilityLabel, weatherMinTempLabel, LV_ALIGN_OUT_RIGHT_MID,
                    15, -1);
    lv_label_set_text(weatherProbabilityLabel, "Вероятность осадков: 0%");
    lv_obj_set_style_text_font(weatherProbabilityLabel, &montserrat_14,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherTimesLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherTimesLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(weatherTimesLabel, 0, 93);
    lv_obj_set_align(weatherTimesLabel, LV_ALIGN_CENTER);
    lv_label_set_text(weatherTimesLabel,
                      "Ночь               Утро               День               Вечер");

    weatherFirstTempLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherFirstTempLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_set_pos(weatherFirstTempLabel, 55, -10);
    lv_obj_set_align(weatherFirstTempLabel, LV_ALIGN_BOTTOM_LEFT);
    lv_label_set_text(weatherFirstTempLabel, "-7°C");

    weatherSecondTempLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherSecondTempLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_align_to(weatherSecondTempLabel, weatherFirstTempLabel, LV_ALIGN_OUT_RIGHT_TOP,
                    65, 0);
    lv_label_set_text(weatherSecondTempLabel, "-10°C");

    weatherThirdTempLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherThirdTempLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_align_to(weatherThirdTempLabel, weatherSecondTempLabel, LV_ALIGN_OUT_RIGHT_TOP,
                    65, 0);
    lv_label_set_text(weatherThirdTempLabel, "-13°C");

    weatherFourthTempLabel = lv_label_create(weatherPanel);
    lv_obj_set_size(weatherFourthTempLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); /// 1
    lv_obj_align_to(weatherFourthTempLabel, weatherThirdTempLabel, LV_ALIGN_OUT_RIGHT_TOP,
                    65, 0);
    lv_label_set_text(weatherFourthTempLabel, "-18°C");
}
Weather::~Weather(){};