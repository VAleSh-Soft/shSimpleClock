#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <avr/pgmspace.h>
#include <shButton.h> // https://github.com/VAleSh-Soft/shButton
#include "_eeprom.h"
#include "shSimpleRTC.h"
#include "shClockEvent.h"
#include "clkTaskManager.h"

// используется периодический автовывод даты и/или температуры
#define USE_AUTO_SHOW_DATA defined(USE_CALENDAR) || defined(USE_TEMP_DATA)

// дополнительные настройки; настраиваются: уровни яркости, порог переключения яркости, период автовывода и включение/отключение анимации
#define USE_OTHER_SETTING defined(USE_SET_BRIGHTNESS_MODE) || defined(USE_LIGHT_SENSOR) || defined(USE_AUTO_SHOW_DATA) || defined(USE_TICKER_FOR_DATA)

// используются матричные экраны
#define USE_MATRIX_DISPLAY defined(MAX72XX_MATRIX_DISPLAY) || defined(WS2812_MATRIX_DISPLAY)

// ===================================================

#if USE_MATRIX_DISPLAY
enum clkDataType : uint8_t
{
  NO_TAG
#ifdef USE_TICKER_FOR_DATA
  ,
  TICKER_TAG
#endif
#ifdef USE_ALARM
  ,
  ALARM_TAG
#endif
#if USE_AUTO_SHOW_DATA
  ,
  AUTO_SHOW_TAG,
  AUTO_SHOW_PERIOD_TAG
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
  ,
  SET_BRIGHTNESS_TAG
#endif
#ifdef USE_LIGHT_SENSOR
  ,
  SET_LIGHT_THRESHOLD_TAG
#endif
};
#endif

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
#if USE_AUTO_SHOW_DATA
  ,
  DISPLAY_AUTO_SHOW_DATA // автовывод даты и/или температуры в конце каждой минуты
#endif
#ifdef USE_CALENDAR
  ,
  DISPLAY_MODE_SHOW_DATE,          // вывод полной даты
  DISPLAY_MODE_SHOW_DOW,           // режим вывода дня недели
  DISPLAY_MODE_SHOW_DAY_AND_MONTH, // режим вывода числа и месяца
  DISPLAY_MODE_SHOW_YEAR           // режим вывода года
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
#ifdef USE_TICKER_FOR_DATA
  ,
  DISPLAY_MODE_SET_TICKER_ON_OFF // режим включения/выключения анимации
#endif
#if USE_AUTO_SHOW_DATA
  ,
  DISPLAY_MODE_SET_AUTO_SHOW_PERIOD // режим настройки периода автовывода даты и/или температуры
#endif
};

// ===================================================

void sscRtcNow();
void sscBlink();
void sscReturnToDefMode();
void sscShowTimeSetting();
void sscSetDisp();
void sscCheckButton();
void sscSetDisplay();
#ifdef USE_ALARM
void sscCheckAlarm();
void sscRunAlarmBuzzer();
void sscShowAlarmState(uint8_t _state); // ????????????????????????
#endif
#ifdef USE_LIGHT_SENSOR
void sscSetBrightness();
void sscShowLightThresholdData(uint8_t _data, bool blink);
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
void sscShowBrightnessData(uint8_t br, bool blink, bool toSensor, bool toMin);
#endif
#ifdef USE_TEMP_DATA
int8_t sscGetCurTemp();
#ifdef USE_DS18B20
void sscCheckDS18b20();
#endif
#endif
#if USE_OTHER_SETTING
void sscShowOtherSetting();
#endif

#ifdef USE_MATRIX_DISPLAY
void sscSetTimeString(uint8_t offset, int8_t hour, int8_t minute, bool show_colon,
                      bool toDate, bool toStringData = false);
void sscSetOtherDataString(clkDataType _type, uint8_t offset, uint8_t _data, bool blink,
                           bool toStringData = false);
void sscSetTag(uint8_t offset, uint8_t index, uint8_t width, bool toStringData);
void sscSetOnOffDataString(clkDataType _type, uint8_t offset, bool _state, bool _blink,
                           bool toStringData = false);
void sscSetNumString(uint8_t offset, uint8_t num,
                     uint8_t width = 6, uint8_t space = 1, bool toStringData = false);
void sscSetChar(uint8_t offset, uint8_t chr, uint8_t width, bool toStringData = false);
void sscShowOnOffData(clkDataType _type, bool _state, bool blink);

#ifdef USE_ALARM
void sscSetAlarmTag(uint8_t offset, bool toStringData);
#endif

#ifdef USE_TICKER_FOR_DATA
void sscSetTickerTag(uint8_t offset, bool toStringData);
void sscAssembleString(clkDisplayMode data_type, uint8_t lenght = 80);
void sscRunTicker();
#endif

#ifdef USE_CALENDAR
void sscSetDayOfWeakString(uint8_t offset, uint8_t dow, bool toStringData = false);
void sscSetYearString(uint8_t offset, uint16_t _year, bool toStringData = false);
#endif

#ifdef USE_TEMP_DATA
void sscSetTempString(uint8_t offset, int16_t temp, bool toStringData = false);
#endif

#if USE_AUTO_SHOW_DATA
void sscAutoShowData();
void sscSetAutoShowPeriodTag(uint8_t offset, bool toStringData = false);
void sscShowAutoShowPeriodData(uint8_t _data, bool blink);
uint8_t sscGetPeriodForAutoShow(uint8_t index);
#endif

#ifdef USE_SET_BRIGHTNESS_MODE
void sscSetBrightnessTag(uint8_t offset, bool toStringData);
#endif

#ifdef USE_LIGHT_SENSOR
void sscSetLightThresholdTag(uint8_t offset, bool toStringData = false);
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

shClockEvent sscClockEvent;
shClockEvent sscAlarmEvent;

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
#if USE_AUTO_SHOW_DATA
clkHandle ssc_auto_show_mode; // вывод даты и/или температуры
#endif
#if defined(USE_TEMP_DATA) && defined(USE_DS18B20)
clkHandle ssc_ds18b20_guard; // опрос датчика DS18b20
#endif
#ifdef USE_LIGHT_SENSOR
clkHandle ssc_light_sensor_guard; // отслеживание показаний датчика света
#endif
#if USE_OTHER_SETTING
clkHandle ssc_other_setting_mode; // режим настроек цифровых данных
#endif
#ifdef USE_TICKER_FOR_DATA
clkHandle ssc_ticker; // отработка бегущей строки
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
Alarm sscAlarm(ALARM_LED_PIN, ALARM_DATA_EEPROM_INDEX);
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
  clkButton(uint8_t button_pin) : shButton(button_pin, BTN_INPUT_TYPE, BTN_TYPE)
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
#ifdef USE_BUZZER_FOR_BUTTON
      if (_state != BTN_LONGCLICK)
      {
        tone(BUZZER_PIN, 2000, 25); // на каждый клик срабатывает пищалка
      }
