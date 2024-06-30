/**
 * @file rp2040.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * 
 * @brief Пример быстрого создания часов на базе rp2040; в файле clockSetting.h
 *        приведены примерные варианты подключения периферии с учетом 
 *        доступности пинов  микроконтроллера;
 * 
 *        Для компиляции скетча используйте аддон Raspberry Pi RP2040 Boards - 
 * https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
 * 
 * @version 1.0
 * @date 23.05.2024
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
