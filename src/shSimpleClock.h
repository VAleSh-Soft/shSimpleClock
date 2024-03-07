#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include <shButton.h> // https://github.com/VAleSh-Soft/shButton
#include "shSimpleRTC.h"

// ===================================================

void sscRtcNow();
void sscBlink();
void sscReturnToDefMode();
void sscShowTimeSetting();
void sscSetDisp();
void sscCheckButton();
void sscSetDisplay();
#ifdef USE_CALENDAR
void sscShowCalendar();
#endif
#ifdef USE_ALARM
void sscCheckAlarm();
void sscRunAlarmBuzzer();
void sscShowAlarmState(uint8_t _state);
#endif
#ifdef USE_LIGHT_SENSOR
void sscSetBrightness();
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
void sscShowBrightnessSetting();
#endif
#ifdef USE_TEMP_DATA
void sscShowTemp();
#ifdef USE_DS18B20
void sscCheckDS18b20();
#endif
#endif

// ===================================================

#if defined(TM1637_DISPLAY)
#include "display_TM1637.h"
#elif defined(MAX72XX_7SEGMENT_DISPLAY) || defined(MAX72XX_MATRIX_DISPLAY)
#include "display_MAX72xx.h"
#elif defined(WS2812_MATRIX_DISPLAY)
#include <FastLED.h> // https://github.com/FastLED/FastLED
#include "display_WS2812.h"
#endif
#ifdef USE_ALARM
#include "alarm.h"
#endif
#ifdef USE_TEMP_DATA
#if defined(USE_DS18B20)
#include "ds1820.h"
#elif defined(USE_NTC)
#include "ntc.h"
#endif
#endif

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
    if (taskList[_handle].callback != NULL)
    {
      taskList[_handle].status = true;
      taskList[_handle].timer = millis();
    }
  }

  void stopTask(clkHandle _handle)
  {
    taskList[_handle].status = false;
  }

  bool getTaskState(clkHandle _handle)
  {
    bool result = false;
    if (taskList != NULL)
    {
      result = taskList[_handle].status && taskList[_handle].callback != NULL;
    }
    return (result);
  }

  void setTaskInterval(clkHandle _handle, uint32_t _interval, bool _restart = true)
  {
    taskList[_handle].interval = _interval;
    if (_restart && (taskList[_handle].callback != NULL))
    {
      taskList[_handle].status = true;
      taskList[_handle].timer = millis();
    }
  }
};

// ==== end clkTaskManager ===========================

clkTaskManager sscTasks;

clkHandle ssc_rtc_guard;              // опрос микросхемы RTC по таймеру, чтобы не дергать ее откуда попало
clkHandle ssc_blink_timer;            // блинк
clkHandle ssc_return_to_default_mode; // таймер автовозврата в режим показа времени из любого режима настройки
clkHandle ssc_set_time_mode;          // режим настройки времени
clkHandle ssc_display_guard;          // вывод данных на экран
#ifdef USE_ALARM
clkHandle ssc_alarm_guard;  // отслеживание будильника
clkHandle ssc_alarm_buzzer; // пищалка будильника
#endif
#ifdef USE_CALENDAR
clkHandle ssc_show_calendar_mode; // режим вывода даты
#endif
#ifdef USE_TEMP_DATA
clkHandle ssc_show_temp_mode; // режим показа температуры
#if defined(USE_DS18B20)
clkHandle ssc_ds18b20_guard; // опрос датчика DS18b20
#endif
#endif
#ifdef USE_LIGHT_SENSOR
clkHandle ssc_light_sensor_guard; // отслеживание показаний датчика света
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
clkHandle ssc_set_brightness_mode; // режим настройки яркости экрана
#endif

// -----------------------------------------
bool sscBlinkFlag = false; // флаг блинка, используется всем, что должно мигать

// ---- экраны -----------------------------
#if defined(TM1637_DISPLAY)
DisplayTM1637 sscDisp(DISPLAY_CLK_PIN, DISPLAY_DAT_PIN);
#elif defined(MAX72XX_7SEGMENT_DISPLAY)
DisplayMAX72xx7segment<DISPLAY_CS_PIN> sscDisp;
#elif defined(MAX72XX_MATRIX_DISPLAY)
DisplayMAX72xxMatrix<DISPLAY_CS_PIN> sscDisp;
#elif defined(WS2812_MATRIX_DISPLAY)
CRGB ssc_leds[256];
DisplayWS2812Matrix sscDisp(ssc_leds, COLOR_OF_NUMBER, MX_TYPE);
#endif

// ---- модуль RTC -------------------------
shSimpleRTC sscClock;

// ---- будильник --------------------------
#ifdef USE_ALARM
Alarm sscAlarm(ALARM_LED_PIN, ALARM_EEPROM_INDEX);
#endif

// ---- датчики температуры ----------------
#ifdef USE_TEMP_DATA
#if defined(USE_DS18B20)
DS1820 sscTempSensor(DS18B20_PIN);
#elif defined(USE_NTC)
NTCSensor sscTempSensor(NTC_PIN, _resistor_std, _balance_resistor, _beta);
#endif
#endif

// ==== clkButton ====================================
enum clkButtonFlag : uint8_t
{
  BTN_FLAG_NONE, // флаг кнопки - ничего не делать
  BTN_FLAG_NEXT, // флаг кнопки - изменить значение
  BTN_FLAG_EXIT  // флаг кнопки - возврат в режим показа текущего времени
};

class clkButton : public shButton
{
private:
  clkButtonFlag _flag = BTN_FLAG_NONE;

public:
#ifdef BTN_PULL_UP
  clkButton(uint8_t button_pin) : shButton(button_pin)
#else
  clkButton(uint8_t button_pin) : shButton(button_pin, PULL_DOWN)
#endif
  {
    shButton::setTimeoutOfLongClick(TIMEOUT_OF_LONGCLICK);
    shButton::setLongClickMode(LCM_ONLYONCE);
    shButton::setVirtualClickOn(true);
    shButton::setTimeoutOfDebounce(TIMEOUT_OF_DEBOUNCE);
  }

