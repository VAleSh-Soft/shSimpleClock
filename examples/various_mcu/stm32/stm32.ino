/**
 * @file esp32.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * 
 * @brief Пример сборки часов на базе stm32; в файле clockSetting.h приведены
 *        примерные варианты подключения периферии с учетом доступности пинов 
 *        микроконтроллера.
 * 
 *        Для компиляции скетча можно использовать аддоны:
 *        - STM32 MCU based boards (stm32duino), ссылка для установки:
 *          - https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json
 *        - Arduino_STM32 by Roger Clark, ссылка на **github.com**:
 *          - https://github.com/rogerclarkmelbourne/Arduino_STM3
 *          Инструкция по установке:
 *          - https://github.com/rogerclarkmelbourne/Arduino_STM32/wiki/Installation
 * 
 *       ВАЖНО!!! - аддон stm32duino разрабатывается и поддерживается 
 *       производителем микроконтроллеров STM32 - STMicroelectronics и, 
 *       соответственно, более предпочтителен к использованию.
 * 
 * @version 1.0
 * @date 30.06.2024
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
  // инициализируем часы
  simple_clock.init();
}

void loop()
{
  // обработка событий часов
  simple_clock.tick();
}
