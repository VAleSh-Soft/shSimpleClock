/**
 * @file clkTaskManager.h
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 *
 * @brief диспетчер задач;
 *        полная версия здесь - https://github.com/VAleSh-Soft/shTaskManager
 *
 * @version 1.6
 * @date 17.05.2026
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <new> // для new (std::nothrow)

// ==== clkTaskManager ===============================

typedef void (*clkTaskManagerCallback)(void);  // тип - указатель для Callback-функции
typedef int8_t clkHandle;                      // тип - идентификатор задачи
static const uint8_t CLK_MAX_TASK_COUNT = 127; // clkHandle - знаковый int8_t, поэтому индексы выше 127
                                               // неработоспособны (см. isValidHandle)
static const clkHandle CLK_INVALID_HANDLE = -1;

struct clkTask // структура, описывающая задачу
{
  bool status;                     // статус задачи
  unsigned long timer;             // таймер задачи
  unsigned long interval;          // интервал срабатывания задачи
  clkTaskManagerCallback callback; // функция, вызываемая при срабатывании таймера задачи

  clkTask() : status(false), timer(0ul), interval(0ul), callback(nullptr) {}
};

class clkTaskManager
{
private:
  uint8_t task_count = 0;     // количество штатных задач
  uint8_t add_task_count = 0; // количество пользовательских задач
  clkTask *taskList = nullptr;

  bool isValidHandle(clkHandle _handle);

public:
  clkHandle rtc_guard;              // опрос микросхемы RTC по таймеру, чтобы не дергать ее откуда попало
  clkHandle blink_timer;            // блинк
  clkHandle return_to_default_mode; // таймер автовозврата в режим показа времени из любого режима настройки
  clkHandle set_time_mode;          // режим настройки времени
  clkHandle display_guard;          // вывод данных на экран
#if defined(USE_ALARM)
  clkHandle alarm_guard;  // отслеживание состояния будильника
  clkHandle alarm_buzzer; // пищалка будильника
#endif
#if __USE_AUTO_SHOW_DATA__
  clkHandle auto_show_mode; // автоматический вывод даты и/или температуры
#endif
#if __USE_TEMP_DATA__ && defined(USE_DS18B20)
  clkHandle ds18b20_guard; // опрос датчика DS18b20
#endif
#if __USE_LIGHT_SENSOR__
  clkHandle light_sensor_guard; // отслеживание показаний датчика освещенности
#endif
#if __USE_OTHER_SETTING__
  clkHandle other_setting_mode; // режим настроек цифровых данных
#endif
#if defined(USE_TICKER_FOR_DATA)
  clkHandle ticker; // отработка бегущей строки
#endif

  clkTaskManager();

  // объект владеет динамическим массивом задач - копирование запрещено
  clkTaskManager(const clkTaskManager &_other) = delete;
  clkTaskManager &operator=(const clkTaskManager &_other) = delete;

  void init();

  void tick();

  clkHandle addTask(unsigned long _interval, clkTaskManagerCallback _callback, bool isActive = true);

  void startTask(clkHandle _handle);

  void stopTask(clkHandle _handle);

  bool getTaskState(clkHandle _handle);

  void setTaskInterval(clkHandle _handle, unsigned long _interval, bool _restart = true);

  void taskExes(clkHandle _handle, bool _restart = true);

  void setAddTaskCount(uint8_t _add_count);
};

// ---- clkTaskManager private ------------------

bool clkTaskManager::isValidHandle(clkHandle _handle)
{
  return (taskList != nullptr &&
          _handle > CLK_INVALID_HANDLE &&
          _handle <= CLK_MAX_TASK_COUNT &&
          _handle < (task_count + add_task_count));
}
// ---- clkTaskManager public -------------------

clkTaskManager::clkTaskManager() {}

void clkTaskManager::init()
{
  if (taskList != nullptr) // защита от утечки при повторном вызове init()
  {
    delete[] taskList;
    taskList = nullptr;
  }

  task_count = 5; // базовое количество задач
#if defined(USE_ALARM)
  task_count += 2;
#endif
#if __USE_AUTO_SHOW_DATA__
  task_count++;
#endif
#if __USE_TEMP_DATA__ && defined(USE_DS18B20)
  task_count++;
#endif
#if __USE_LIGHT_SENSOR__
  task_count++;
#endif
#if __USE_OTHER_SETTING__
  task_count++;
#endif
#if defined(USE_TICKER_FOR_DATA)
  task_count++;
#endif

  // общее число слотов не должно выходить за диапазон clkHandle (int8_t)
  if (add_task_count + task_count > CLK_MAX_TASK_COUNT)
  {
    add_task_count = CLK_MAX_TASK_COUNT - task_count;
  }
  taskList = new (std::nothrow) clkTask[task_count + add_task_count];

  if (taskList == nullptr)
  {
    task_count = 0;
    add_task_count = 0;
  }
}

void clkTaskManager::tick()
{
  if (taskList != nullptr)
  {
    for (uint8_t i = 0; i < (task_count + add_task_count); i++)
    {
      if (taskList[i].status && taskList[i].callback != nullptr)
      {
        unsigned long elapsed = millis() - taskList[i].timer;
        if (elapsed >= taskList[i].interval)
        {
          if (elapsed - taskList[i].interval >= taskList[i].interval)
          {
            // после длительного простоя (блокировки loop() и т.п.) не устраиваем
            // серию "догоняющих" вызовов - синхронизируем таймер с текущим временем
            taskList[i].timer = millis();
          }
          else
          {
            taskList[i].timer += taskList[i].interval;
          }
          taskList[i].callback();
        }
      }
    }
  }
}

clkHandle clkTaskManager::addTask(unsigned long _interval, clkTaskManagerCallback _callback, bool isActive)
{
  if (taskList != nullptr && _callback != nullptr)
  {
    for (uint8_t i = 0; i < (task_count + add_task_count); i++)
    {
      if (!taskList[i].callback)
      {
        taskList[i].status = isActive;
        taskList[i].interval = _interval;
        taskList[i].callback = _callback;
        taskList[i].timer = millis();
        return (i);
      }
    }
  }
  return (CLK_INVALID_HANDLE);
}

void clkTaskManager::startTask(clkHandle _handle)
{
  if (isValidHandle(_handle) && taskList[_handle].callback != nullptr)
  {
    taskList[_handle].status = true;
    taskList[_handle].timer = millis();
  }
}

void clkTaskManager::stopTask(clkHandle _handle)
{
  if (isValidHandle(_handle))
  {
    taskList[_handle].status = false;
  }
}

bool clkTaskManager::getTaskState(clkHandle _handle)
{
  if (isValidHandle(_handle))
  {
    return (taskList[_handle].status && taskList[_handle].callback != nullptr);
  }

  return (false);
}

void clkTaskManager::setTaskInterval(clkHandle _handle, unsigned long _interval, bool _restart)
{
  if (isValidHandle(_handle))
  {
    taskList[_handle].interval = _interval;
    if (_restart && (taskList[_handle].callback != nullptr))
    {
      taskList[_handle].status = true;
      taskList[_handle].timer = millis();
    }
  }
}

void clkTaskManager::taskExes(clkHandle _handle, bool _restart)
{
  if (isValidHandle(_handle))
  {
    if (taskList[_handle].callback != nullptr)
    {
      if (_restart)
      {
        taskList[_handle].status = true;
        taskList[_handle].timer = millis();
      }
      taskList[_handle].callback();
    }
  }
}

void clkTaskManager::setAddTaskCount(uint8_t _add_count)
{
  add_task_count = (_add_count > CLK_MAX_TASK_COUNT) ? CLK_MAX_TASK_COUNT : _add_count;
}

// ==== end clkTaskManager ===========================

clkTaskManager clkTasks;