  clkButtonFlag getBtnFlag()
  {
    return (_flag);
  }

  void setBtnFlag(clkButtonFlag flag)
  {
    _flag = flag;
  }

  uint8_t getButtonState()
  {
    uint8_t _state = shButton::getButtonState();
    switch (_state)
    {
    case BTN_DOWN:
    case BTN_DBLCLICK:
    case BTN_LONGCLICK:
      // в любом режиме, кроме стандартного, каждый клик любой кнопки перезапускает таймер автовыхода в стандартный режим
      if (sscTasks.getTaskState(ssc_return_to_default_mode))
      {
        sscTasks.startTask(ssc_return_to_default_mode);
      }
#ifdef USE_ALARM
      // если сработал будильник, отключить его
      if (sscAlarm.getAlarmState() == ALARM_YES)
      {
        sscAlarm.setAlarmState(ALARM_ON);
        shButton::resetButtonState();
      }
#endif
      break;
    }
    return (_state);
  }
};

// ==== end clkButton=================================

enum clkDisplayMode : uint8_t
{
  DISPLAY_MODE_SHOW_TIME, // основной режим - вывод времени на индикатор
  DISPLAY_MODE_SET_HOUR,  // режим настройки часов
  DISPLAY_MODE_SET_MINUTE // режим настройки минут
#ifdef USE_CALENDAR
  ,
  DISPLAY_MODE_SET_DAY,   // режим настройки дня месяца
  DISPLAY_MODE_SET_MONTH, // режим настройки месяца
  DISPLAY_MODE_SET_YEAR   // режим настройки года
#endif
#ifdef USE_ALARM
  ,
  DISPLAY_MODE_ALARM_ON_OFF,    // режим настройки будильника - вкл/выкл
  DISPLAY_MODE_SET_ALARM_HOUR,  // режим настройки будильника - часы
  DISPLAY_MODE_SET_ALARM_MINUTE // режим настройки будильника - минуты
#endif
#ifdef USE_TEMP_DATA
  ,
  DISPLAY_MODE_SHOW_TEMP // режим вывода температуры
#endif
#ifdef USE_CALENDAR
  ,
  DISPLAY_MODE_SHOW_CALENDAR // режим вывода даты
#endif
#ifdef USE_LIGHT_SENSOR
  ,
  DISPLAY_MODE_SET_LIGHT_THRESHOLD // режим настройки порога переключения яркости
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
#ifdef USE_LIGHT_SENSOR
  ,
  DISPLAY_MODE_SET_BRIGHTNESS_MIN // режим настройки минимального уровня яркости экрана
#endif
  ,
  DISPLAY_MODE_SET_BRIGHTNESS_MAX // режим настройки максимального уровня яркости экрана
#endif
};

clkDisplayMode ssc_display_mode = DISPLAY_MODE_SHOW_TIME;

clkButton sscBtnSet = NULL;
clkButton sscBtnUp = NULL;
clkButton sscBtnDown = NULL;

// ==== shSimpleClock ================================

class shSimpleClock
{
private:
public:
  /**
   * @brief конструктор объекта часов
   *
   */
  shSimpleClock() {}