#endif
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

    // ==== валидация EEPROM ===========================
#if USE_AUTO_SHOW_DATA
    if (read_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX) > 7)
    {
      write_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX, 1);
    }
#endif

#ifdef USE_LIGHT_SENSOR
    uint8_t l = read_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX);
    if (l > 9 || l == 0)
    {
      write_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX, 3);
    }
#endif

// ==== датчики ====================================
#if defined(USE_NTC)
    sscTempSensor.setADCbitDepth(_bit_depth); // установить разрядность АЦП вашего МК, для AVR обычно равна 10 бит
#endif
    // проверить корректность заданных уровней яркости
    uint8_t x = read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX);
#if defined(MAX72XX_7SEGMENT_DISPLAY) || defined(MAX72XX_MATRIX_DISPLAY)
    x = (x > 15) ? 8 : x;
#elif defined(WS2812_MATRIX_DISPLAY)
    x = ((x > 25) || (x == 0)) ? 15 : x;
#else
    x = ((x > 7) || (x == 0)) ? 7 : x;
#endif
    write_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX, x);
#ifdef USE_LIGHT_SENSOR
    uint8_t y = read_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX);
    if ((y > 9) || (y == 0))
    {
      write_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX, 3);
    }

    x = read_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX);
#if defined(MAX72XX_7SEGMENT_DISPLAY) || defined(MAX72XX_MATRIX_DISPLAY)
    x = (x > 15) ? 0 : x;
#elif defined(WS2812_MATRIX_DISPLAY)
    x = ((x > 25) || (x == 0)) ? 1 : x;
#else
    x = ((x > 7) || (x == 0)) ? 1 : x;
#endif
    write_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX, x);
#else
    sscDisp.setBrightness(read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX));
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
#ifdef USE_ALARM
    task_count += 2;
#endif
#if USE_AUTO_SHOW_DATA
    task_count++;
#endif
#if defined(USE_TEMP_DATA) && defined(USE_DS18B20)
    task_count++;
#endif
#ifdef USE_LIGHT_SENSOR
    task_count++;
#endif
#ifdef USE_OTHER_SETTING
    task_count++;
#endif
#ifdef USE_TICKER_FOR_DATA
    task_count++;
#endif
    sscTasks.init(task_count);

    ssc_rtc_guard = sscTasks.addTask(50ul, sscRtcNow);
    ssc_blink_timer = sscTasks.addTask(50ul, sscBlink);
    ssc_return_to_default_mode = sscTasks.addTask(AUTO_EXIT_TIMEOUT * 1000ul, sscReturnToDefMode, false);
    ssc_set_time_mode = sscTasks.addTask(100ul, sscShowTimeSetting, false);
#if defined(USE_TEMP_DATA) && defined(USE_DS18B20)
    ssc_ds18b20_guard = sscTasks.addTask(3000ul, sscCheckDS18b20);
#endif
#if USE_AUTO_SHOW_DATA
    ssc_auto_show_mode = sscTasks.addTask(100ul, sscAutoShowData, false);
#endif
#ifdef USE_ALARM
    ssc_alarm_guard = sscTasks.addTask(200ul, sscCheckAlarm);
    ssc_alarm_buzzer = sscTasks.addTask(50ul, sscRunAlarmBuzzer, false);
#endif
    ssc_display_guard = sscTasks.addTask(50ul, sscSetDisp);
#if defined(USE_LIGHT_SENSOR)
    ssc_light_sensor_guard = sscTasks.addTask(100ul, sscSetBrightness);
#else
    sscDisp.setBrightness(read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX));
#endif
#if USE_OTHER_SETTING
    ssc_other_setting_mode = sscTasks.addTask(100ul, sscShowOtherSetting, false);
#endif
#ifdef USE_TICKER_FOR_DATA
    ssc_ticker = sscTasks.addTask(1000ul / TICKER_SPEED, sscRunTicker, false);
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
   * @brief получить текущий режим экрана часов
   *
   * @return clkDisplayMode
   */
  clkDisplayMode getDisplayMode() { return ssc_display_mode; }

  /**
   * @brief установить режим экрана часов
   *
   * @param _mode режим для установки
   */
  void setDisplayMode(clkDisplayMode _mode) { ssc_display_mode = _mode; }

  /**
   * @brief подключить callback-функцию к ежесекундному событию
   *
   * @param _interval интервал вызова функции в секундах
   * @param _callback вызываемая функция
   * @param _active статус события - активно/не активно
   */
  void setClockEvent(uint16_t _interval, sceCallback _callback, bool _active = true)
  {
    sscClockEvent.init(_interval, _callback, _active);
  }

  /**
   * @brief установить статус ежесекундного события
   *
   * @param _state true - событие активно; flase - событие не активно
   */
  void setClockEventState(bool _state) { sscClockEvent.setState(_state); }

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

  /**
   * @brief установить цвет фона на матрице из адресных светодиодов
   *
   * @param _color цвет, например, для синего CRGB::Blue
   */
  void setColorOfBackground(CRGB _color)
  {
    sscDisp.setColorOfBackground(_color);
  }

  /**
   * @brief установка максимальной мощности блока питания матрицы
   *
   * @param volts напряжение, Вольт
   * @param milliamps максимальный ток, милиампер
   */
  void setMaxPSP(uint8_t volts, uint32_t milliamps) { sscDisp.setMaxPSP(volts, milliamps); }
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
    return sscGetCurTemp();
  }
#endif

#ifdef USE_ALARM
  /**
   * @brief подключить callback-функцию к событию будильника
   *
   * @param _callback вызываемая функция
   * @param _active статус события - активно/не активно
   */
  void setAlarmEvent(sceCallback _callback, bool _active = true)
  {
    sscAlarmEvent.init(_callback, _active);
  }

  /**
   * @brief установить статус события будильника
   *
   * @param _state true - событие активно; flase - событие не активно
   */
  void setAlarmEventState(bool _state) { sscAlarmEvent.setState(_state); }

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

  /**
   * @brief отключение сигнала сработавшего будильника
   *
   */
  void buzzerStop()
  {
    if (sscAlarm.getAlarmState() == ALARM_YES)
    {
      sscAlarm.setAlarmState(ALARM_ON);
    }
  }
