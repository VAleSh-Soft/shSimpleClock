/**
 * @file clkTaskManager.h
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 *
 * @brief диспетчер задач; 
 *        полная версия здесь - https://github.com/VAleSh-Soft/shTaskManager
 *
 * @version 1.0
 * @date 11.03.2024
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
  bool status;          // статус задачи
  uint32_t timer;       // таймер задачи
  uint32_t interval;    // интервал срабатывания задачи
  clkCallback callback; // функция, вызываемая при срабатывании таймера задачи
};

class clkTaskManager
{
private:
  uint8_t TASKCOUNT = 0;
  clkTask *taskList = NULL;

  bool isValidHandle(clkHandle _handle)
  {
    return (_handle > CLK_INVALID_HANDLE && _handle < TASKCOUNT);
  }

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
#if defined(USE_LIGHT_SENSOR)
  clkHandle light_sensor_guard; // отслеживание показаний датчика света
#endif
#if __USE_OTHER_SETTING__
  clkHandle other_setting_mode; // режим настроек цифровых данных
#endif
#if defined(USE_TICKER_FOR_DATA)
  clkHandle ticker; // отработка бегущей строки
#endif

  clkTaskManager() {}

  void init(uint8_t _taskCount)
  {
    TASKCOUNT = (_taskCount) ? _taskCount : 1;
    taskList = (clkTask *)calloc(TASKCOUNT, sizeof(clkTask));
    if (taskList == NULL)
    {
      TASKCOUNT = 0;
    }
  }

  void tick()
  {
    for (uint8_t i = 0; i < TASKCOUNT; i++)
    {
      if (taskList[i].status && taskList[i].callback != NULL)
      {
        if (millis() - taskList[i].timer >= taskList[i].interval)
        {
          taskList[i].timer += taskList[i].interval;
          taskList[i].callback();
        }
      }
    }
  }

  clkHandle addTask(uint32_t _interval, clkCallback _callback, bool isActive = true)
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

  void startTask(clkHandle _handle)
  {
    if (isValidHandle(_handle) && taskList[_handle].callback != NULL)
    {
      taskList[_handle].status = true;
      taskList[_handle].timer = millis();
    }
  }

  void stopTask(clkHandle _handle)
  {
    if (isValidHandle(_handle))
    {
      taskList[_handle].status = false;
    }
  }

  bool getTaskState(clkHandle _handle)
  {
    if (isValidHandle(_handle) && (taskList != NULL))
    {
      return (taskList[_handle].status && taskList[_handle].callback != NULL);
    }
    
    return (false);
  }

  void setTaskInterval(clkHandle _handle, uint32_t _interval, bool _restart = true)
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
};

// ==== end clkTaskManager ===========================

clkTaskManager clkTasks;

