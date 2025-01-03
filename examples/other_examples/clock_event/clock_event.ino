/**
 * @file clock_event.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * 
 * @brief Пример использования событий часов при использовании библиотеки
 *        shSimpleClock; показано использование ежесекундного события и 
 *        события будильника;
 * 
 *        ВАЖНО: объявление файла clockSetting.h в скетче должно быть ДО
 *               объявления библиотеки shSimpleClock.h (см. скетч ниже)
 * 
 *        Вход в настройки будильника - двойной клик кнопкой Set.
 * 
 * @version 1.0
 * @date 11.03.2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "clockSetting.h"
#include <shSimpleClock.h> // https://github.com/VAleSh-Soft/shSimpleClock

// объявляем экземпляр часов
shSimpleClock simple_clock;

void clockEventExample()
{
  static uint16_t count = 0;
  
  count += 5;
  Serial.print(count);
  Serial.println(" seconds have passed");
}

void alarmEventExample()
{
  Serial.println("The alarm went off");
}

void setup()
{
  Serial.begin(115200);

  // подключаем событие часов - вызывается через каждые пять секунд
  simple_clock.setClockEvent(5, clockEventExample);

  // подключаем событие будильника
  simple_clock.setAlarmEvent(alarmEventExample);

  // инициализируем часы
  simple_clock.init();
}

void loop()
{
  // обработка событий часов
  simple_clock.tick();
}