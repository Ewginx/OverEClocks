#include "Translation/Translation.h"

#if LANGUAGE == 1
    const char *settings_translation[] = {"Светлая тема","Авто", "Город, код страны ISO-81118", "Город и\nкод страны", "WiFi SSID", "WiFi SSID", "WiFi пароль", "WiFi пароль", "AccessPoint IP:"};
    const char *digital_clock_translation[] = {"Вc","Пн","Вт", "Ср", "Чт", "Пт", "Сб" };
    const char *alarm_translation[] = {"По будням","СБ ВС","Однократно", "Ок", "Отмена", "Через ", " д ", " ч ", " м"};
    const char *weather_translation[] = {"По будням","СБ ВС","Однократно", "Ок", "Отмена"};
#endif



// std::map<String, String> settings_translation = {
//     {String("Auto"), String("Авто")}, 
//     {String("WiFi Password"), String("WiFi пароль")}
//     {String("WiFi SSID"), String("WiFi SSID")}
//     };