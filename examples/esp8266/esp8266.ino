/**
 * @file esp8266.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * 
 * @brief Пример сборки часов на базе esp8266; в файле clockSetting.h приведены
 *        примерные варианты подключения периферии с учетом доступности пинов 
 *        микроконтроллера.
 * 
 *        В виду того, что у esp8266 только один аналоговый вход, одновременно
 *        использовать датчик освещения и NTC-термистор в качестве датчика
 *        температуры не получится ))
 * 
 *        В то же время и датчик ds28b20 если и можно будет использовать только 
 *        если отказаться от одной из кнопок. В общем, полноценный набор опций
 *        собрать будет невозможно.
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