#endif

#ifdef USE_SET_BRIGHTNESS_MODE
  /**
   * @brief получение значения максимальной яркости экрана
   *
   * @return uint8_t
   */
  uint8_t getBrightnessMax() { return (read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX)); }

  /**
   * @brief установка максимальной яркости экрана
   *
   * @param _br новое значение
   */
  void setBrightnessMax(uint8_t _br)
  {
    write_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX, _br);
#if defined(CHIPSET_WS2812B)
    _br *= 10;
#endif
    sscDisp.setBrightness(_br);
  }

#if defined(USE_LIGHT_SENSOR)
  /**
   * @brief получение значения минимальной яркости экрана
   *
   * @return uint8_t
   */
  uint8_t getBrightnessMin() { return (read_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX)); }

  /**
   * @brief установка минимальной яркости экрана
   *
   * @param _br
   */
  void setBrightnessMin(uint8_t _br)
  {
    write_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX, _br);
#if defined(CHIPSET_WS2812B)
    _br *= 10;
#endif
    sscDisp.setBrightness(_br);
  }
#endif
#endif

#ifdef USE_LIGHT_SENSOR
  /**
   * @brief получение текущего порога переключения яркости
   *
   * @return uint8_t интервал 1..9
   */
  uint8_t getLightThresholdValue()
  {
    return (read_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX));
  }

  /**
   * @brief установка нового порога переключения яркости
   *
   * @param _data новое значение, интервал 1..9
   */
  void setLightThresholdValue(uint8_t _data)
  {
    _data = (_data > 9) ? 9 : (_data == 0) ? 1
                                           : _data;
    write_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX, _data);
  }
#endif

#ifdef USE_MATRIX_DISPLAY

#ifdef USE_TICKER_FOR_DATA
  /**
   * @brief включение/выключение анимации
   *
   * @param _state true -включена, false - отключена
   */
  void setAnimationState(bool _state)
  {
    write_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX, (byte)_state);
  }

  /**
   * @brief получение текущего состояния анимации
   *
   * @return true включена
   * @return false отключена
   */
  bool getAnimationState()
  {
    return (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX));
  }
#endif

#if USE_AUTO_SHOW_DATA
  /**
   * @brief установка интервала автовывода информации
   *
   * @param _index интервал задается в пределах 0..6, при этом соотвествтие индексов интервалу следующее: 0 -> 0, 1 -> 1, 2 -> 5, 3 -> 10, 4 -> 15, 5 -> 20, 6 -> 30, 7 -> 60
   */
  void setIntervalForAutoShowData(uint8_t _index)
  {
    _index = (_index > 7) ? 1 : _index;
    write_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX, _index);
  }

  /**
   * @brief получение текущего интервала автовывода информации
   *
   * @return uint8_t
   */
  uint8_t getIntervalForAutoShowData()
  {
    return read_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX);
  }
#endif

#endif
};

// ==== end shSimpleClock ============================

void sscRtcNow()
{
  static bool flag = false;

  sscClock.now();
  if (ssc_display_mode == DISPLAY_MODE_SHOW_TIME)
  {
#ifdef USE_TICKER_FOR_DATA
    // если работает бегущая строка, ничего не делать
    if (sscTasks.getTaskState(ssc_ticker))
    {
      return;
    }
#endif

#if defined(USE_CALENDAR) || defined(USE_TEMP_DATA)
    uint8_t x = read_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX);
    if (((sscGetPeriodForAutoShow(x) > 0) &&
         (sscClock.getCurTime().minute() % sscGetPeriodForAutoShow(x) == 0)) &&
        (sscClock.getCurTime().second() == 0) &&
        !flag)
    {
      flag = true;
      ssc_display_mode = DISPLAY_AUTO_SHOW_DATA;
    }
    else
#endif
    {
#ifdef USE_MATRIX_DISPLAY
      sscDisp.showTime(sscClock.getCurTime().hour(), sscClock.getCurTime().minute(), sscClock.getCurTime().second(), sscBlinkFlag);
#else
      sscDisp.showTime(sscClock.getCurTime().hour(), sscClock.getCurTime().minute(), sscBlinkFlag);
#endif
    }
  }

  if (sscClock.getCurTime().second() > 0)
  {
    flag = false;
  }
}

void sscBlink()
{
  static uint8_t cur_sec = sscClock.getCurTime().second();
  static uint32_t tmr = 0;
  if (cur_sec != sscClock.getCurTime().second())
  {
    sscClockEvent.run();
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
#ifdef USE_LIGHT_SENSOR
  case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
#ifdef USE_LIGHT_SENSOR
  case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
#endif
#ifdef USE_TICKER_FOR_DATA
  case DISPLAY_MODE_SET_TICKER_ON_OFF:
#endif
#if USE_AUTO_SHOW_DATA
  case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
#endif
    sscBtnSet.setBtnFlag(BTN_FLAG_EXIT);
    break;
#if USE_AUTO_SHOW_DATA
  case DISPLAY_AUTO_SHOW_DATA:
    sscTasks.stopTask(ssc_auto_show_mode);
    ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
    break;
#endif
  default:
    break;
  }
#ifdef USE_TICKER_FOR_DATA
  if (ssc_display_mode == DISPLAY_MODE_SHOW_TIME && read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
  {
    sscAssembleString(DISPLAY_MODE_SHOW_TIME);
  }
#endif
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

// ==== sscShowTimeSetting ===========================
void _startTimeSettingMode(uint8_t &curHour, uint8_t &curMinute)
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
#ifdef USE_TICKER_FOR_DATA
  case DISPLAY_MODE_SET_TICKER_ON_OFF:
    curHour = read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX);
    break;
#endif
  default:
    break;
  }
