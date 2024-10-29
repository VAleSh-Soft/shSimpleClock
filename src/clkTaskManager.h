/**
 * @file clkTaskManager.h
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 *
 * @brief диспетчер задач;
 *        полная версия здесь - https://github.com/VAleSh-Soft/shTaskManager
 *
 * @version 1.5
 * @date 01.05.2024
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

// ==== clkTaskManager ===============================

typedef void (*clkCallback)(void); // тип - указатель для Callback-функции
typedef int8_t clkHandle;          // тип - идентификатор задачи
static const clkHandle CLK_INVALID_HANDLE = -1;

struct clkTask // структура, описывающая задачу
{
  bool status;            // статус задачи
  unsigned long timer;    // таймер задачи
  unsigned long interval; // интервал срабатывания задачи
  clkCallback callback;   // функция, вызываемая при срабатывании таймера задачи
};

class clkTaskManager
{
private:
  uint8_t TASKCOUNT = 0;
  clkTask *taskList = NULL;

  bool isValidHandle(clkHandle _handle);

public:
  clkHandle rtc_guard;              // опрос микросхемы RTC по таймеру, чтобы не дергать ее откуда попало
  clkHandle blink_timer;            // блинк
  clkHandle return_to_default_mode; // таймер автовозврата в режим показа времени из любого режима настройки
  clkHandle set_time_mode;          // режим настройки времени
  clkHandle display_guard;          // вывод данных на экран
#if defined(USE_ALARM)
  clkHandle alarm_guard;  // отслеживание будильника
  clkHandle alarm_buzzer; // пищалка будильника
#endif
#if __USE_AUTO_SHOW_DATA__
  clkHandle auto_show_mode; // вывод даты и/или температуры
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

  void init(uint8_t _taskCount);

  void tick();

  clkHandle addTask(unsigned long _interval, clkCallback _callback, bool isActive = true);

  void startTask(clkHandle _handle);

  void stopTask(clkHandle _handle);

  bool getTaskState(clkHandle _handle);

  void setTaskInterval(clkHandle _handle, unsigned long _interval, bool _restart = true);
};

// ---- clkTaskManager private ------------------

bool clkTaskManager::isValidHandle(clkHandle _handle)
{
  return (_handle > CLK_INVALID_HANDLE && _handle < TASKCOUNT);
}
// ---- clkTaskManager public -------------------

clkTaskManager::clkTaskManager() {}

void clkTaskManager::init(uint8_t _taskCount)
{
  TASKCOUNT = (_taskCount) ? _taskCount : 1;
  taskList = (clkTask *)calloc(TASKCOUNT, sizeof(clkTask));
  if (taskList == NULL)
  {
    TASKCOUNT = 0;
  }
}

void clkTaskManager::tick()
{
  for (uint8_t i = 0; i < TASKCOUNT; i++)
  {
   unsigned long now = millis();
    if (taskList[i].status && taskList[i].callback != NULL)
    {
      if (now - taskList[i].timer >= taskList[i].interval)
      {
        taskList[i].timer += taskList[i].interval;
        taskList[i].callback();
      }
    }
  }
}

clkHandle clkTaskManager::addTask(unsigned long _interval, clkCallback _callback, bool isActive)
{
  for (uint8_t i = 0; i < TASKCOUNT; i++)
  {
    if (taskList[i].callback == NULL)
    {
      taskList[i].status = isActive;
      taskList[i].interval = _interval;
      taskList[i].callback = _callback;
      taskList[i].timer = millis();
      return (i);
    }
  }
  return (CLK_INVALID_HANDLE);
}

void clkTaskManager::startTask(clkHandle _handle)
{
  if (isValidHandle(_handle) && taskList[_handle].callback != NULL)
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
  if (isValidHandle(_handle) && (taskList != NULL))
  {
    return (taskList[_handle].status && taskList[_handle].callback != NULL);
  }

  return (false);
}

void clkTaskManager::setTaskInterval(clkHandle _handle, unsigned long _interval, bool _restart)
{
  if (isValidHandle(_handle))
  {
    taskList[_handle].interval = _interval;
    if (_restart && (taskList[_handle].callback != NULL))
    {
      taskList[_handle].status = true;
      taskList[_handle].timer = millis();
    }
  }
}

// ==== end clkTaskManager ===========================

clkTaskManager clkTasks;
