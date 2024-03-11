/**
 * @file clock_event.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * 
 * @brief Пример использования событий часов при использовании библиотеки
 *        shSimpleClock; показано использование ежесекундного события и 
 *        события будильника;
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
shSimpleClock clock;

void clockEventExample()
{
  static uint16_t count = 0;
  
  count += 5;
  Serial.print(count);
  Serial.println(" seconds have passed");
}

void AlarmEventExample()
{
  Serial.println("The alarm went off");
}

void setup()
{
  Serial.begin(9600);

  // подключаем событие часов - вызывается через каждые пять секунд
  clock.setClockEvent(5, clockEventExample);

  // подключаем событие будильника
  clock.setAlarmEvent(AlarmEventExample);

  // инициализируем часы
  clock.init();
}

void loop()
{
  // обработка событий часов
  clock.tick();
}