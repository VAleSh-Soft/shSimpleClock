/**
 * @file additional_button.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 *
 * @brief Пример использования дополнительной кнопки CLK_BTN_ADD1; кнопка
 *        используется для включения/выключения и настройки будильника;
 *
 *        ВАЖНО: объявление файла clockSetting.h в скетче должно быть ДО
 *               объявления библиотеки shSimpleClock.h (см. скетч ниже)
 *
 *        Использование кнопки: - одиночный клик - включение/выключение будильника;
 *                              - двойной клик - вход в режим настройки времени
 *                                срабатывания будильника.
 *
 * @version 1.0
 * @date 13.05.2026
 *
 * @copyright Copyright (c) 2026
 *
 */
#include "clockSetting.h"
#include <shSimpleClock.h> // https://github.com/VAleSh-Soft/shSimpleClock

// объявляем экземпляр часов
shSimpleClock simple_clock;

void clk_btn_add1()
{
  switch (simple_clock.getButtonState(CLK_BTN_ADD1))
  {
  // одиночный клик переключает состояние будильника - включен/выключен
  case BTN_ONECLICK:
    simple_clock.setOnOffAlarm(!simple_clock.getOnOffAlarm());
    Serial.print("Alarm is "); 
    Serial.println(simple_clock.getOnOffAlarm() ? "ON" : "OFF");
    break;
  // двойной клик - вход в режим настройки времени срабатывания будильника
  case BTN_DBLCLICK:
    simple_clock.setDisplayMode(DISPLAY_MODE_SET_ALARM_HOUR);
    Serial.println("The alarm time setting mode has been started");
    break;
  }
}

void setup()
{
  Serial.begin(115200);

  // инициализируем часы
  simple_clock.init();
}

void loop()
{
  // обработка событий часов
  simple_clock.tick();

  // обработка событий дополнительной кнопки
  clk_btn_add1();
}