  /**
   * @brief инициализация часов
   *
   */
  void init()
  {
    // ==== RTC ======================================
    Wire.begin();
    sscRtcNow();

    // ==== кнопки ===================================
    if (BTN_SET_PIN >= 0)
    {
      sscBtnSet = (clkButton){BTN_SET_PIN};
    }

    if (BTN_UP_PIN >= 0)
    {
      sscBtnUp = (clkButton){BTN_UP_PIN};
      sscBtnUp.setLongClickMode(LCM_CLICKSERIES);
      sscBtnUp.setIntervalOfSerial(INTERVAL_OF_SERIAL);
    }

    if (BTN_DOWN_PIN >= 0)
    {
      sscBtnDown = (clkButton){BTN_DOWN_PIN};
      sscBtnDown.setLongClickMode(LCM_CLICKSERIES);
      sscBtnDown.setIntervalOfSerial(100);
    }

// ==== датчики ====================================
#if defined(USE_NTC)
    sscTempSensor.setADCbitDepth(_bit_depth); // установить разрядность АЦП вашего МК, для AVR обычно равна 10 бит
#endif
    // проверить корректность заданных уровней яркости
    uint8_t x = EEPROM.read(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX);
#if defined(MAX72XX_7SEGMENT_DISPLAY) || defined(MAX72XX_MATRIX_DISPLAY)
    x = (x > 15) ? 8 : x;
#elif defined(WS2812_MATRIX_DISPLAY)
    x = ((x > 25) || (x == 0)) ? 15 : x;
#else
    x = ((x > 7) || (x == 0)) ? 7 : x;
#endif
    EEPROM.update(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX, x);
#ifdef USE_LIGHT_SENSOR
    uint8_t y = EEPROM.read(LIGHT_THRESHOLD_EEPROM_INDEX);
    y = ((y > 9) || (y == 0)) ? 3 : y;
    EEPROM.update(LIGHT_THRESHOLD_EEPROM_INDEX, y);

    x = EEPROM.read(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX);
#if defined(MAX72XX_7SEGMENT_DISPLAY) || defined(MAX72XX_MATRIX_DISPLAY)
    x = (x > 15) ? 0 : x;
#elif defined(WS2812_MATRIX_DISPLAY)
    x = ((x > 25) || (x == 0)) ? 1 : x;
#else
    x = ((x > 7) || (x == 0)) ? 1 : x;
#endif
    EEPROM.update(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX, x);
#else
    sscDisp.setBrightness(EEPROM.read(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX));
#endif

// ==== экраны =======================================
#if defined(WS2812_MATRIX_DISPLAY)
    setFastLEDData(ssc_leds, 256);
#elif defined(MAX72XX_MATRIX_DISPLAY) || defined(MAX72XX_7SEGMENT_DISPLAY)
    sscDisp.shutdownAllDevices(false);
#ifdef MAX72XX_MATRIX_DISPLAY
    sscDisp.setDirection(2);
    sscDisp.setFlip(false);
#endif
#endif

// выставить яркость в минимум, чтобы при включении не сверкало максимальной яркостью
#if defined(WS2812_MATRIX_DISPLAY)
    FastLED.setBrightness(0);
#elif defined(MAX72XX_MATRIX_DISPLAY) || defined(MAX72XX_7SEGMENT_DISPLAY)
    sscDisp.setBrightness(0);
#else
    sscDisp.setBrightness(1);
#endif

    // ==== задачи ===================================
    uint8_t task_count = 5; // базовое количество задач
#ifdef USE_LIGHT_SENSOR
    task_count++;
#endif
#ifdef USE_TEMP_DATA
    task_count++;
#if defined(USE_DS18B20)
    task_count++;
#endif
#endif
#ifdef USE_CALENDAR
    task_count++;
#endif
#ifdef USE_ALARM
    task_count += 2;
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
    task_count++;
#endif
    sscTasks.init(task_count);

    ssc_rtc_guard = sscTasks.addTask(50ul, sscRtcNow);
    ssc_blink_timer = sscTasks.addTask(50ul, sscBlink);
    ssc_return_to_default_mode = sscTasks.addTask(AUTO_EXIT_TIMEOUT * 1000ul, sscReturnToDefMode, false);
    ssc_set_time_mode = sscTasks.addTask(100ul, sscShowTimeSetting, false);
#ifdef USE_TEMP_DATA
    ssc_show_temp_mode = sscTasks.addTask(500ul, sscShowTemp, false);
#if defined(USE_DS18B20)
    ssc_ds18b20_guard = sscTasks.addTask(3000ul, sscCheckDS18b20);
#endif
#endif
#ifdef USE_CALENDAR
#if (defined(WS2812_MATRIX_DISPLAY) || defined(MAX72XX_MATRIX_DISPLAY)) && defined(USE_TICKER_FOR_DATE)
    uint32_t t = 1000ul / TICKER_SPEED;
#else
    uint32_t t = 1000ul;
#endif
    ssc_show_calendar_mode = sscTasks.addTask(t, sscShowCalendar, false);
#endif
#ifdef USE_ALARM
    ssc_alarm_guard = sscTasks.addTask(200ul, sscCheckAlarm);
    ssc_alarm_buzzer = sscTasks.addTask(50ul, sscRunAlarmBuzzer, false);
#endif
    ssc_display_guard = sscTasks.addTask(50ul, sscSetDisp);
#if defined(USE_LIGHT_SENSOR)
    ssc_light_sensor_guard = sscTasks.addTask(100ul, sscSetBrightness);
#elif defined(WS2812_MATRIX_DISPLAY)
    FastLED.setBrightness(EEPROM.read(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX) * 10);
#else
    sscDisp.setBrightness(EEPROM.read(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX));
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
    ssc_set_brightness_mode = sscTasks.addTask(100ul, sscShowBrightnessSetting, false);
#endif
  }

  /**
   * @brief обработка событий часов
   *
   */
  void tick()
  {
    sscCheckButton();
    sscTasks.tick();
    sscSetDisplay();
  }

  /**
   * @brief получить текущий статус или событие кнопки Set
   *
   * @return uint8_t
   */
  uint8_t getBtnSetState()
  {
    uint8_t result = 0;
    if (&sscBtnSet != NULL)
    {
      result = sscBtnSet.getLastState();
    }
    return result;
  }

  /**
   * @brief получить текущий статус или событие кнопки Up
   *
   * @return uint8_t
   */
  uint8_t getBtnUpState()
  {
    uint8_t result = 0;
    if (&sscBtnUp != NULL)
    {
      result = sscBtnUp.getLastState();
    }
    return result;
  }

  /**
   * @brief получить текущий статус или событие кнопки Down
   *
   * @return uint8_t
   */
  uint8_t getBtnDownState()
  {
    uint8_t result = 0;
    if (&sscBtnDown != NULL)
    {
      result = sscBtnDown.getLastState();
    }
    return result;
  }

#ifdef MAX72XX_MATRIX_DISPLAY
  /**
   * @brief установить угол поворота изображения
   *
   * @param _dir угол поворота изображения, 0..3
   */
  void setMatrixDirection(uint8_t _dir)
  {
    sscDisp.setDirection(_dir);
  }

  /**
   * @brief включить отражение изображения по горизонтали (по строкам)
   *
   * @param _mode true - включить отражение, false - отключить отражение
   */
  void setMatrixFlipMode(bool _mode)
  {
    sscDisp.setFlip(_mode);
  }
#endif

#ifdef WS2812_MATRIX_DISPLAY
  /**
   * @brief установить цвет для вывода информации на матрице из адресных светодиодов
   *
   * @param _color цвет, например, для красного CRGB::Red
   */
  void setColorOfNumber(CRGB _color)
  {
    sscDisp.setColorOfNumber(_color);
  }
#endif

  /**
   * @brief получение текущих даты и времени
   *
   * @return DateTime
   */
  DateTime getCurrentDateTime()
  {
    return (sscClock.getCurTime());
  }

  /**
   * @brief установка текущего времени
   *
   * @param _hour часы для установки
   * @param _minute минуты для установки
   * @param _second секунды для установки
   */
  void setCurrentTime(uint8_t _hour, uint8_t _minute, uint8_t _second)
  {
    sscClock.setCurTime(_hour, _minute, _second);
  }

#ifdef USE_CALENDAR
  /**
   * @brief установка текущей даты
   *
   * @param _date число месяца для установки
   * @param _month номер месяца для установки (1..2)
   */
  void setCurrentDate(uint8_t _date, uint8_t _month)
  {
    sscClock.setCurDate(_date, _month);
  }

