/**
 * @file lcd2004_i2c.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * @brief Пример быстрого создания часов с помощью библиотеки shSimpleClock
 *        с использованием часового модуля DS3231 и текстового LCD 1602 экрана,
 *        подключаемого с помощью адаптера **I2C** на базе чипа **PCF8574**;
 *        настройки часов указываются в файле clockSetting.h, который следует
 *        разместить в папке со скетчем;
 *
 *        ВАЖНО: объявление файла clockSetting.h в скетче должно быть ДО
 *               объявления библиотеки shSimpleClock.h (см. скетч ниже)
 *
 *        В скетче так же показан способ использования кнопок, предназначенных
 *        для управления часами, в своих целях, а так же вывод произвольного
 *        текста в свободных строках;
 * @version 1.0
 * @date 17.05.2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "clockSetting.h"
#include <shSimpleClock.h> // https://github.com/VAleSh-Soft/shSimpleClock

// объявляем экземпляр часов
shSimpleClock simple_clock;

void setup()
{
  Serial.begin(9600);

  // инициализируем часы
  simple_clock.init();

  simple_clock.printTextForScreen(0, 3, "shSimpleClock.h lib");
}

void loop()
{
  // обработка событий часов
  simple_clock.tick();

  // пример использования событий кнопок управления часами в своих целях
  if (simple_clock.getButtonState(CLK_BTN_UP) == BTN_DBLCLICK)
  {
    // например, управляем подсветкой экрана по двойному клику кнопкой Up
    static bool light = true;

    light = !light;
    simple_clock.setBacklightState(light);
    if (light)
    {
      Serial.println("Backlight ON");
    }
    else
    {
      Serial.println("Backlight OFF");
    }
  }
}
