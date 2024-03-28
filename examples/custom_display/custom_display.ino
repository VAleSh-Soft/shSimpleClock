/**
 * @file custom_display.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * @brief Пример вывода пользовательской информации на экран часов;
 *
 *        Вывод данных по двойному клику кнопкой Up на примере часов с 
 *        семисегментным индикатором на драйвере TM1637;
 * 
 *        Так же показано использование флагов кнопок управления часами для 
 *        управления пользовательским экраном;
 * 
 *        ВАЖНО: объявление файла clockSetting.h в скетче должно быть ДО
 *               объявления библиотеки shSimpleClock.h (см. скетч ниже)

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

void setCustomDisplay()
{
  if (clock.getDisplayMode() == DISPLAY_MODE_CUSTOM_1)
  {
    // выводим наборы символов на экран от "0000" до "FFFF"; смена символов - каждые 500 милисекунд
    static uint32_t timer = 0;
    if (millis() - timer >= 500)
    {
      timer = millis();

      static uint8_t data = 0x00;
      for (uint8_t i = 0; i < 4; i++)
      {
        clkDisp.setDispData(i, clkDisp.encodeDigit(data));
      }
      // после завершения автоматически вернуть экран в режим отображения текущего времени
      if (data++ > 0x0F)
      {
        data = 0x00;
        clock.setDisplayMode(DISPLAY_MODE_SHOW_TIME);
      }
    }

    // или немедленно вернуться в режим отображения текущего времени при удержании нажатой кнопки Set
    if (clock.getButtonFlag(CLK_BTN_SET, true) == CLK_BTN_FLAG_EXIT)
    {
      clock.setDisplayMode(DISPLAY_MODE_SHOW_TIME);
    }
  }
}

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

  // работа с пользовательским экраном
  setCustomDisplay();
}