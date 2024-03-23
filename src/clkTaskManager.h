/**
 * @file clkTaskManager.h
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 *
 * @brief диспетчер задач; полная версия здесь - https://github.com/VAleSh-Soft/shTaskManager
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
typedef int16_t clkHandle;         // тип - идентификатор задачи
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

  clkHandle clk_rtc_guard;              // опрос микросхемы RTC по таймеру, чтобы не дергать ее откуда попало
  clkHandle clk_blink_timer;            // блинк
  clkHandle clk_return_to_default_mode; // таймер автовозврата в режим показа времени из любого режима настройки
  clkHandle clk_set_time_mode;          // режим настройки времени
  clkHandle clk_display_guard;          // вывод данных на экран
#if defined(USE_ALARM)
  clkHandle clk_alarm_guard;  // отслеживание будильника
  clkHandle clk_alarm_buzzer; // пищалка будильника
#endif
#if USE_AUTO_SHOW_DATA
  clkHandle clk_auto_show_mode; // вывод даты и/или температуры
#endif
#if defined(USE_TEMP_DATA) && defined(USE_DS18B20)
  clkHandle clk_ds18b20_guard; // опрос датчика DS18b20
#endif
#if defined(USE_LIGHT_SENSOR)
  clkHandle clk_light_sensor_guard; // отслеживание показаний датчика света
#endif
#if USE_OTHER_SETTING
  clkHandle clk_other_setting_mode; // режим настроек цифровых данных
#endif
#if defined(USE_TICKER_FOR_DATA)
  clkHandle clk_ticker; // отработка бегущей строки
#endif

public:
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
    int16_t result = CLK_INVALID_HANDLE;
    for (uint8_t i = 0; i < TASKCOUNT; i++)
    {
      if (taskList[i].callback == NULL)
      {
        taskList[i].status = isActive;
        taskList[i].interval = _interval;
        taskList[i].callback = _callback;
        taskList[i].timer = millis();
        result = i;
        break;
      }
    }
    return (result);
  }

  void startTask(clkHandle _handle)
  {
    if (isValidHandle(_handle))
    {
      if (taskList[_handle].callback != NULL)
      {
        taskList[_handle].status = true;
        taskList[_handle].timer = millis();
      }
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
    bool result = isValidHandle(_handle);
    if (result && (taskList != NULL))
    {
      result = taskList[_handle].status && taskList[_handle].callback != NULL;
    }
    return (result);
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

  clkHandle rtc_guard() { return clk_rtc_guard; }
  void rtc_guard(clkHandle _idx) { clk_rtc_guard = _idx; }

  clkHandle blink_timer() { return clk_blink_timer; }
  void blink_timer(clkHandle _idx) { clk_blink_timer = _idx; }

  clkHandle return_to_default_mode() { return clk_return_to_default_mode; }
  void return_to_default_mode(clkHandle _idx) { clk_return_to_default_mode = _idx; }

  clkHandle set_time_mode() { return clk_set_time_mode; }
  void set_time_mode(clkHandle _idx) { clk_set_time_mode = _idx; }

  clkHandle display_guard() { return clk_display_guard; }
  void display_guard(clkHandle _idx) { clk_display_guard = _idx; }

#if defined(USE_ALARM)
  clkHandle alarm_guard() { return clk_alarm_guard; }
  void alarm_guard(clkHandle _idx) { clk_alarm_guard = _idx; }

  clkHandle alarm_buzzer() { return clk_alarm_buzzer; }
  void alarm_buzzer(clkHandle _idx) { clk_alarm_buzzer = _idx; }
#endif

#if USE_AUTO_SHOW_DATA
  clkHandle auto_show_mode() { return clk_auto_show_mode; }
  void auto_show_mode(clkHandle _idx) { clk_auto_show_mode = _idx; }
#endif

#if defined(USE_TEMP_DATA) && defined(USE_DS18B20)
  clkHandle ds18b20_guard() { return clk_ds18b20_guard; }
  void ds18b20_guard(clkHandle _idx) { clk_ds18b20_guard = _idx; }
#endif

#if defined(USE_LIGHT_SENSOR)
  clkHandle light_sensor_guard() { return clk_light_sensor_guard; }
  void light_sensor_guard(clkHandle _idx) { clk_light_sensor_guard = _idx; }
#endif

#if USE_OTHER_SETTING
  clkHandle other_setting_mode() { return clk_other_setting_mode; }
  void other_setting_mode(clkHandle _idx) { clk_other_setting_mode = _idx; }
#endif

#if defined(USE_TICKER_FOR_DATA)
  clkHandle ticker() { return clk_ticker; }
  void ticker(clkHandle _idx) { clk_ticker = _idx; }
#endif
};

// ==== end clkTaskManager ===========================