#ifdef USE_TICKER_FOR_DATA
  if (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
  {
    switch (ssc_display_mode)
    {
#ifdef USE_ALARM
    case DISPLAY_MODE_ALARM_ON_OFF:
    case DISPLAY_MODE_SET_ALARM_HOUR:
#endif
#ifdef USE_CALENDAR
    case DISPLAY_MODE_SET_DAY:
    case DISPLAY_MODE_SET_YEAR:
#endif
#ifdef USE_TICKER_FOR_DATA
    case DISPLAY_MODE_SET_TICKER_ON_OFF:
      if (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
      {
        sscAssembleString(ssc_display_mode);
      }
      break;
#endif
    default:
      break;
    }
  }
#endif
}

void _checkBtnSetForTmSet(uint8_t &curHour, uint8_t &curMinute, bool &time_checked)
{
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
#ifdef USE_TICKER_FOR_DATA
      case DISPLAY_MODE_SET_TICKER_ON_OFF:
        EEPROM.write(TICKER_STATE_VALUE_EEPROM_INDEX, curHour);
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
#ifdef USE_TICKER_FOR_DATA
      case DISPLAY_MODE_SET_TICKER_ON_OFF:
#if USE_AUTO_SHOW_DATA
        ssc_display_mode = DISPLAY_MODE_SET_AUTO_SHOW_PERIOD;
#else
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
#endif
        sscStopSetting(ssc_set_time_mode);
        break;
#endif
#if USE_AUTO_SHOW_DATA
      case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
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
}

void _checkBtnUpDownForTmSet(uint8_t &curHour, uint8_t &curMinute, bool &time_checked)
{
#ifdef USE_CALENDAR
  static const uint8_t PROGMEM days_of_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
#endif
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
#ifdef USE_TICKER_FOR_DATA
    case DISPLAY_MODE_SET_TICKER_ON_OFF:
      sscCheckData(curHour, 1, true);
      break;
#endif
    default:
      break;
    }
    time_checked = true;
    sscBtnUp.setBtnFlag(BTN_FLAG_NONE);
    sscBtnDown.setBtnFlag(BTN_FLAG_NONE);
  }
}

void _setDisplayForTmSet(uint8_t &curHour, uint8_t &curMinute)
{
  if (ssc_display_mode == DISPLAY_MODE_SHOW_TIME)
  {
#ifdef USE_TICKER_FOR_DATA
    if (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
    {
      sscAssembleString(DISPLAY_MODE_SHOW_TIME);
    }
#endif
  }
  else if (sscTasks.getTaskState(ssc_set_time_mode))
  {
    switch (ssc_display_mode)
    {
#ifdef USE_ALARM
    case DISPLAY_MODE_ALARM_ON_OFF:
      sscShowOnOffData(ALARM_TAG,
                       curHour,
                       (!sscBlinkFlag &&
                        (&sscBtnUp == NULL || !sscBtnUp.isButtonClosed()) &&
                        (&sscBtnDown == NULL || !sscBtnDown.isButtonClosed())));
      break;
#endif
#ifdef USE_TICKER_FOR_DATA
    case DISPLAY_MODE_SET_TICKER_ON_OFF:
      sscShowOnOffData(TICKER_TAG,
                       curHour,
                       (!sscBlinkFlag &&
                        (&sscBtnUp == NULL || !sscBtnUp.isButtonClosed()) &&
                        (&sscBtnDown == NULL || !sscBtnDown.isButtonClosed())));
      break;
#endif
    default:
      sscShowTimeData(curHour, curMinute);
      break;
    }
  }
}

void sscShowTimeSetting()
{
  static bool time_checked = false;
  static uint8_t curHour = 0;
  static uint8_t curMinute = 0;

  if (!sscTasks.getTaskState(ssc_set_time_mode))
  {
    _startTimeSettingMode(curHour, curMinute);
    time_checked = false;
  }

#ifdef USE_TICKER_FOR_DATA
  // подождать, пока отработает бегущая строка
  if (sscTasks.getTaskState(ssc_ticker))
  {
    return;
  }
#endif

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
  _checkBtnSetForTmSet(curHour, curMinute, time_checked);
  _checkBtnUpDownForTmSet(curHour, curMinute, time_checked);

  // вывод данных на экран ============
  _setDisplayForTmSet(curHour, curMinute);
}

// ==== end sscShowTimeSetting =======================

void sscSetDisp()
{
#ifdef USE_TICKER_FOR_DATA
  // обновление экрана делать только если в данный момент не работает бегущая строка, она сама обновляет экран, когда ей это нужно
  if (!sscTasks.getTaskState(ssc_ticker))
#endif
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
#ifdef USE_LIGHT_SENSOR
    case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
#endif
#ifdef USE_TICKER_FOR_DATA
    case DISPLAY_MODE_SET_TICKER_ON_OFF:
#endif
#if USE_AUTO_SHOW_DATA
    case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
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
    case DISPLAY_MODE_SHOW_DATE:
      sscTasks.stopTask(ssc_auto_show_mode);
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
#ifdef USE_TICKER_FOR_DATA
    case DISPLAY_MODE_SET_TICKER_ON_OFF:
#endif
#if USE_AUTO_SHOW_DATA
    case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
#endif
#ifdef USE_LIGHT_SENSOR
    case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
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
    switch (ssc_display_mode)
    {
#ifdef USE_ALARM
    case DISPLAY_MODE_ALARM_ON_OFF:
#endif
#ifdef USE_TICKER_FOR_DATA
    case DISPLAY_MODE_SET_TICKER_ON_OFF:
#endif
      return;
    default:
      btn.setBtnFlag(BTN_FLAG_NEXT);
      break;
    }
    break;
  }
}

