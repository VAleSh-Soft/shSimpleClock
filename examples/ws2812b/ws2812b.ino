/**
 * @file ws2812b.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * 
 * @brief Пример быстрого создания часов с помощью библиотеки shSimpleClock
 *        с использованием часового модуля DS3231 и матрицы 8x32, построенной
 *        на адресных светодиодах WS2812B; настройки часов указываются в файле 
 *        clockSetting.h, который следует разместить в папке со скетчем;
 * 
 *        ВАЖНО: объявление файла clockSetting.h в скетче должно быть ДО
 *               объявления библиотеки shSimpleClock.h (см. скетч ниже)
 * 
 *        В скетче так же показан способ использования кнопок, предназначенных
 *        для управления часами, в своих целях;
 * 
 * @version 1.0
 * @date 03.03.2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "clockSetting.h"
#include <shSimpleClock.h> // https://github.com/VAleSh-Soft/shSimpleClock
#include <shButton.h>      // https://github.com/VAleSh-Soft/shButton

// объявляем экземпляр часов
shSimpleClock clock;

void setup()
{
  Serial.begin(9600);

  // устанавливаем синий цвет для цифр
  clock.setColorOfNumber(CRGB::Blue);
  // инициализируем часы
  clock.init();
}

void loop()
{
  // обработка событий часов
  clock.tick();

  // пример использования событий кнопок управления часами в своих целях
  if (clock.getBtnUpState() == BTN_DBLCLICK)
  {
    // например, обрабатываем событие двойного клика кнопкой Up
    Serial.println("Double click of button Up");
  }
}