  /**
   * @brief установка текущего года
   *
   * @param _year год для установки (0..99 )
   */
  void setCurrentYear(uint8_t _year)
  {
    sscClock.setCurYear(_year);
  }
#endif

#ifdef USE_TEMP_DATA
  /**
   * @brief получить текущую температуру, если определен используемый датчик
   *
   * @return int8_t
   */
  int8_t getTemperature()
  {
    int8_t result = -127;
#if defined(USE_DS18B20) || defined(USE_NTC)
    result = sscTempSensor.getTemp();
#elif defined(RTC_DS3231)
    result = sscClock.getTemperature();
#endif

    return result;
  }
#endif

#ifdef USE_ALARM
    /**
     * @brief получение времени срабатывания будильника
     * 
     * @return uint16_t количество минут с полуночи
     */
  uint16_t getAlarmPoint() { return (sscAlarm.getAlarmPoint()); }

  /**
   * @brief установка времени срабатывания будильника
   * 
   * @param _point количество минут с полуночи
   */
  void setAlarmPoint(uint16_t _point) { sscAlarm.setAlarmPoint(_point); }

  /**
   * @brief установка времени срабатывания будильника
   * 
   * @param _hour час
   * @param _minute минута
   */
  void setAlarmPoint(uint8_t _hour, uint8_t _minute) { sscAlarm.setAlarmPoint(_hour * 60 + _minute); }

  /**
   * @brief получение состояния будильника - включен или выключен
   * 
   * @return true - включен;
   * @return false - выключен
   */
  bool getOnOffAlarm() { return (sscAlarm.getOnOffAlarm()); }

  /**
   * @brief установка состояния будильника - включить или выключить
   * 
   * @param _state true - включен, false - выключен
   */
  void setOnOffAlarm(bool _state) { sscAlarm.setOnOffAlarm(_state); }

  /**
   * @brief получение статуса будильника, позволяет отслеживать срабатывание будильника
   * 
   * @return AlarmState 0 - будильник выключен, 1 - будильник включен, 2 - будильник сработал
   */
  AlarmState getAlarmState() { return (sscAlarm.getAlarmState()); }
#endif
};

// ==== end shSimpleClock ============================

void sscRtcNow()
{
  sscClock.now();
  if (ssc_display_mode == DISPLAY_MODE_SHOW_TIME)
  {
#if defined(MAX72XX_MATRIX_DISPLAY) || defined(WS2812_MATRIX_DISPLAY)
    sscDisp.showTime(sscClock.getCurTime().hour(), sscClock.getCurTime().minute(), sscClock.getCurTime().second(), sscBlinkFlag);
#else
    sscDisp.showTime(sscClock.getCurTime().hour(), sscClock.getCurTime().minute(), sscBlinkFlag);
#endif
  }
}

void sscBlink()
{
  static uint8_t cur_sec = sscClock.getCurTime().second();
  static uint32_t tmr = 0;
  if (cur_sec != sscClock.getCurTime().second())
  {
    cur_sec = sscClock.getCurTime().second();
    sscBlinkFlag = true;
    tmr = millis();
  }
  else if (sscBlinkFlag && millis() - tmr >= 500)
  {
    sscBlinkFlag = false;
  }
}

void sscReturnToDefMode()
{
  switch (ssc_display_mode)
  {
  case DISPLAY_MODE_SET_HOUR:
  case DISPLAY_MODE_SET_MINUTE:
#ifdef USE_CALENDAR
  case DISPLAY_MODE_SET_DAY:
  case DISPLAY_MODE_SET_MONTH:
  case DISPLAY_MODE_SET_YEAR:
#endif
#ifdef USE_ALARM
  case DISPLAY_MODE_SET_ALARM_HOUR:
  case DISPLAY_MODE_SET_ALARM_MINUTE:
  case DISPLAY_MODE_ALARM_ON_OFF:
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
#ifdef USE_LIGHT_SENSOR
  case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
#endif
    sscBtnSet.setBtnFlag(BTN_FLAG_EXIT);
    break;
#ifdef USE_TEMP_DATA
  case DISPLAY_MODE_SHOW_TEMP:
    ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
    sscTasks.stopTask(ssc_show_temp_mode);
    break;
#endif
#ifdef USE_CALENDAR
  case DISPLAY_MODE_SHOW_CALENDAR:
    ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
    sscTasks.stopTask(ssc_show_calendar_mode);
    break;
#endif
  default:
    break;
  }
  sscTasks.stopTask(ssc_return_to_default_mode);
}

void sscShowTimeData(uint8_t hour, uint8_t minute)
{
  // если наступило время блинка и кнопки Up/Down не нажаты, то стереть соответствующие разряды; при нажатых кнопках Up/Down во время изменения данных ничего не мигает
  if (!sscBlinkFlag && !sscBtnUp.isButtonClosed() && !sscBtnDown.isButtonClosed())
  {
    switch (ssc_display_mode)
    {
    case DISPLAY_MODE_SET_HOUR:
#ifdef USE_CALENDAR
    case DISPLAY_MODE_SET_DAY:
#endif
#ifdef USE_ALARM
    case DISPLAY_MODE_SET_ALARM_HOUR:
#endif
      hour = -1;
      break;
    case DISPLAY_MODE_SET_MINUTE:
#ifdef USE_CALENDAR
    case DISPLAY_MODE_SET_MONTH:
    case DISPLAY_MODE_SET_YEAR:
#endif
#ifdef USE_ALARM
    case DISPLAY_MODE_SET_ALARM_MINUTE:
#endif
      minute = -1;
      break;
    default:
      break;
    }
  }

  bool toDate = false;
  bool toColon = false;
#if defined(USE_CALENDAR)
  toDate = (ssc_display_mode >= DISPLAY_MODE_SET_DAY && ssc_display_mode <= DISPLAY_MODE_SET_YEAR);
  toColon = ssc_display_mode != DISPLAY_MODE_SET_YEAR;
#endif
#if defined(MAX72XX_MATRIX_DISPLAY) || defined(WS2812_MATRIX_DISPLAY)
  sscDisp.showTime(hour, minute, 0, toColon, toDate);
#else
  sscDisp.showTime(hour, minute, toColon);
#endif
}