void sscCheckUpDownButton()
{
  uint8_t btn_up = 0;
  uint8_t btn_down = 0;

  if (&sscBtnUp != NULL)
  {
    btn_up = sscBtnUp.getButtonState();
  }
  if (&sscBtnDown != NULL)
  {
    btn_down = sscBtnDown.getButtonState();
  }

  switch (ssc_display_mode)
  {
  case DISPLAY_MODE_SHOW_TIME:
    if (btn_down == BTN_LONGCLICK)
    {
#ifdef USE_TICKER_FOR_DATA
      // вход в настройки анимации
      ssc_display_mode = DISPLAY_MODE_SET_TICKER_ON_OFF;
      sscBtnDown.resetButtonState();
#elif USE_AUTO_SHOW_DATA
      // вход в настройки периода автовывода на экран даты и/или температуры
      ssc_display_mode = DISPLAY_MODE_SET_AUTO_SHOW_PERIOD;
      sscBtnDown.resetButtonState();
#endif
    }
#ifdef USE_TEMP_DATA
    if (btn_up == BTN_ONECLICK)
    {
      ssc_display_mode = DISPLAY_MODE_SHOW_TEMP;
    }
#endif
#ifdef USE_CALENDAR
    if (btn_down == BTN_ONECLICK)
    {
      ssc_display_mode = DISPLAY_MODE_SHOW_DATE;
    }
#endif
    if (&sscBtnUp != NULL &&
            (&sscBtnDown != NULL &&
             sscBtnUp.isSecondButtonPressed(sscBtnDown, BTN_LONGCLICK)) ||
        &sscBtnDown != NULL &&
            (&sscBtnUp != NULL &&
             sscBtnDown.isSecondButtonPressed(sscBtnUp, BTN_LONGCLICK)))
    {
#ifdef USE_SET_BRIGHTNESS_MODE
#ifdef USE_LIGHT_SENSOR
      ssc_display_mode = DISPLAY_MODE_SET_BRIGHTNESS_MIN;
#else
      ssc_display_mode = DISPLAY_MODE_SET_BRIGHTNESS_MAX;
#endif
#elif defined(USE_LIGHT_SENSOR)
      ssc_display_mode = DISPLAY_MODE_SET_LIGHT_THRESHOLD;
#endif
    }
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
#ifdef USE_LIGHT_SENSOR
  case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
#ifdef USE_LIGHT_SENSOR
  case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
#endif
#ifdef USE_TICKER_FOR_DATA
  case DISPLAY_MODE_SET_TICKER_ON_OFF:
#endif
#if USE_AUTO_SHOW_DATA
  case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
#endif
    if (&sscBtnUp != NULL && (&sscBtnDown == NULL || !sscBtnDown.isButtonClosed()))
    {
      sscCheckUDbtn(sscBtnUp);
    }
    if (&sscBtnDown != NULL && (&sscBtnUp == NULL || !sscBtnUp.isButtonClosed()))
    {
      sscCheckUDbtn(sscBtnDown);
    }
    break;
#ifdef USE_TEMP_DATA
  case DISPLAY_MODE_SHOW_TEMP:
    if (btn_up == BTN_ONECLICK)
    {
      sscReturnToDefMode();
    }
    break;
#endif
#ifdef USE_CALENDAR
  case DISPLAY_MODE_SHOW_DATE:
    if (btn_down == BTN_ONECLICK)
    { // выход из режима показа даты по клику кнопкой Down
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
#ifdef USE_TICKER_FOR_DATA
  case DISPLAY_MODE_SET_TICKER_ON_OFF:
#endif
    if (!sscTasks.getTaskState(ssc_set_time_mode))
    {
      sscShowTimeSetting();
    }
    break;
#if USE_AUTO_SHOW_DATA
  case DISPLAY_AUTO_SHOW_DATA:
#ifdef USE_CALENDAR
  case DISPLAY_MODE_SHOW_DATE:
#endif
#ifdef USE_TEMP_DATA
  case DISPLAY_MODE_SHOW_TEMP:
#endif
    if (!sscTasks.getTaskState(ssc_auto_show_mode))
    {
      sscAutoShowData();
    }
    break;
#endif
#if USE_OTHER_SETTING
#if USE_AUTO_SHOW_DATA
  case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
#endif
#ifdef USE_LIGHT_SENSOR
  case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
#ifdef USE_LIGHT_SENSOR
  case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
#endif
    if (!sscTasks.getTaskState(ssc_other_setting_mode))
    {
      sscShowOtherSetting();
    }
    break;
#endif
  default:
    break;
  }
}

#ifdef USE_CALENDAR
#endif

#ifdef USE_ALARM
void sscCheckAlarm()
{
  sscAlarm.tick(sscClock.getCurTime());
  if (sscAlarm.getAlarmState() == ALARM_YES && !sscTasks.getTaskState(ssc_alarm_buzzer))
  {
    sscRunAlarmBuzzer();
    sscAlarmEvent.run();
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
  sscDisp.setColumn(21, 0b00100100);
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
  if (sscTasks.getTaskState(ssc_other_setting_mode))
  {
    if (ssc_display_mode == DISPLAY_MODE_SET_BRIGHTNESS_MAX ||
        ssc_display_mode == DISPLAY_MODE_SET_BRIGHTNESS_MIN)
    {
      return; // в режиме настройки яркости ничего не регулировать
    }
  }
#endif

  uint8_t x = 1;
#ifdef USE_LIGHT_SENSOR
  static uint16_t b;
  b = (b * 2 + analogRead(LIGHT_SENSOR_PIN)) / 3;
  if (b < read_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX) * 100)
  {
    x = read_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX);
  }
  else if (b > (read_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX) * 100 + 50))
#endif
  {
    x = read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX);
  }
  sscDisp.setBrightness(x);
}

void sscShowLightThresholdData(uint8_t _data, bool blink)
{
  sscDisp.clear();

  sscSetOtherDataString(SET_LIGHT_THRESHOLD_TAG, 1, _data, blink);
}
#endif

#ifdef USE_TEMP_DATA

#ifdef USE_DS18B20
void sscCheckDS18b20()
{
  sscTempSensor.readData();
}
#endif

int8_t sscGetCurTemp()
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

#if USE_OTHER_SETTING

// ==== sscShowOtherSetting =============================
void _startOtherSettingMode(uint8_t &x)
{
  sscTasks.startTask(ssc_other_setting_mode);
  sscTasks.startTask(ssc_return_to_default_mode);
  switch (ssc_display_mode)
  {
#ifdef USE_LIGHT_SENSOR
  case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
    x = read_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX);
    break;
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
    x = read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX);
    break;
#ifdef USE_LIGHT_SENSOR
  case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
    x = read_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX);
    break;
#endif
#endif
#if USE_AUTO_SHOW_DATA
  case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
    x = read_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX);
    break;
#endif
  }
#ifdef USE_TICKER_FOR_DATA
  if (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
  {
    sscAssembleString(ssc_display_mode);
  }
#endif
}

void _checkBtnSetForOthSet(uint8_t &x)
{
  if (&sscBtnSet != NULL)
  {
    if (sscBtnSet.getBtnFlag() > BTN_FLAG_NONE)
    {
      switch (ssc_display_mode)
      {
#ifdef USE_LIGHT_SENSOR
      case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
        write_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX, x);
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
        sscStopSetting(ssc_other_setting_mode);
        break;
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
      case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
        write_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX, x);
#ifdef USE_LIGHT_SENSOR
        if (sscBtnSet.getBtnFlag() == BTN_FLAG_NEXT)
        {
          ssc_display_mode = DISPLAY_MODE_SET_LIGHT_THRESHOLD;
        }
        else
#endif
        {
          ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
        }
        sscStopSetting(ssc_other_setting_mode);
        break;
#ifdef USE_LIGHT_SENSOR
      case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
        write_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX, x);
        if (sscBtnSet.getBtnFlag() == BTN_FLAG_NEXT)
        {
          ssc_display_mode = DISPLAY_MODE_SET_BRIGHTNESS_MAX;
        }
        else
        {
          ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
        }
        sscStopSetting(ssc_other_setting_mode);
        break;
#endif
#endif
#if USE_AUTO_SHOW_DATA
      case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
        write_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX, x);
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
        sscStopSetting(ssc_other_setting_mode);
        break;
