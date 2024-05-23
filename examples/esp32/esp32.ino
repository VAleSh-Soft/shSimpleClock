/**
 * @file esp32.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * 
 * @brief Пример сборки часов на базе esp32; в файле clockSetting.h приведены
 *        примерные варианты подключения периферии с учетом доступности пинов 
 *        микроконтроллера.
 * 
 *        Для компиляции скетча используйте аддон esp32 - 
 *        https://dl.espressif.com/dl/package_esp32_index.json
 * 
 * @version 1.0
 * @date 01.04.2024
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
