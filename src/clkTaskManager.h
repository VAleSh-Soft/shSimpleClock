/**
 * @file clkTaskManager.h
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 *
 * @brief диспетчер задач;
 *        полная версия здесь - https://github.com/VAleSh-Soft/shTaskManager
 *
 * @version 1.7
 * @date 22.09.2026
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

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

class sscTaskList
{
public:
  clkHandle rtc_guard = CLK_INVALID_HANDLE;              // опрос микросхемы RTC по таймеру, чтобы не дергать ее откуда попало
  clkHandle blink_timer = CLK_INVALID_HANDLE;            // блинк
  clkHandle return_to_default_mode = CLK_INVALID_HANDLE; // таймер автовозврата в режим показа времени из любого режима настройки
  clkHandle set_time_mode = CLK_INVALID_HANDLE;          // режим настройки времени
  clkHandle display_guard = CLK_INVALID_HANDLE;          // вывод данных на экран
#if defined(USE_ALARM)
  clkHandle alarm_guard = CLK_INVALID_HANDLE;  // отслеживание состояния будильника
  clkHandle alarm_buzzer = CLK_INVALID_HANDLE; // пищалка будильника
#endif
#if __USE_AUTO_SHOW_DATA__
  clkHandle auto_show_mode = CLK_INVALID_HANDLE; // автоматический вывод даты и/или температуры
#endif
#if __USE_TEMP_DATA__ && defined(USE_DS18B20)
  clkHandle ds18b20_guard = CLK_INVALID_HANDLE; // опрос датчика DS18b20
#endif
#if __USE_LIGHT_SENSOR__
  clkHandle light_sensor_guard = CLK_INVALID_HANDLE; // отслеживание показаний датчика освещенности
#endif
#if __USE_OTHER_SETTING__
  clkHandle other_setting_mode = CLK_INVALID_HANDLE; // режим настроек цифровых данных
#endif
#if defined(USE_TICKER_FOR_DATA)
  clkHandle ticker = CLK_INVALID_HANDLE; // отработка бегущей строки
#endif

  sscTaskList() {};
};

sscTaskList clkTaskList;

class clkTaskManager
{
private:
  uint8_t task_count = 0; // количество штатных задач
  clkTask *task_list = nullptr;

  bool isValidHandle(clkHandle _handle);

public:
  clkTaskManager();

  // объект владеет динамическим массивом задач - копирование запрещено
  clkTaskManager(const clkTaskManager &_other) = delete;
  clkTaskManager &operator=(const clkTaskManager &_other) = delete;

  void init(uint8_t _count);

  void tick();

  clkHandle addTask(unsigned long _interval, clkTaskManagerCallback _callback, bool isActive = true);

  void startTask(clkHandle _handle);

  void stopTask(clkHandle _handle);

  bool getTaskState(clkHandle _handle);

  // диагностика: интервал задачи и время, прошедшее с последнего запуска таймера
  // задачи (0, если хендл невалиден); нужны, чтобы отличить "таймер постоянно
  // перезапускают" от "портится поле interval"
  unsigned long getTaskInterval(clkHandle _handle);
  unsigned long getTaskTimer(clkHandle _handle);

  void setTaskInterval(clkHandle _handle, unsigned long _interval, bool _restart = true);

  void taskExes(clkHandle _handle, bool _restart = true);

  uint8_t getTaskCount();
};

// ---- clkTaskManager private ------------------

bool clkTaskManager::isValidHandle(clkHandle _handle)
{
  return (task_list != nullptr &&
          _handle > CLK_INVALID_HANDLE &&
          _handle < task_count);
}
// ---- clkTaskManager public -------------------

clkTaskManager::clkTaskManager() {}

void clkTaskManager::init(uint8_t _count)
{
  if (task_list != nullptr) // защита от утечки при повторном вызове init()
  {
    delete[] task_list;
    task_list = nullptr;
  }

  task_count = (_count) ? _count : 1;

  // общее число задач не должно выходить за диапазон clkHandle (int8_t)
  if (task_count > CLK_MAX_TASK_COUNT)
  {
    task_count = CLK_MAX_TASK_COUNT;
  }

  task_list = (clkTask *)calloc((task_count), sizeof(clkTask));

  if (task_list == nullptr)
  {
    task_count = 0;
  }
}

void clkTaskManager::tick()
{
  if (task_list != nullptr)
  {
    for (uint8_t i = 0; i < (task_count); i++)
    {
      if (task_list[i].status && task_list[i].callback != nullptr)
      {
        unsigned long elapsed = millis() - task_list[i].timer;
        if (elapsed >= task_list[i].interval)
        {
          if (elapsed - task_list[i].interval >= task_list[i].interval)
          {
            // после длительного простоя (блокировки loop() и т.п.) не устраиваем
            // серию "догоняющих" вызовов - синхронизируем таймер с текущим временем
            task_list[i].timer = millis();
          }
          else
          {
            task_list[i].timer += task_list[i].interval;
          }
          task_list[i].callback();
        }
      }
    }
  }
}

clkHandle clkTaskManager::addTask(unsigned long _interval, clkTaskManagerCallback _callback, bool isActive)
{
  if (task_list != nullptr && _callback != nullptr)
  {
    for (uint8_t i = 0; i < (task_count); i++)
    {
      if (!task_list[i].callback)
      {
        task_list[i].status = isActive;
        task_list[i].interval = _interval;
        task_list[i].callback = _callback;
        task_list[i].timer = millis();
        return (i);
      }
    }
  }
  return (CLK_INVALID_HANDLE);
}

void clkTaskManager::startTask(clkHandle _handle)
{
  if (isValidHandle(_handle) && task_list[_handle].callback != nullptr)
  {
    task_list[_handle].status = true;
    task_list[_handle].timer = millis();
  }
}

void clkTaskManager::stopTask(clkHandle _handle)
{
  if (isValidHandle(_handle))
  {
    task_list[_handle].status = false;
  }
}

bool clkTaskManager::getTaskState(clkHandle _handle)
{
  if (isValidHandle(_handle))
  {
    return (task_list[_handle].status && task_list[_handle].callback != nullptr);
  }

  return (false);
}

unsigned long clkTaskManager::getTaskInterval(clkHandle _handle)
{
  if (isValidHandle(_handle))
  {
    return (task_list[_handle].interval);
  }

  return (0);
}

unsigned long clkTaskManager::getTaskTimer(clkHandle _handle)
{
  return (task_list[_handle].timer);
}

void clkTaskManager::setTaskInterval(clkHandle _handle, unsigned long _interval, bool _restart)
{
  if (isValidHandle(_handle))
  {
    task_list[_handle].interval = _interval;
    if (_restart && (task_list[_handle].callback != nullptr))
    {
      task_list[_handle].status = true;
      task_list[_handle].timer = millis();
    }
  }
}

void clkTaskManager::taskExes(clkHandle _handle, bool _restart)
{
  if (isValidHandle(_handle))
  {
    if (task_list[_handle].callback != nullptr)
    {
      if (_restart)
      {
        task_list[_handle].status = true;
        task_list[_handle].timer = millis();
      }
      task_list[_handle].callback();
    }
  }
}

uint8_t clkTaskManager::getTaskCount()
{
  uint8_t _task_count = 5; // базовое количество задач
#if defined(USE_ALARM)
  _task_count += 2;
#endif
#if __USE_AUTO_SHOW_DATA__
  _task_count++;
#endif
#if __USE_TEMP_DATA__ && defined(USE_DS18B20)
  _task_count++;
#endif
#if __USE_LIGHT_SENSOR__
  _task_count++;
#endif
#if __USE_OTHER_SETTING__
  _task_count++;
#endif
#if defined(USE_TICKER_FOR_DATA)
  _task_count++;
#endif

  return (_task_count);
}

// ==== end clkTaskManager ===========================

clkTaskManager clkTasks;

#if ADDITIONAL_TASKS_COUNT > 0
clkTaskManager clkAddTasks;
#endif