#endif
      default:
        break;
      }
      sscBtnSet.setBtnFlag(BTN_FLAG_NONE);
    }
  }
}

void _checkBtnUpDownForOthSet(uint8_t &x)
{
  if (((&sscBtnUp != NULL) && (sscBtnUp.getBtnFlag() == BTN_FLAG_NEXT)) ||
      ((&sscBtnDown != NULL) && (sscBtnDown.getBtnFlag() == BTN_FLAG_NEXT)))
  {
    bool dir = ((&sscBtnUp != NULL) && (sscBtnUp.getBtnFlag() == BTN_FLAG_NEXT));
    switch (ssc_display_mode)
    {
#ifdef USE_LIGHT_SENSOR
    case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
      sscCheckData(x, 9, dir, 1, false);
      break;
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
#ifdef USE_LIGHT_SENSOR
    case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
    case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
#if defined(MAX72XX_MATRIX_DISPLAY)
      sscCheckData(x, 15, dir, 0, false);
#elif defined(WS2812_MATRIX_DISPLAY)
      sscCheckData(x, 25, dir, 1, false);
#endif
      break;
#endif
#if USE_AUTO_SHOW_DATA
    case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
      sscCheckData(x, 7, dir, 0, false);
      break;
#endif
    default:
      break;
    }

    sscBtnUp.setBtnFlag(BTN_FLAG_NONE);
    sscBtnDown.setBtnFlag(BTN_FLAG_NONE);
  }
}

void _setDisplayDataForOthSet(uint8_t &x)
{
  if (ssc_display_mode == DISPLAY_MODE_SHOW_TIME)
  {
#ifdef USE_TICKER_FOR_DATA
    if (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
    {
      sscAssembleString(DISPLAY_MODE_SHOW_TIME);
    }
#endif
  }
  else
  {
    bool blink = !sscBlinkFlag && !sscBtnUp.isButtonClosed() && !sscBtnDown.isButtonClosed();
    switch (ssc_display_mode)
    {
#ifdef USE_LIGHT_SENSOR
    case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
      sscShowLightThresholdData(x, blink);
      break;
#endif
#ifdef USE_SET_BRIGHTNESS_MODE
#ifdef USE_LIGHT_SENSOR
    case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
    case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
      sscDisp.setBrightness(x);
      bool snr;
#ifdef USE_LIGHT_SENSOR
      snr = true;
#else
      snr = false;
#endif
      sscShowBrightnessData(x, blink, snr,
                            ssc_display_mode != DISPLAY_MODE_SET_BRIGHTNESS_MAX);
      break;
#endif
#if USE_AUTO_SHOW_DATA
    case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
      sscShowAutoShowPeriodData(x, blink);
      break;
#endif
    default:
      break;
    }
  }
}

void sscShowOtherSetting()
{
  static uint8_t x = 0;

  // ==== инициализация диалога настроек =============
  if (!sscTasks.getTaskState(ssc_other_setting_mode))
  {
    _startOtherSettingMode(x);
  }

#ifdef USE_TICKER_FOR_DATA
  // подождать, пока отработает бегущая строка
  if (sscTasks.getTaskState(ssc_ticker))
  {
    return;
  }
#endif

  // ==== опрос кнопок ===============================
  _checkBtnSetForOthSet(x);

  _checkBtnUpDownForOthSet(x);

  // ==== вывод данных на экран ======================
  _setDisplayDataForOthSet(x);
}
// ==== end sscShowOtherSetting =========================

#endif

#ifdef USE_MATRIX_DISPLAY

void sscSetTimeString(uint8_t offset, int8_t hour, int8_t minute, bool show_colon,
                      bool toDate, bool toStringData)
{
  if (hour >= 0)
  {
    sscSetNumString(offset, hour, 6, 1, toStringData);
  }
  if (minute >= 0)
  {
    sscSetNumString(offset + 16, minute, 6, 1, toStringData);
  }
  if (show_colon)
  {
    uint8_t colon = (toDate) ? 0x01 : 0x24;
    if (toStringData)
    {
      sData.setData(offset + 14, colon);
    }
    else
    {
      sscDisp.setColumn(offset + 14, colon);
    }
  }
}

void sscSetOtherDataString(clkDataType _type, uint8_t offset, uint8_t _data, bool blink,
                           bool toStringData)
{
  switch (_type)
  {
#ifdef USE_SET_BRIGHTNESS_MODE
  case SET_BRIGHTNESS_TAG:
    sscSetBrightnessTag(offset, toStringData);
    break;
#endif
#ifdef USE_LIGHT_SENSOR
  case SET_LIGHT_THRESHOLD_TAG:
    sscSetLightThresholdTag(offset, toStringData);
    break;
#endif
#if USE_AUTO_SHOW_DATA
  case AUTO_SHOW_PERIOD_TAG:
    sscSetAutoShowPeriodTag(offset, toStringData);
    break;
#endif
  default:
    break;
  }
  if (toStringData)
  {
    sData.setData(offset + 18, 0x24); // ":"
  }
  else
  {
    sscDisp.setColumn(offset + 18, 0x24); // ":"
  }

  if (!blink)
  {
    _data = (_data < 100) ? _data : _data % 100;
    if (_data >= 10)
    {
      sscSetChar(offset + 20, _data / 10 + 0x30, 5, toStringData);
      sscSetChar(offset + 26, _data % 10 + 0x30, 5, toStringData);
    }
    else
    {
      sscSetChar(offset + 23, _data % 10 + 0x30, 5, toStringData);
    }
  }
}

void sscSetTag(uint8_t offset, uint8_t index, uint8_t width, bool toStringData)
{
  for (uint8_t i = 0; i < 3; i++)
  {
    sscSetChar(offset + i * 6,
               pgm_read_byte(&tags[index * 3 + i]),
               5,
               toStringData);
  }
}

void sscSetOnOffDataString(clkDataType _type, uint8_t offset, bool _state, bool _blink,
                           bool toStringData)
{
  switch (_type)
  {
#ifdef USE_TICKER_FOR_DATA
  case TICKER_TAG:
    sscSetTickerTag(offset, toStringData);
    break;
#endif
#ifdef USE_ALARM
  case ALARM_TAG:
    sscSetAlarmTag(offset, toStringData);
    break;
#endif
  default:
    break;
  }

  if (toStringData)
  {
    sData.setData(offset + 20, 0x24); // ":"
  }
  else
  {
    sscDisp.setColumn(offset + 20, 0x24); // ":"
  }

  if (!_blink)
  {
    sscSetChar(offset + 24, ((_state) ? 0x0C : 0x0B), 6, toStringData);
  }
}

void sscSetNumString(uint8_t offset, uint8_t num, uint8_t width, uint8_t space, bool toStringData)
{
  sscSetChar(offset, num / 10, width, toStringData);
  sscSetChar(offset + width + space, num % 10, width, toStringData);
}

void sscSetChar(uint8_t offset, uint8_t chr, uint8_t width, bool toStringData)
{
  for (uint8_t j = offset, i = 0; i < width; j++, i++)
  {
    uint8_t chr_data = 0;
    switch (width)
    {
    case 5:
      chr_data = reverseByte(pgm_read_byte(&font_5_7[chr * width + i]));
      break;
    case 6:
      chr_data = pgm_read_byte(&font_digit[chr * width + i]);
      break;
    default:
      break;
    }

    if (toStringData)
    {
      if (j < sData.getDataLenght())
      {
        sData.setData(j, chr_data);
      }
    }
    else
    {
      if (j < 32)
      {
        sscDisp.setColumn(j, chr_data);
      }
    }
  }
}

void sscShowOnOffData(clkDataType _type, bool _state, bool blink)
{
  sscDisp.clear();

  sscSetOnOffDataString(_type, 1, _state, blink);
}

#ifdef USE_ALARM
void sscSetAlarmTag(uint8_t offset, bool toStringData)
{
  sscSetTag(offset, DISP_ALARM_TAG, 5, toStringData);
}
#endif

#ifdef USE_TICKER_FOR_DATA
void sscSetTickerTag(uint8_t offset, bool toStringData)
{
  sscSetTag(offset, DISP_ANIMATION_TAG, 5, toStringData);
}

void sscAssembleString(clkDisplayMode data_type, uint8_t lenght)
{
  if (!sData.stringInit(lenght))
    return;

  // скопировать в начало строки содержимое экрана
  for (uint8_t i = 0; i < 32; i++)
  {
    sData.setData(i, sscDisp.getColumn(i));
  }

  // сформировать вторую часть строки
  switch (data_type)
  {
  case DISPLAY_MODE_SHOW_TIME: // время
    sscSetTimeString(lenght - 31,
                     sscClock.getCurTime().hour(),
                     sscClock.getCurTime().minute(),
                     true,
                     false,
                     true);
    break;

#ifdef USE_TEMP_DATA
  case DISPLAY_MODE_SHOW_TEMP: // температура
    sscSetTempString(lenght - 31, sscGetCurTemp(), true);
    break;
#endif

#ifdef USE_SET_BRIGHTNESS_MODE
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX: // настройка яркости
#if defined(USE_LIGHT_SENSOR)
  case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
    uint8_t x;
    x = read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX);
    bool snr;
    snr = false;
    bool to_min;
    to_min = false;
#if defined(USE_LIGHT_SENSOR)
    x = read_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX);
    snr = true;
    to_min = ssc_display_mode == DISPLAY_MODE_SET_BRIGHTNESS_MIN;
#else
    x = read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX);
    snr = false;
