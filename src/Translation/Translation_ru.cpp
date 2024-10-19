#include "Translation/Translation.h"

#if LANGUAGE == 1
    const char *settings_translation[] = {"Темная тема","Авто", "Moscow", "ru", "Город и код страны", "WiFi SSID", "WiFi SSID", "WiFi пароль", "WiFi пароль", "AccessPoint IP адрес:", "Переподключить WiFi", "Обновление погоды", "Обновить погоду"};
    const char *digital_clock_translation[] = {"Вc","Пн","Вт", "Ср", "Чт", "Пт", "Сб" };
    const char *alarm_translation[] = {"По будням","СБ ВС","Однократно", "Ок", "Отмена", "Через ", " д ", " ч ", " м", "Отключить", "Отложить"};
    const char *weather_translation[] = {"км/ч","По ощущениям","мбар", "Ночью           ", "Утром           ", "Днём           ", "Вечером", "Н/Д"};
#endif



// std::map<String, String> settings_translation = {
//     {String("Auto"), String("Авто")}, 
//     {String("WiFi Password"), String("WiFi пароль")}
//     {String("WiFi SSID"), String("WiFi SSID")}
//     };