void sscCheckData(uint8_t &dt, uint8_t max, bool toUp, uint8_t min = 0, bool toLoop = true)
{
  (toUp) ? dt++ : dt--;
  if ((dt > max) || (min > 0 && dt < min))
  {
    if (toLoop)
    {
      dt = (toUp) ? min : max;
    }
    else
    {
      dt = (toUp) ? max : min;
    }
  }
}

void sscStopSetting(clkHandle task)
{
  sscTasks.stopTask(task);
  sscTasks.stopTask(ssc_return_to_default_mode);
}

void sscShowTimeSetting()
{
  static bool time_checked = false;
  static uint8_t curHour = 0;
  static uint8_t curMinute = 0;
#ifdef USE_CALENDAR
  static const uint8_t PROGMEM days_of_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
#endif

  if (!sscTasks.getTaskState(ssc_set_time_mode))
  {
    sscTasks.startTask(ssc_set_time_mode);
    sscTasks.startTask(ssc_return_to_default_mode);
    switch (ssc_display_mode)
    {
#ifdef USE_ALARM
    case DISPLAY_MODE_SET_ALARM_HOUR:
    case DISPLAY_MODE_SET_ALARM_MINUTE:
      curHour = sscAlarm.getAlarmPoint() / 60;
      curMinute = sscAlarm.getAlarmPoint() % 60;
      break;
    case DISPLAY_MODE_ALARM_ON_OFF:
      curHour = (uint8_t)sscAlarm.getOnOffAlarm();
      break;
#endif
    default:
      break;
    }
    time_checked = false;
  }

  if (!time_checked)
  {
    switch (ssc_display_mode)
    {
    case DISPLAY_MODE_SET_HOUR:
    case DISPLAY_MODE_SET_MINUTE:
      curHour = sscClock.getCurTime().hour();
      curMinute = sscClock.getCurTime().minute();
      break;
#ifdef USE_CALENDAR
    case DISPLAY_MODE_SET_DAY:
    case DISPLAY_MODE_SET_MONTH:
      curHour = sscClock.getCurTime().day();
      curMinute = sscClock.getCurTime().month();
      break;
    case DISPLAY_MODE_SET_YEAR:
      curHour = 20;
      curMinute = sscClock.getCurTime().year() % 100;
      break;
#endif
    default:
      break;
    }
  }

  // опрос кнопок =====================
  if (sscBtnSet.getBtnFlag() > BTN_FLAG_NONE)
  {
    if (time_checked)
    {
      switch (ssc_display_mode)
      {
      case DISPLAY_MODE_SET_HOUR:
      case DISPLAY_MODE_SET_MINUTE:
        sscClock.setCurTime(curHour, curMinute, 0);
        sscRtcNow();
        break;
#ifdef USE_CALENDAR
      case DISPLAY_MODE_SET_DAY:
      case DISPLAY_MODE_SET_MONTH:
        sscClock.setCurDate(curHour, curMinute);
        sscRtcNow();
        break;
      case DISPLAY_MODE_SET_YEAR:
        sscClock.setCurYear(curMinute);
        sscRtcNow();
        break;
#endif
#ifdef USE_ALARM
      case DISPLAY_MODE_SET_ALARM_HOUR:
      case DISPLAY_MODE_SET_ALARM_MINUTE:
        sscAlarm.setAlarmPoint(curHour * 60 + curMinute);
        break;
      case DISPLAY_MODE_ALARM_ON_OFF:
        sscAlarm.setOnOffAlarm((bool)curHour);
        break;
#endif
      default:
        break;
      }
      time_checked = false;
    }
    if (sscBtnSet.getBtnFlag() == BTN_FLAG_NEXT)
    {
      switch (ssc_display_mode)
      {
      case DISPLAY_MODE_SET_HOUR:
#ifdef USE_CALENDAR
      case DISPLAY_MODE_SET_MINUTE:
      case DISPLAY_MODE_SET_DAY:
      case DISPLAY_MODE_SET_MONTH:
#endif
#ifdef USE_ALARM
      case DISPLAY_MODE_SET_ALARM_HOUR:
#endif
        ssc_display_mode = clkDisplayMode(uint8_t(ssc_display_mode + 1));
        sscStopSetting(ssc_set_time_mode);
        break;
#ifdef USE_ALARM
      case DISPLAY_MODE_ALARM_ON_OFF:
        ssc_display_mode = (curHour) ? DISPLAY_MODE_SET_ALARM_HOUR : DISPLAY_MODE_SHOW_TIME;
        sscStopSetting(ssc_set_time_mode);
        break;
#endif
      default:
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
        sscStopSetting(ssc_set_time_mode);
        break;
      }
    }
    else
    {
      ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
      sscStopSetting(ssc_set_time_mode);
    }
    sscBtnSet.setBtnFlag(BTN_FLAG_NONE);
  }

  if ((sscBtnUp.getBtnFlag() == BTN_FLAG_NEXT) || (sscBtnDown.getBtnFlag() == BTN_FLAG_NEXT))
  {
    bool dir = sscBtnUp.getBtnFlag() == BTN_FLAG_NEXT;
    switch (ssc_display_mode)
    {
    case DISPLAY_MODE_SET_HOUR:
#ifdef USE_ALARM
    case DISPLAY_MODE_SET_ALARM_HOUR:
#endif
      sscCheckData(curHour, 23, dir);
      break;
    case DISPLAY_MODE_SET_MINUTE:
#ifdef USE_ALARM
    case DISPLAY_MODE_SET_ALARM_MINUTE:
#endif
      sscCheckData(curMinute, 59, dir);
      break;
#ifdef USE_ALARM
    case DISPLAY_MODE_ALARM_ON_OFF:
      sscCheckData(curHour, 1, true);
      break;
#endif
#ifdef USE_CALENDAR
    case DISPLAY_MODE_SET_DAY:
      uint8_t i;
      i = pgm_read_byte(&days_of_month[curMinute - 1]);
      if (curMinute == 2 && (sscClock.getCurTime().year() % 4 == 0))
      {
        i++;
      }
      sscCheckData(curHour, i, dir, 1);
      break;
    case DISPLAY_MODE_SET_MONTH:
      sscCheckData(curMinute, 12, dir, 1);
      break;
    case DISPLAY_MODE_SET_YEAR:
      sscCheckData(curMinute, 99, dir);
      break;
#endif
    default:
      break;
    }
    time_checked = true;
    sscBtnUp.setBtnFlag(BTN_FLAG_NONE);
    sscBtnDown.setBtnFlag(BTN_FLAG_NONE);
  }

  // вывод данных на экран ============
  switch (ssc_display_mode)
  {
#ifdef USE_ALARM
  case DISPLAY_MODE_ALARM_ON_OFF:
    sscShowAlarmState(curHour);
    break;
#endif
  default:
    sscShowTimeData(curHour, curMinute);
    break;
  }
}