#endif
    sscSetOtherDataString(SET_BRIGHTNESS_TAG, lenght - 31, x, false, true);
    break;
#endif

#ifdef USE_LIGHT_SENSOR
  case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
    uint8_t y;
    y = read_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX);
    sscSetOtherDataString(SET_LIGHT_THRESHOLD_TAG, lenght - 31, y, false, true);
    break;
#endif

#ifdef USE_ALARM
  case DISPLAY_MODE_SET_ALARM_HOUR: // настройка часа срабатывания будильника
    sscSetTimeString(lenght - 31,
                     sscAlarm.getAlarmPoint() / 60,
                     sscAlarm.getAlarmPoint() % 60,
                     true,
                     false,
                     true);
    break;
  case DISPLAY_MODE_ALARM_ON_OFF: // настройка включения/выключения будильника
    sscSetOnOffDataString(ALARM_TAG, lenght - 31, sscAlarm.getOnOffAlarm(), false, true);
    break;
#endif

#ifdef USE_CALENDAR
  case DISPLAY_MODE_SHOW_DOW: // день недели
    sscSetDayOfWeakString(lenght - 25,
                          getDayOfWeek(sscClock.getCurTime().day(),
                                       sscClock.getCurTime().month(),
                                       sscClock.getCurTime().year()),
                          true);
    break;

  case DISPLAY_MODE_SHOW_DAY_AND_MONTH: // число и месяц
  case DISPLAY_MODE_SET_DAY:            // настройка числа
    sscSetTimeString(lenght - 31, sscClock.getCurTime().day(), sscClock.getCurTime().month(), true, true, true);
    break;

  case DISPLAY_MODE_SHOW_YEAR: // год
  case DISPLAY_MODE_SET_YEAR:  // настройка года
    sscSetYearString(lenght - 31, sscClock.getCurTime().year(), true);
    break;
#endif
#ifdef USE_TICKER_FOR_DATA
  case DISPLAY_MODE_SET_TICKER_ON_OFF: // настройка включения/выключения анимации
    sscSetOnOffDataString(TICKER_TAG, lenght - 31, read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX), false, true);
    break;
#endif
#if USE_AUTO_SHOW_DATA
  case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
    sscSetOtherDataString(AUTO_SHOW_PERIOD_TAG,
                          lenght - 31,
                          sscGetPeriodForAutoShow(read_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX)),
                          false,
                          true);
    break;
#endif
  }
  sscRunTicker();
}

void sscRunTicker()
{
  static uint8_t n = 0;

  if (!sscTasks.getTaskState(ssc_ticker))
  {
    sscTasks.startTask(ssc_ticker);
    n = 0;
  }

  for (uint8_t i = 0; i < 32; i++)
  {
    sscDisp.setColumn(i, sData.getData(i + n));
  }
  sscDisp.show();

  if (n++ >= sData.getDataLenght() - 32)
  {
    sscTasks.stopTask(ssc_ticker);
    sData.stringFree();
  }
}

#endif

#ifdef USE_CALENDAR
void sscSetDayOfWeakString(uint8_t offset, uint8_t dow, bool toStringData)
{
  for (uint8_t j = 0; j < 3; j++)
  {
    sscSetChar(offset + j * 7,
               pgm_read_byte(&day_of_week[dow * 3 + j]), 5, toStringData);
  }
}

