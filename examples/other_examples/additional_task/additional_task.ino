/**
 * @file additional_task.ino
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * 
 * @brief Пример добавления пользовательской задачи в список задач диспетчера
 *        задач библиотеки;
 * 
 *        Для добавления пользовательских задач нужно указать диспетчеру, 
 *        сколько дополнительных задач вы будете использовать; для этого 
 *        используется метод setAdditionalTaskCount(), который ОБЯЗАТЕЛЬНО
 *        должен быть вызван ПЕРЕД инициализацией часов.
 * 
 *        После инициализации часов нужно добавить задачи в список; для этого
 *        используется метод addAdditionalTask() с указанием параметров задачи.
 * 
 *        Метод, вызываемый задачей, не должен иметь параметров и возвращать
 *        что либо; кроме того, здесь показан пример управления задачи, которая
 *        сама останавливается через три минуты (180 срабатываний с интервалом
 *        в одну секунду).
 * 
 * @version 1.0
 * @date 2026-06-13
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include "clockSetting.h"
#include <shSimpleClock.h> // https://github.com/VAleSh-Soft/shSimpleClock

// объявляем экземпляр часов
shSimpleClock simple_clock;

clkHandle new_task;

void addedTask();

void addedTask()
{
  static uint16_t seconds_count = 0;
  Serial.print(++seconds_count);
  Serial.println(" seconds have passed");

  // через три минуты задача останавливается
  if (seconds_count >= 180)
  {
    simple_clock.stopAdditionalTask(new_task);
    Serial.println("Task stopped");
  }
  
}

void setup()
{
  Serial.begin(115200);

  // увеличиваем список задач на одну дополнительную задачу; 
  // вызывать ПЕРЕД методом init()
  simple_clock.setAdditionalTaskCount(1);
  // инициализируем часы
  simple_clock.init();
  // подключаем дополнительную задачу; выполнять ПОСЛЕ вызова метода init()
  new_task = simple_clock.addAdditionalTask(1000ul, addedTask);
}

void loop()
{
  // обработка событий часов
  simple_clock.tick();
}