void sscSetDisp()
{
  sscDisp.show();
}

void sscCheckSetButton()
{
  if (&sscBtnSet == NULL)
  {
    return;
  }

  switch (sscBtnSet.getButtonState())
  {
  case BTN_ONECLICK:
    switch (ssc_display_mode)
    {
    case DISPLAY_MODE_SET_HOUR:
    case DISPLAY_MODE_SET_MINUTE:
#ifdef USE_CALENDAR
    case DISPLAY_MODE_SET_DAY:
    case DISPLAY_MODE_SET_MONTH:
    case DISPLAY_MODE_SET_YEAR:
#endif
#ifdef USE_ALARM
    case DISPLAY_MODE_SET_ALARM_HOUR:
    case DISPLAY_MODE_SET_ALARM_MINUTE:
    case DISPLAY_MODE_ALARM_ON_OFF:
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
#ifdef USE_LIGHT_SENSOR
    case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
    case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
#endif
      sscBtnSet.setBtnFlag(BTN_FLAG_NEXT);
      break;
    case DISPLAY_MODE_SHOW_TIME:
#ifdef USE_ALARM
#ifdef USE_ONECLICK_TO_SET_ALARM
      ssc_display_mode = DISPLAY_MODE_ALARM_ON_OFF;
#endif
#endif
      break;
    default:
      break;
    }
    break;
  case BTN_DBLCLICK:
    switch (ssc_display_mode)
    {
    case DISPLAY_MODE_SHOW_TIME:
#ifdef USE_ALARM
#ifndef USE_ONECLICK_TO_SET_ALARM
      ssc_display_mode = DISPLAY_MODE_ALARM_ON_OFF;
#endif
#endif
      break;
    default:
      break;
    }
    break;
  case BTN_LONGCLICK:
    switch (ssc_display_mode)
    {
    case DISPLAY_MODE_SHOW_TIME:
      ssc_display_mode = DISPLAY_MODE_SET_HOUR;
      break;
#ifdef USE_CALENDAR
    case DISPLAY_MODE_SHOW_CALENDAR:
      sscTasks.stopTask(ssc_show_calendar_mode);
      ssc_display_mode = DISPLAY_MODE_SET_DAY;
      break;
#endif
    case DISPLAY_MODE_SET_HOUR:
    case DISPLAY_MODE_SET_MINUTE:
#ifdef USE_CALENDAR
    case DISPLAY_MODE_SET_DAY:
    case DISPLAY_MODE_SET_MONTH:
    case DISPLAY_MODE_SET_YEAR:
#endif
#ifdef USE_ALARM
    case DISPLAY_MODE_SET_ALARM_HOUR:
    case DISPLAY_MODE_SET_ALARM_MINUTE:
    case DISPLAY_MODE_ALARM_ON_OFF:
#endif
      sscBtnSet.setBtnFlag(BTN_FLAG_EXIT);
      break;
    default:
      break;
    }
    break;
  }
}

void sscCheckUDbtn(clkButton &btn)
{
  switch (btn.getLastState())
  {
  case BTN_DOWN:
  case BTN_DBLCLICK:
    btn.setBtnFlag(BTN_FLAG_NEXT);
    break;
  case BTN_LONGCLICK:
#ifdef USE_ALARM
    if (ssc_display_mode != DISPLAY_MODE_ALARM_ON_OFF)
#endif
    {
      btn.setBtnFlag(BTN_FLAG_NEXT);
    }
    break;
  }
}

void sscCheckUpDownButton()
{
  if (&sscBtnUp != NULL)
  {
    sscBtnUp.getButtonState();
  }
  if (&sscBtnDown != NULL)
  {
    sscBtnDown.getButtonState();
  }

  switch (ssc_display_mode)
  {
  case DISPLAY_MODE_SHOW_TIME:
#ifdef USE_TEMP_DATA
    if (&sscBtnUp != NULL && sscBtnUp.getLastState() == BTN_ONECLICK)
    {
      ssc_display_mode = DISPLAY_MODE_SHOW_TEMP;
    }
#endif
#ifdef USE_CALENDAR
    if (&sscBtnDown != NULL && sscBtnDown.getLastState() == BTN_ONECLICK)
    {
      ssc_display_mode = DISPLAY_MODE_SHOW_CALENDAR;
    }
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
    if (&sscBtnUp != NULL && sscBtnUp.isSecondButtonPressed(sscBtnDown, BTN_LONGCLICK) ||
        &sscBtnDown != NULL && sscBtnDown.isSecondButtonPressed(sscBtnUp, BTN_LONGCLICK))
    {
#ifdef USE_LIGHT_SENSOR
      ssc_display_mode = DISPLAY_MODE_SET_BRIGHTNESS_MIN;
#else
      ssc_display_mode = DISPLAY_MODE_SET_BRIGHTNESS_MAX;
#endif
    }
#endif
    break;
  case DISPLAY_MODE_SET_HOUR:
  case DISPLAY_MODE_SET_MINUTE:
#ifdef USE_CALENDAR
  case DISPLAY_MODE_SET_DAY:
  case DISPLAY_MODE_SET_MONTH:
  case DISPLAY_MODE_SET_YEAR:
#endif
#ifdef USE_ALARM
  case DISPLAY_MODE_SET_ALARM_HOUR:
  case DISPLAY_MODE_SET_ALARM_MINUTE:
  case DISPLAY_MODE_ALARM_ON_OFF:
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
#ifdef USE_LIGHT_SENSOR
  case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
#endif
    if (&sscBtnUp != NULL && (&sscBtnDown != NULL && !sscBtnDown.isButtonClosed()))
    {
      sscCheckUDbtn(sscBtnUp);
    }
    if (&sscBtnDown != NULL && (&sscBtnUp != NULL && !sscBtnUp.isButtonClosed()))
    {
      sscCheckUDbtn(sscBtnDown);
    }
    break;
#ifdef USE_TEMP_DATA
  case DISPLAY_MODE_SHOW_TEMP:
    if (&sscBtnUp != NULL && sscBtnUp.getLastState() == BTN_ONECLICK)
    {
      sscReturnToDefMode();
    }
    break;
#endif
#ifdef USE_CALENDAR
  case DISPLAY_MODE_SHOW_CALENDAR:
    if (&sscBtnDown != NULL && sscBtnDown.getLastState() == BTN_ONECLICK)
    {
      sscReturnToDefMode();
    }
    break;
#endif
  default:
    break;
  }
}

