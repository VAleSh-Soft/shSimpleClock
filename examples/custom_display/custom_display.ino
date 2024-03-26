/**
 * @file custom_display.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * @brief Пример вывода пользовательской информации на экран часов;
 *
 * Вывод данных по двойному клику кнопкой Up;
 *
 * @version 1.0
 * @date 26.03.2024
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
  // инициализируем часы
  clock.init();
}

void loop()
{
  // обработка событий часов
  clock.tick();

  // переключаем режим экрана по двойному клику кнопкой Up
  if (clock.getButtonState(CLK_BTN_UP) == BTN_DBLCLICK)
  {
    clock.setDisplayMode(DISPLAY_MODE_CUSTOM_1);
  }

  // выводим наборы символов на экран от "0000" до "FFFF"; смена символов - каждые 500 милисекунд
  if (clock.getDisplayMode() == DISPLAY_MODE_CUSTOM_1)
  {
    static uint32_t timer = 0;
    if (millis() - timer >= 500)
    {
      timer = millis();

      static uint8_t data = 0x00;
      for (uint8_t i = 0; i < 4; i++)
      {
        sscDisp.setDispData(i, sscDisp.encodeDigit(data));
      }
      // после завершения вернуть экран в режим отображения текущего времени
      if (data++ > 0x0F)
      {
        data = 0x00;
        clock.setDisplayMode(DISPLAY_MODE_SHOW_TIME);
      }
    }
  }
}