void sscSetYearString(uint8_t offset, uint16_t _year, bool toStringData)
{
  sscSetNumString(offset, 20, 6, 2, toStringData);
  sscSetNumString(offset + 16, _year % 2000, 6, 2, toStringData);
}
#endif

#ifdef USE_TEMP_DATA
void sscSetTempString(uint8_t offset, int16_t temp, bool toStringData)
{
  // если температура выходит за диапазон, сформировать строку минусов
  if (temp > 99 || temp < -99)
  {
    for (uint8_t i = 0; i < 4; i++)
    {
      sscSetChar(offset + 2 + i * 7, 0x2D, 5, toStringData);
    }
  }
  else
  {
    bool plus = temp > 0;
    uint8_t plus_pos = offset + 6;
    if (temp < 0)
    {
      temp = -temp;
    }
    sscSetChar(offset + 13, temp % 10, 6, toStringData);
    if (temp > 9)
    {
      // если температура двухзначная, переместить знак на позицию левее
      plus_pos = offset;
      sscSetChar(offset + 6, temp / 10, 6, toStringData);
    }
    // сформировать впереди плюс или минус
    if (temp != 0)
    {
      (plus) ? sscSetChar(plus_pos, 0x2B, 5, toStringData)
             : sscSetChar(plus_pos, 0x2D, 5, toStringData);
    }
    // сформировать в конце знак градуса Цельсия
    sscSetChar(offset + 20, 0xB0, 5, toStringData);
    sscSetChar(offset + 25, 0x43, 5, toStringData);
  }
}

#endif

#if USE_AUTO_SHOW_DATA
void sscAutoShowData()
{
  static uint8_t n = 0;
  static uint8_t n_max = 0;
  static uint32_t timer = 0;

  if (!sscTasks.getTaskState(ssc_auto_show_mode))
  {
    switch (ssc_display_mode)
    {
      n = 0;
#ifdef USE_CALENDAR
    case DISPLAY_MODE_SHOW_DATE:
      n = 0;
      n_max = 3;
      break;
#endif
#ifdef USE_TEMP_DATA
    case DISPLAY_MODE_SHOW_TEMP:
      n = 3;
      n_max = 4;
      break;
#endif
    case DISPLAY_AUTO_SHOW_DATA:
#ifdef USE_CALENDAR
      n = 0;
      n_max = 3;
#else
      n = 3;
#endif
#ifdef USE_TEMP_DATA
      n_max = 4;
#endif
      break;
    default:
      break;
    }
    sscTasks.startTask(ssc_auto_show_mode);
  }

#ifdef USE_TICKER_FOR_DATA
  if (sscTasks.getTaskState(ssc_ticker))
  {
    timer = millis();
    return;
  }
#endif

  if (millis() - timer >= 1000)
  {
    timer = millis();
#ifdef USE_TICKER_FOR_DATA
    if (!read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
    {
      sscDisp.clear();
    }
#else
    sscDisp.clear();
#endif
    switch (n)
    {
#ifdef USE_CALENDAR
    case 0:
#ifdef USE_TICKER_FOR_DATA
      if (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
      {
        sscAssembleString(DISPLAY_MODE_SHOW_DOW);
      }
      else
#endif
      {
        sscSetDayOfWeakString(7, getDayOfWeek(sscClock.getCurTime().day(),
                                              sscClock.getCurTime().month(),
                                              sscClock.getCurTime().year()));
      }
      break;
    case 1:
#ifdef USE_TICKER_FOR_DATA
      if (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
      {
        sscAssembleString(DISPLAY_MODE_SHOW_DAY_AND_MONTH);
      }
      else
#endif
      {
        sscSetTimeString(1, sscClock.getCurTime().day(), sscClock.getCurTime().month(), true, true, false);
      }
      break;
    case 2:
#ifdef USE_TICKER_FOR_DATA
      if (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
      {
        sscAssembleString(DISPLAY_MODE_SHOW_YEAR);
      }
      else
#endif
      {
        sscSetYearString(1, sscClock.getCurTime().year());
      }
      break;
#endif
#ifdef USE_TEMP_DATA
    case 3:
#ifdef USE_TICKER_FOR_DATA
      if (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
      {
        sscAssembleString(DISPLAY_MODE_SHOW_TEMP);
      }
      else
#endif
      {
        sscSetTempString(1, sscGetCurTemp());
      }
      break;
#endif
    }
#ifdef USE_TICKER_FOR_DATA
    if (!read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
    {
      sscDisp.show();
    }
#else
    sscDisp.show();
#endif

    if (++n > n_max)
    {
      sscReturnToDefMode();
    }
  }
}

void sscSetAutoShowPeriodTag(uint8_t offset, bool toStringData)
{
  sscSetTag(offset, DISP_DATE_DISPLAY_INTERVAL_TAG, 5, toStringData);
}

void sscShowAutoShowPeriodData(uint8_t _data, bool blink)
{
  sscDisp.clear();

  sscSetOtherDataString(AUTO_SHOW_PERIOD_TAG, 0, sscGetPeriodForAutoShow(_data), blink);
}

uint8_t sscGetPeriodForAutoShow(uint8_t index)
{
  uint8_t result = index;
  if (index > 7)
  {
    result = 1;
  }
  else if (index > 1)
  {
    index--;
    result = (index < 5) ? index * 5
                         : ((index == 5) ? 30 : 60);
  }
  // таблица соответствий
  // index:  0  1  2  3  4  5  6  7
  // return: 0  1  5 10 15 20 30 60
  return (result);
}
#endif

#ifdef USE_SET_BRIGHTNESS_MODE
void sscSetBrightnessTag(uint8_t offset, bool toStringData)
{
  sscSetTag(offset, DISP_BRIGHTNESS_TAG, 5, toStringData);
#ifdef USE_LIGHT_SENSOR
  uint8_t x = (ssc_display_mode == DISPLAY_MODE_SET_BRIGHTNESS_MIN) ? 0x30 : 0x31;
  sscSetChar(offset + 12, x, 5, toStringData);
#endif
}

void sscShowBrightnessData(uint8_t br, bool blink, bool toSensor, bool toMin)
{
  sscDisp.clear();

  sscSetOtherDataString(SET_BRIGHTNESS_TAG, 1, br, blink);
}
#endif

#ifdef USE_LIGHT_SENSOR
void sscSetLightThresholdTag(uint8_t offset, bool toStringData)
{
  sscSetTag(offset, DISP_LIGHT_THRESHOLD_TAG, 5, toStringData);
}
#endif

#endif