void sscCheckButton()
{
  sscCheckSetButton();
  sscCheckUpDownButton();
}

void sscSetDisplay()
{
  switch (ssc_display_mode)
  {
  case DISPLAY_MODE_SET_HOUR:
  case DISPLAY_MODE_SET_MINUTE:
#ifdef USE_CALENDAR
  case DISPLAY_MODE_SET_DAY:
  case DISPLAY_MODE_SET_MONTH:
  case DISPLAY_MODE_SET_YEAR:
#endif
#ifdef USE_ALARM
  case DISPLAY_MODE_SET_ALARM_HOUR:
  case DISPLAY_MODE_SET_ALARM_MINUTE:
  case DISPLAY_MODE_ALARM_ON_OFF:
#endif
    if (!sscTasks.getTaskState(ssc_set_time_mode))
    {
      sscShowTimeSetting();
    }
    break;
#ifdef USE_TEMP_DATA
  case DISPLAY_MODE_SHOW_TEMP:
    if (!sscTasks.getTaskState(ssc_show_temp_mode))
    {
      sscShowTemp();
    }
    break;
#endif
#ifdef USE_CALENDAR
  case DISPLAY_MODE_SHOW_CALENDAR:
    if (!sscTasks.getTaskState(ssc_show_calendar_mode))
    {
      sscShowCalendar();
    }
    break;
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
#ifdef USE_LIGHT_SENSOR
  case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
    if (!sscTasks.getTaskState(ssc_set_brightness_mode))
    {
      sscShowBrightnessSetting();
    }
    break;
#endif
  default:
    break;
  }
}

#ifdef USE_CALENDAR
void sscShowCalendar()
{
  if (!sscTasks.getTaskState(ssc_show_calendar_mode))
  {
    sscTasks.startTask(ssc_show_calendar_mode);
    sscDisp.showDate(sscClock.getCurTime(), true);
  }

  if (sscDisp.showDate(sscClock.getCurTime()))
  {
    sscReturnToDefMode();
  }
}
#endif

#ifdef USE_ALARM
void sscCheckAlarm()
{
  sscAlarm.tick(sscClock.getCurTime());
  if (sscAlarm.getAlarmState() == ALARM_YES && !sscTasks.getTaskState(ssc_alarm_buzzer))
  {
    sscRunAlarmBuzzer();
  }
}

void sscRunAlarmBuzzer()
{
  static uint8_t n = 0;
  static uint8_t k = 0;
  static uint8_t m = 0;
  // "мелодия" пищалки: первая строка - частота, вторая строка - длительность
  static const PROGMEM uint32_t pick[2][8] = {
      {2000, 0, 2000, 0, 2000, 0, 2000, 0},
      {70, 70, 70, 70, 70, 70, 70, 510}};

  if (!sscTasks.getTaskState(ssc_alarm_buzzer))
  {
    sscTasks.startTask(ssc_alarm_buzzer);
    n = 0;
    k = 0;
    m = 0;
  }
  else if (sscAlarm.getAlarmState() == ALARM_ON)
  { // остановка пищалки, если будильник отключен
    sscTasks.stopTask(ssc_alarm_buzzer);
    return;
  }

  tone(BUZZER_PIN, pgm_read_dword(&pick[0][n]), pgm_read_dword(&pick[1][n]));
  sscTasks.setTaskInterval(ssc_alarm_buzzer, pgm_read_dword(&pick[1][n]), true);
  if (++n >= 8)
  {
    n = 0;
    if (++k >= ALARM_DURATION)
    { // приостановка пищалки через заданное число секунд
      k = 0;
      if (++m >= ALARM_REPETITION_COUNT)
      { // отключение пищалки после заданного количества срабатываний
        sscTasks.stopTask(ssc_alarm_buzzer);
        sscTasks.setTaskInterval(ssc_alarm_buzzer, 50, false);
        sscAlarm.setAlarmState(ALARM_ON);
      }
      else
      {
        sscTasks.setTaskInterval(ssc_alarm_buzzer, ALARM_SNOOZE_DELAY * 1000ul, true);
      }
    }
  }
}

void sscShowAlarmState(uint8_t _state)
{
#if defined(TM1637_DISPLAY)
  sscDisp.setDispData(0, 0b01110111); // "A"
  sscDisp.setDispData(1, 0b10111000); // "L:"
  sscDisp.setDispData(2, 0x00);
#elif defined(MAX72XX_7SEGMENT_DISPLAY)
  sscDisp.setDispData(0, 0b01110111); // "A"
  sscDisp.setDispData(1, 0b10001110); // "L:"
  sscDisp.setDispData(2, 0x00);
#elif defined(MAX72XX_MATRIX_DISPLAY) || defined(WS2812_MATRIX_DISPLAY)
  sscDisp.clear();
#ifdef USE_RU_LANGUAGE
  sscDisp.setDispData(1, 0xC1, 5);  // "Б"
  sscDisp.setDispData(7, 0xE4, 5);  // "д"
  sscDisp.setDispData(13, 0xEA, 5); // "к"
#else
  sscDisp.setDispData(1, 0x41, 5);  // "A"
  sscDisp.setDispData(7, 0x6C, 5);  // "l"
  sscDisp.setDispData(13, 0x6D, 5); // "r"
#endif
#ifdef MAX72XX_MATRIX_DISPLAY
  sscDisp.setColumn(2, 5, 0b00100100);
#else
  sscDisp.setColumn(21, 0b00100100);
#endif
#endif

  if (!sscBlinkFlag && !sscBtnUp.isButtonClosed() && !sscBtnDown.isButtonClosed())
  {
#if defined(TM1637_DISPLAY) || defined(MAX72XX_7SEGMENT_DISPLAY)
    sscDisp.setDispData(3, 0x00);
#endif
  }
  else
  {
#if defined(TM1637_DISPLAY)
    sscDisp.setDispData(3, (_state) ? 0b01011100 : 0b00001000);
#elif defined(MAX72XX_7SEGMENT_DISPLAY)
    sscDisp.setDispData(3, (_state) ? 0b00011101 : 0b00001000);
#elif defined(MAX72XX_MATRIX_DISPLAY) || defined(WS2812_MATRIX_DISPLAY)
    sscDisp.setDispData(25, (_state) ? 0x0C : 0x0B);
#endif
  }
}
#endif

#ifdef USE_LIGHT_SENSOR
void sscSetBrightness()
{
#ifdef USE_SET_BRIGHTNESS_MODE
  if (sscTasks.getTaskState(ssc_set_brightness_mode))
  {
    return; // в режиме настройки яркости ничего не регулировать
  }
#endif

  uint8_t x = 1;
#ifdef USE_LIGHT_SENSOR
  static uint16_t b;
  b = (b * 2 + analogRead(LIGHT_SENSOR_PIN)) / 3;
  if (b < EEPROM.read(LIGHT_THRESHOLD_EEPROM_INDEX) * 100)
  {
    x = EEPROM.read(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX);
  }
  else if (b > (EEPROM.read(LIGHT_THRESHOLD_EEPROM_INDEX) * 100 + 50))
#endif
  {
    x = EEPROM.read(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX);
  }
  sscDisp.setBrightness(x);
}
#endif

#ifdef USE_SET_BRIGHTNESS_MODE
void sscShowBrightnessSetting()
{
  static uint8_t x = 0;

  if (!sscTasks.getTaskState(ssc_set_brightness_mode))
  {
    sscTasks.startTask(ssc_set_brightness_mode);
    sscTasks.startTask(ssc_return_to_default_mode);
    x = EEPROM.read(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX);
#ifdef USE_LIGHT_SENSOR
    if (ssc_display_mode == DISPLAY_MODE_SET_BRIGHTNESS_MIN)
    {
      x = EEPROM.read(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX);
    }
#endif
  }

  // ==== опрос кнопок ===============================
  if (sscBtnSet.getBtnFlag() > BTN_FLAG_NONE)
  {
    switch (ssc_display_mode)
    {
    case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
      EEPROM.update(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX, x);
      ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
      sscStopSetting(ssc_set_brightness_mode);
      break;
#ifdef USE_LIGHT_SENSOR
    case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
      EEPROM.update(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX, x);
      if (sscBtnSet.getBtnFlag() == BTN_FLAG_NEXT)
      {
        ssc_display_mode = DISPLAY_MODE_SET_BRIGHTNESS_MAX;
      }
      else
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
      sscStopSetting(ssc_set_brightness_mode);
      break;
#endif
    default:
      break;
    }
    sscBtnSet.setBtnFlag(BTN_FLAG_NONE);
  }

  if ((sscBtnUp.getBtnFlag() == BTN_FLAG_NEXT) || (sscBtnDown.getBtnFlag() == BTN_FLAG_NEXT))
  {
    bool dir = sscBtnUp.getBtnFlag() == BTN_FLAG_NEXT;
#if defined(MAX72XX_7SEGMENT_DISPLAY) || defined(MAX72XX_MATRIX_DISPLAY)
    sscCheckData(x, 15, dir, 0, false);
#elif defined(WS2812_MATRIX_DISPLAY)
    sscCheckData(x, 25, dir, 1, false);
#else
    sscCheckData(x, 7, dir, 1, false);
#endif

    sscBtnUp.setBtnFlag(BTN_FLAG_NONE);
    sscBtnDown.setBtnFlag(BTN_FLAG_NONE);
  }

  // ==== вывод данных на экран ======================
#if defined(WS2812_MATRIX_DISPLAY)
  FastLED.setBrightness(x * 10);
#else
  sscDisp.setBrightness(x);
#endif
  bool sscBlink = !sscBlinkFlag && !sscBtnUp.isButtonClosed() && !sscBtnDown.isButtonClosed();
  bool snr = false;
#ifdef USE_LIGHT_SENSOR
  snr = true;
#endif
  sscDisp.showBrightnessData(x, sscBlink, snr,
                             ssc_display_mode != DISPLAY_MODE_SET_BRIGHTNESS_MAX);
}
#endif

#ifdef USE_TEMP_DATA
#ifdef USE_DS18B20
void sscCheckDS18b20()
{
  sscTempSensor.readData();
}
#endif
void sscShowTemp()
{
  if (!sscTasks.getTaskState(ssc_show_temp_mode))
  {
    sscTasks.startTask(ssc_return_to_default_mode);
    sscTasks.startTask(ssc_show_temp_mode);
  }

#if defined(USE_DS18B20) || defined(USE_NTC)
  sscDisp.showTemp(sscTempSensor.getTemp());
#elif defined(RTC_DS3231)
  sscDisp.showTemp(sscClock.getTemperature());
#endif
}
#endif
