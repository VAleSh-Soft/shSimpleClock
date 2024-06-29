#pragma once

// ==== составные флаги условной компиляции ==========

// флаг работы с esp8266 или esp32
#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
#define __USE_ARDUINO_ESP__ 1
#else
#define __USE_ARDUINO_ESP__ 0
#endif

// используется или нет вывод температуры; вывод температуры доступен, если:
//   - задан флаг USE_TEMP_DATA и:
//     - либо задано использование одного из датчиков и пин для датчика,
//     - либо используется RTC модуль DS3231
#if defined(USE_TEMP_DATA) && ((defined(USE_DS18B20) && DS18B20_PIN >= 0) || \
                               (defined(USE_NTC) && NTC_PIN >= 0) ||         \
                               defined(RTC_DS3231))
#define __USE_TEMP_DATA__ 1
#else
#define __USE_TEMP_DATA__ 0
#endif

// используется или нет периодический автовывод даты и/или температуры
#if defined(USE_CALENDAR) || __USE_TEMP_DATA__
#define __USE_AUTO_SHOW_DATA__ 1
#else
#define __USE_AUTO_SHOW_DATA__ 0
#endif

// регулировка яркости не доступна для экранов LCD 1602/2004
#if defined(USE_SET_BRIGHTNESS_MODE) && !defined(LCD_I2C_DISPLAY)
#define __USE_SET_BRIGHTNESS_MODE__ 1
#else
#define __USE_SET_BRIGHTNESS_MODE__ 0
#endif

// датчик освещенности не используется для экранов LCD 1602/2004
#if defined(USE_LIGHT_SENSOR) && !defined(LCD_I2C_DISPLAY)
#define __USE_LIGHT_SENSOR__ 1
#else
#define __USE_LIGHT_SENSOR__ 0
#endif

// дополнительные настройки; здесь настраиваются:
//   - уровни яркости;
//   - порог переключения яркости;
//   - период автовывода даты и/или температуры;
//   - выбор цвета символов для адресных светодиодов;
#if __USE_SET_BRIGHTNESS_MODE__ || defined(USE_LIGHT_SENSOR) || \
    defined(WS2812_MATRIX_DISPLAY) || __USE_AUTO_SHOW_DATA__
#define __USE_OTHER_SETTING__ 1
#else
#define __USE_OTHER_SETTING__ 0
#endif

// используются матричные экраны
#if defined(MAX72XX_MATRIX_DISPLAY) || defined(WS2812_MATRIX_DISPLAY)
#define __USE_MATRIX_DISPLAY__ 1
#else
#define __USE_MATRIX_DISPLAY__ 0
#endif

// используются опции с параметрами вкл/откл:
//   - включение/отключение будильника
//   - включение/отключение анимации
//   - включение/отключение отображения секундного столбика
#if defined(USE_ALARM) || defined(USE_TICKER_FOR_DATA) || defined(SHOW_SECOND_COLUMN)
#define __USE_ON_OFF_DATA__ 1
#else
#define __USE_ON_OFF_DATA__ 0
#endif

// ===================================================

#include <Arduino.h>
#include <Wire.h>
#if defined(ARDUINO_ARCH_ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif
#include "_eeprom.h"
#include "shSimpleRTC.h"
#include "clkTaskManager.h"
#include "clkButtons.h"

// ===================================================

enum clkDataType : uint8_t
{
  NO_TAG
#if defined(USE_TICKER_FOR_DATA)
  ,
  SET_TICKER_TAG
#endif
#if defined(USE_ALARM)
  ,
  SET_ALARM_TAG
#endif
#if __USE_AUTO_SHOW_DATA__
  ,
  SET_AUTO_SHOW_PERIOD_TAG
#endif
#if __USE_SET_BRIGHTNESS_MODE__
  ,
  SET_BRIGHTNESS_TAG
#endif
#if __USE_LIGHT_SENSOR__
  ,
  SET_LIGHT_THRESHOLD_TAG
#endif
#if defined(WS2812_MATRIX_DISPLAY)
  ,
  SET_COLOR_OF_NUMBER_TAG
#endif
#if defined(SHOW_SECOND_COLUMN)
  ,
  SET_SECOND_COLUMN_TAG
#endif
};

enum clkDisplayMode : uint8_t
{
  DISPLAY_MODE_SHOW_TIME, // основной режим - вывод времени на индикатор
  DISPLAY_MODE_SET_HOUR,  // режим настройки часов
  DISPLAY_MODE_SET_MINUTE // режим настройки минут
#if defined(USE_CALENDAR)
  ,
  DISPLAY_MODE_SET_DAY,   // режим настройки дня месяца
  DISPLAY_MODE_SET_MONTH, // режим настройки месяца
  DISPLAY_MODE_SET_YEAR   // режим настройки года
#endif
#if defined(USE_ALARM)
  ,
  DISPLAY_MODE_ALARM_ON_OFF,    // режим настройки будильника - вкл/выкл
  DISPLAY_MODE_SET_ALARM_HOUR,  // режим настройки будильника - часы
  DISPLAY_MODE_SET_ALARM_MINUTE // режим настройки будильника - минуты
#endif
#if __USE_TEMP_DATA__
  ,
  DISPLAY_MODE_SHOW_TEMP // режим вывода температуры
#endif
#if __USE_AUTO_SHOW_DATA__
  ,
  DISPLAY_AUTO_SHOW_DATA // автовывод даты и/или температуры в конце каждой минуты
#endif
#if defined(USE_CALENDAR)
  ,
  DISPLAY_MODE_SHOW_DATE,          // вывод полной даты
  DISPLAY_MODE_SHOW_DOW,           // режим вывода дня недели
  DISPLAY_MODE_SHOW_DAY_AND_MONTH, // режим вывода числа и месяца
  DISPLAY_MODE_SHOW_YEAR           // режим вывода года
#endif
#if __USE_LIGHT_SENSOR__
  ,
  DISPLAY_MODE_SET_LIGHT_THRESHOLD // режим настройки порога переключения яркости
#endif
#if __USE_SET_BRIGHTNESS_MODE__
#if __USE_LIGHT_SENSOR__
  ,
  DISPLAY_MODE_SET_BRIGHTNESS_MIN // режим настройки минимального уровня яркости экрана
#endif
  ,
  DISPLAY_MODE_SET_BRIGHTNESS_MAX // режим настройки максимального уровня яркости экрана
#endif
#if defined(USE_TICKER_FOR_DATA)
  ,
  DISPLAY_MODE_SET_TICKER_ON_OFF // режим включения/выключения анимации
#endif
#if __USE_AUTO_SHOW_DATA__
  ,
  DISPLAY_MODE_SET_AUTO_SHOW_PERIOD // режим настройки периода автовывода даты и/или температуры
#endif
#if defined(WS2812_MATRIX_DISPLAY)
  ,
  DISPLAY_MODE_SET_COLOR_OF_NUMBER // режим настройки цвета символов для адресных светодиодов
#endif
#if defined(SHOW_SECOND_COLUMN)
  ,
  DISPLAY_MODE_SET_SECOND_COLUMN_ON_OFF // режим включения/выключения отображения секундного столбика
#endif
  ,
  DISPLAY_MODE_CUSTOM_1,
  DISPLAY_MODE_CUSTOM_2,
  DISPLAY_MODE_CUSTOM_3,
  DISPLAY_MODE_CUSTOM_4
};

// ===================================================

void sscRtcNow();
void sscBlink();
void sscReturnToDefMode();
void sscShowTimeData(int8_t hour, int8_t minute);
#if defined(SHOW_SECOND_COLUMN)
void sscShowSecondColumn(int8_t second);
#endif
void sscShowTimeSetting();
void sscShowDisplay();
void sscCheckButton();
void sscSetDisplayMode();
#if defined(USE_ALARM)
void sscCheckAlarm();
void sscRunAlarmBuzzer();
#endif
#if __USE_LIGHT_SENSOR__
void sscSetBrightness();
#endif
#if __USE_TEMP_DATA__
int8_t sscGetCurTemp();
#if defined(USE_DS18B20)
void sscCheckDS18b20();
#endif
#endif
void sscClearButtonFlag();
void sscStopSetting(clkHandle task);
#if __USE_OTHER_SETTING__
void sscShowOtherSetting();
#endif
#if __USE_AUTO_SHOW_DATA__
void sscAutoShowData();
uint8_t sscGetPeriodForAutoShow(uint8_t index);
#endif
#if __USE_ON_OFF_DATA__
void sscShowOnOffData(clkDataType _type, bool _state, bool blink);
#endif

#if __USE_MATRIX_DISPLAY__

void sscSetTimeString(uint8_t offset, int8_t hour, int8_t minute, bool show_colon,
                      bool toDate, bool toStringData = false);
void sscSetOtherDataString(clkDataType _type, uint8_t offset, uint8_t _data, bool blink,
                           bool toStringData = false);
void sscSetTag(uint8_t offset, uint8_t index, uint8_t width, bool toStringData);
void sscSetOnOffDataString(clkDataType _type, uint8_t offset, bool _state, bool _blink,
                           bool toStringData = false);
void sscSetNumString(uint8_t offset, uint8_t num,
                     uint8_t width = 6, uint8_t space = 1, bool toStringData = false, bool firstSpace = false);
void sscSetChar(uint8_t offset, uint8_t chr, uint8_t width, bool toStringData = false);

#if defined(USE_TICKER_FOR_DATA)
void sscAssembleString(clkDisplayMode data_type, uint8_t lenght = 80);
void sscRunTicker();
#endif

#if defined(USE_CALENDAR)
void sscSetDayOfWeakString(uint8_t offset, uint8_t dow, bool toStringData = false);
void sscSetYearString(uint8_t offset, int16_t _year, bool toStringData = false);
#endif

#if __USE_TEMP_DATA__
void sscSetTempString(uint8_t offset, int16_t temp, bool toStringData = false);
#endif

#if defined(WS2812_MATRIX_DISPLAY)
uint8_t sscGetIndexOfCurrentColorOfNumber();
#endif

#else

void sscSetOtherData(clkDataType _type, uint8_t _data, bool blink);
void sscSetTag(clkDataType _type);
void sscSetOnOffData(clkDataType _type, bool _state, bool _blink);
void sscShowTime(int8_t hour, int8_t minute, bool show_colon);
#if defined(USE_CALENDAR)
void sscShowDate(shDateTime date);
#endif
#if __USE_TEMP_DATA__
void sscShowTemp(int temp);
#endif
#endif

// ===================================================

bool sscBlinkFlag = false; // флаг блинка, используется всем, что должно мигать

clkDisplayMode ssc_display_mode = DISPLAY_MODE_SHOW_TIME;

// ===================================================

#if defined(TM1637_DISPLAY)
#include "display_TM1637.h"
#elif defined(LCD_I2C_DISPLAY)
#include "display_LCD_I2C.h"
#elif defined(MAX72XX_7SEGMENT_DISPLAY) || defined(MAX72XX_MATRIX_DISPLAY)
#include "display_MAX72xx.h"
#elif defined(WS2812_MATRIX_DISPLAY)
#include <FastLED.h> // https://github.com/FastLED/FastLED
#include "display_WS2812.h"
#else
#error "Unknown display specified. Set the supported display in clockSetting.h"
#endif
#if defined(USE_ALARM)
#include "clkAlarmClass.h"
#endif
#if __USE_TEMP_DATA__
#if defined(USE_DS18B20)
#if defined(ARDUINO_ARCH_RP2040)
#include "ds1820_new.h"
#else
#include "ds1820.h"
#endif
#elif defined(USE_NTC)
#include "ntc.h"
#endif
#endif
#if defined USE_CLOCK_EVENT
#include "shClockEvent.h"
#endif

// ===================================================

#if __USE_LIGHT_SENSOR__
uint16_t light_threshold_step = 102;

void sscGetLightThresholdStep(uint8_t _adc_bit_depth)
{
  uint16_t x = 1;

  for (uint8_t i = 0; i < _adc_bit_depth; i++)
  {
    x *= 2;
  }

  light_threshold_step = x / 10;
}
#endif

// ==== shSimpleClock ================================

class shSimpleClock
{
private:
  void rtc_init();

  void eeprom_validate();

  void sensor_init();

  void display_init();

  void task_list_init();

public:
  /**
   * @brief конструктор объекта часов
   *
   */
  shSimpleClock();

#if __USE_LIGHT_SENSOR__ || defined(USE_NTC)
  /**
   * @brief задать действующее разрешение АЦП микроконтроллера;
   *
   * @param bit_depth разрешение; для AVR - обычно 10, для ESP - 12
   */
  void setADCbitDepth(uint8_t bit_depth);
#endif

  /**
   * @brief инициализация часов
   *
   */
  void init();

  /**
   * @brief обработка событий часов
   *
   */
  void tick();

  /**
   * @brief получить текущий режим экрана часов
   *
   * @return clkDisplayMode
   */
  clkDisplayMode getDisplayMode();

  /**
   * @brief установить режим экрана часов
   *
   * @param _mode режим для установки
   */
  void setDisplayMode(clkDisplayMode _mode);

#if defined(LCD_I2C_DISPLAY)
  /**
   * @brief включить или выключить подсветку экрана
   *
   * @param _state true - включить, false - выключить
   */
  void setBacklightState(bool _state);

  /**
   * @brief вывести произвольный текст на экран
   *
   * @param _col позиция курсора для первого символа текста
   * @param _line номер строки для вывода
   * @param _str текст для вывода
   */
  void printTextForScreen(uint8_t _col, uint8_t _line, const char *_str);
#endif

#if defined USE_CLOCK_EVENT
  /**
   * @brief подключить callback-функцию к ежесекундному событию
   *
   * @param _interval интервал вызова функции в секундах
   * @param _callback вызываемая функция
   * @param _active статус события - активно/не активно
   */
  void setClockEvent(uint16_t _interval,
                     sceCallback _callback,
                     bool _active = true);

  /**
   * @brief установить статус ежесекундного события
   *
   * @param _state true - событие активно; false - событие не активно
   */
  void setClockEventState(bool _state);

  /**
   * @brief получить текущий статус ежесекундного события
   *
   * @return true
   * @return false
   */
  bool getClockEventState();
#endif

  /**
   * @brief получить текущее состояние или событие кнопки
   *
   * @param _btn идентификатор кнопки, может иметь значение: CLK_BTN_SET, CLK_BTN_UP, CLK_BTN_DOWN;
   * @return uint8_t
   */
  uint8_t getButtonState(clkButtonType _btn);

  /**
   * @brief получить флаг кнопки
   *
   * @param _btn идентификатор кнопки, может иметь значение: CLK_BTN_SET, CLK_BTN_UP, CLK_BTN_DOWN;
   * @param _clear если true, то флаг кнопки после считывания будет очищен (установлено значение CLK_BTN_FLAG_NONE);
   * @return clkButtonFlag возможные варианты: CLK_BTN_FLAG_NONE, CLK_BTN_FLAG_NEXT, CLK_BTN_FLAG_EXIT
   */
  clkButtonFlag getButtonFlag(clkButtonType _btn, bool _clear = false);

  /**
   * @brief установить флаг кнопки
   *
   * @param _btn  идентификатор кнопки, может иметь значение: CLK_BTN_SET, CLK_BTN_UP, CLK_BTN_DOWN;
   * @param _flag устанавливаемый флаг; возможные варианты: CLK_BTN_FLAG_NONE, CLK_BTN_FLAG_NEXT, CLK_BTN_FLAG_EXIT
   */
  void setButtonFlag(clkButtonType _btn, clkButtonFlag _flag);

#if defined(MAX72XX_MATRIX_DISPLAY)
  /**
   * @brief установить угол поворота изображения
   *
   * @param _dir угол поворота изображения, 0..3
   */
  void setMatrixDirection(uint8_t _dir);

  /**
   * @brief включить отражение изображения по горизонтали (по строкам)
   *
   * @param _mode true - включить отражение, false - отключить отражение
   */
  void setMatrixFlipMode(bool _mode);
#endif

#if defined(WS2812_MATRIX_DISPLAY)
  /**
   * @brief установить цвет для вывода информации на матрице из адресных светодиодов
   *
   * @param _color цвет, например, для красного CRGB::Red
   */
  void setColorOfNumber(CRGB _color);

  /**
   * @brief получение текущего цвета символов экрана из адресных светдиодов
   *
   * @return CRGB
   */
  CRGB getColorOfNumber();

  /**
   * @brief установить цвет фона на матрице из адресных светодиодов
   *
   * @param _color цвет, например, для синего CRGB::Blue
   */
  void setColorOfBackground(CRGB _color);

  /**
   * @brief получение текущего цвета фона экрана из адресных светдиодов
   *
   * @return CRGB
   */
  CRGB getColorOfBackground();

  /**
   * @brief установка максимальной мощности блока питания матрицы
   *
   * @param volts напряжение, Вольт
   * @param milliamps максимальный ток, милиампер
   */
  void setMaxPSP(uint8_t volts, uint32_t milliamps);
#endif

  /**
   * @brief получение текущих даты и времени
   *
   * @return shDateTime
   */
  shDateTime getCurrentDateTime();

  /**
   * @brief установка текущего времени
   *
   * @param _hour часы для установки
   * @param _minute минуты для установки
   * @param _second секунды для установки
   */
  void setCurrentTime(uint8_t _hour, uint8_t _minute, uint8_t _second);

#if defined(USE_CALENDAR)
  /**
   * @brief установка текущей даты
   *
   * @param _date число месяца для установки
   * @param _month номер месяца для установки (1..2)
   */
  void setCurrentDate(uint8_t _date, uint8_t _month);

  /**
   * @brief установка текущего года
   *
   * @param _year год для установки (0..99 )
   */
  void setCurrentYear(uint8_t _year);
#endif

#if __USE_TEMP_DATA__
  /**
   * @brief получить текущую температуру, если определен используемый датчик
   *
   * @return int8_t
   */
  int8_t getTemperature();
#endif

#if defined(USE_ALARM)

#if defined USE_CLOCK_EVENT
  /**
   * @brief подключить callback-функцию к событию будильника
   *
   * @param _callback вызываемая функция
   * @param _active статус события - активно/не активно
   */
  void setAlarmEvent(sceCallback _callback, bool _active = true);

  /**
   * @brief установить статус события будильника
   *
   * @param _state true - событие активно; flase - событие не активно
   */
  void setAlarmEventState(bool _state);

  /**
   * @brief получить текущий статус события будильника
   *
   * @return true
   * @return false
   */
  bool getAlarmEventState();
#endif

  /**
   * @brief получение времени срабатывания будильника
   *
   * @return uint16_t количество минут с полуночи
   */
  uint16_t getAlarmPoint();

  /**
   * @brief установка времени срабатывания будильника
   *
   * @param _point количество минут с полуночи
   */
  void setAlarmPoint(uint16_t _point);

  /**
   * @brief установка времени срабатывания будильника
   *
   * @param _hour час
   * @param _minute минута
   */
  void setAlarmPoint(uint8_t _hour, uint8_t _minute);

  /**
   * @brief получение состояния будильника - включен или выключен
   *
   * @return true - включен;
   * @return false - выключен
   */
  bool getOnOffAlarm();

  /**
   * @brief установка состояния будильника - включить или выключить
   *
   * @param _state true - включен, false - выключен
   */
  void setOnOffAlarm(bool _state);

  /**
   * @brief получение статуса будильника, позволяет отслеживать срабатывание будильника
   *
   * @return AlarmState 0 - будильник выключен, 1 - будильник включен, 2 - будильник сработал
   */
  AlarmState getAlarmState();

  /**
   * @brief отключение сигнала сработавшего будильника
   *
   */
  void buzzerStop();
#endif

#if __USE_SET_BRIGHTNESS_MODE__
  /**
   * @brief получение значения максимальной яркости экрана
   *
   * @return uint8_t
   */
  uint8_t getBrightnessMax();

  /**
   * @brief установка максимальной яркости экрана
   *
   * @param _br новое значение
   */
  void setBrightnessMax(uint8_t _br);

#if __USE_LIGHT_SENSOR__
  /**
   * @brief получение значения минимальной яркости экрана
   *
   * @return uint8_t
   */
  uint8_t getBrightnessMin();

  /**
   * @brief установка минимальной яркости экрана
   *
   * @param _br
   */
  void setBrightnessMin(uint8_t _br);
#endif
#endif

#if __USE_LIGHT_SENSOR__
  /**
   * @brief получение текущего порога переключения яркости
   *
   * @return uint8_t интервал 1..9
   */
  uint8_t getLightThresholdValue();

  /**
   * @brief установка нового порога переключения яркости
   *
   * @param _data новое значение, интервал 1..9
   */
  void setLightThresholdValue(uint8_t _data);
#endif

#if __USE_MATRIX_DISPLAY__

#if defined(USE_TICKER_FOR_DATA)
  /**
   * @brief включение/выключение анимации
   *
   * @param _state true -включена, false - отключена
   */
  void setAnimationState(bool _state);

  /**
   * @brief получение текущего состояния анимации
   *
   * @return true включена
   * @return false отключена
   */
  bool getAnimationState();
#endif

#if __USE_AUTO_SHOW_DATA__
  /**
   * @brief установка интервала автовывода информации
   *
   * @param _index интервал задается в пределах 0..7, при этом соответствие индексов интервалу следующее: 0 -> 0, 1 -> 1, 2 -> 5, 3 -> 10, 4 -> 15, 5 -> 20, 6 -> 30, 7 -> 60
   */
  void setIntervalForAutoShowData(uint8_t _index);

  /**
   * @brief получение текущего интервала автовывода информации
   *
   * @return uint8_t 0..7, при этом соответствие индексов интервалу следующее: 0 -> 0, 1 -> 1, 2 -> 5, 3 -> 10, 4 -> 15, 5 -> 20, 6 -> 30, 7 -> 60
   */
  uint8_t getIntervalForAutoShowData();
#endif

#if defined(SHOW_SECOND_COLUMN)
  /**
   * @brief установка  статуса секундного столбика - включен или выключен
   *
   * @param _state true - отображается, false - отключен
   */
  void setSecondColumnState(bool _state);

  /**
   * @brief получение статуса секундного столбика - включен или выключен
   *
   * @return true отображается
   * @return false отключен
   */
  bool getSecondColumnState();
#endif

#endif
};

// ---- shSimpleClock private -------------------

void shSimpleClock::rtc_init()
{
#if defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_STM32)
  Wire.setSDA(RTC_SDA_PIN);
  Wire.setSCL(RTC_SCL_PIN);
#elif defined(ARDUINO_ARCH_ESP32)
  Wire.setPins(RTC_SDA_PIN, RTC_SCL_PIN);
#endif
  Wire.begin();

  // если часовой модуль не запущен, запускаем его, для чего нужно установить время
  if (!clkClock.isRunning())
  {
#if defined(RTC_DS3231)
    // устанавливаем режим 24 часа
    clkClock.setClockMode(false);
#endif

    clkClock.setCurTime(0, 0, 0);
    clkClock.setCurDate(1, 1);
    clkClock.setCurYear(0);
  }
#if !defined(RTC_DS1307)
  clkClock.startRTC();
#endif

  sscRtcNow();
}

void shSimpleClock::eeprom_validate()
{
#if defined(SHOW_SECOND_COLUMN)
  if (read_eeprom_8(SECOND_COLUMN_ON_OF_DATA_EEPROM_INDEX) > 1)
  {
    write_eeprom_8(SECOND_COLUMN_ON_OF_DATA_EEPROM_INDEX, 0);
  }
#endif

#if __USE_AUTO_SHOW_DATA__
  if (read_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX) > 7)
  {
    write_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX, 1);
  }
#endif

#if __USE_LIGHT_SENSOR__
  uint8_t l = read_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX);
  if (l > 9 || l == 0)
  {
    write_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX, 3);
  }
#endif

#if defined(WS2812_MATRIX_DISPLAY)
  CRGB c;
  read_eeprom_crgb(COLOR_OF_NUMBER_VALUE_EEPROM_INDEX, c);
  // если задан черный цвет или не записана ячейка обновления, задать цвет вимволов по умолчанию
  if ((c.r == 0x00 && c.g == 0x00 && c.b == 0x00) ||
      read_eeprom_8(COLOR_OF_NUMBER_VALUE_EEPROM_INDEX) != CRGB_UPDATE_DATA)
  {
    c = COLOR_OF_NUMBER;
    write_eeprom_crgb(COLOR_OF_NUMBER_VALUE_EEPROM_INDEX, c);
  }
  setColorOfNumber(c);

  read_eeprom_crgb(COLOR_OF_BACKGROUND_VALUE_EEPROM_INDEX, c);
  // если не записана ячейка обновления, задать цвет фона по умолчанию
  if (read_eeprom_8(COLOR_OF_BACKGROUND_VALUE_EEPROM_INDEX) != CRGB_UPDATE_DATA)
  {
    c = COLOR_OF_BACKGROUND;
    write_eeprom_crgb(COLOR_OF_BACKGROUND_VALUE_EEPROM_INDEX, COLOR_OF_BACKGROUND);
  }
  setColorOfBackground(c);
#endif
}

void shSimpleClock::sensor_init()
{
#if defined(USE_DS18B20)
  sscTempSensor.init(DS18B20_PIN);
#elif defined(USE_NTC)
  sscTempSensor.setADCbitDepth(BIT_DEPTH); // установить разрядность АЦП вашего МК, для AVR обычно равна 10 бит
#endif
#if !defined(LCD_I2C_DISPLAY)
#if __USE_LIGHT_SENSOR__
#if defined(__STM32F1__) || defined(__STM32F4__)
  pinMode((uint8_t)LIGHT_SENSOR_PIN, INPUT_ANALOG);
#endif
  sscGetLightThresholdStep(BIT_DEPTH);
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
#if __USE_LIGHT_SENSOR__
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
  clkDisplay.setBrightness(read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX));
#endif
#endif
}

void shSimpleClock::display_init()
{
#if defined(WS2812_MATRIX_DISPLAY)
  clkDisplay.init();
#elif defined(MAX72XX_MATRIX_DISPLAY) || defined(MAX72XX_7SEGMENT_DISPLAY)
  clkDisplay.init();
  clkDisplay.shutdownAllDevices(false);
#if defined(MAX72XX_MATRIX_DISPLAY)
  clkDisplay.setDirection(2);
  clkDisplay.setFlip(false);
#endif
#elif defined(LCD_I2C_DISPLAY)
  clkDisplay.init();
#endif

// выставить яркость в минимум, чтобы при включении не сверкало максимальной яркостью
#if defined(WS2812_MATRIX_DISPLAY)
  FastLED.setBrightness(0);
#elif defined(MAX72XX_MATRIX_DISPLAY) || defined(MAX72XX_7SEGMENT_DISPLAY)
  clkDisplay.setBrightness(0);
#elif defined(TM1637_DISPLAY)
  clkDisplay.setBrightness(1);
#endif
}

void shSimpleClock::task_list_init()
{
  uint8_t task_count = 5; // базовое количество задач
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
  clkTasks.init(task_count);

  clkTasks.rtc_guard = clkTasks.addTask(50ul, sscRtcNow);
  clkTasks.blink_timer = clkTasks.addTask(50ul, sscBlink);
  clkTasks.return_to_default_mode = clkTasks.addTask(AUTO_EXIT_TIMEOUT * 1000ul,
                                                     sscReturnToDefMode,
                                                     false);
  clkTasks.set_time_mode = clkTasks.addTask(50ul, sscShowTimeSetting, false);
#if __USE_TEMP_DATA__ && defined(USE_DS18B20)
  clkTasks.ds18b20_guard = clkTasks.addTask(3000ul, sscCheckDS18b20);
#endif
#if __USE_AUTO_SHOW_DATA__
  clkTasks.auto_show_mode = clkTasks.addTask(100ul, sscAutoShowData, false);
#endif
#if defined(USE_ALARM)
  clkTasks.alarm_guard = clkTasks.addTask(200ul, sscCheckAlarm);
  clkTasks.alarm_buzzer = clkTasks.addTask(50ul, sscRunAlarmBuzzer, false);
#endif
  clkTasks.display_guard = clkTasks.addTask(50ul, sscShowDisplay);
#if __USE_LIGHT_SENSOR__
  clkTasks.light_sensor_guard = clkTasks.addTask(100ul, sscSetBrightness);
#elif !defined(LCD_I2C_DISPLAY)
  clkDisplay.setBrightness(read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX));
#endif
#if __USE_OTHER_SETTING__
  clkTasks.other_setting_mode = clkTasks.addTask(50ul, sscShowOtherSetting, false);
#endif
#if defined(USE_TICKER_FOR_DATA)
  clkTasks.ticker = clkTasks.addTask(1000ul / TICKER_SPEED, sscRunTicker, false);
#endif
}

// ---- shSimpleClock public --------------------

shSimpleClock::shSimpleClock() {}

#if __USE_LIGHT_SENSOR__ || defined(USE_NTC)
void shSimpleClock::setADCbitDepth(uint8_t bit_depth)
{
#if defined(USE_NTC)
  sscTempSensor.setADCbitDepth(bit_depth);
#endif
#if __USE_LIGHT_SENSOR__
  sscGetLightThresholdStep(bit_depth);
#endif
}
#endif

void shSimpleClock::init()
{
#if defined(USE_ALARM)
  clkAlarm.init();
#endif

  // ==== кнопки ===================================
  clkButtons.init();

  // ==== валидация EEPROM =========================
  eeprom_validate();

  // ==== датчики ==================================
  sensor_init();

  // ==== RTC ======================================
  rtc_init();

  // ==== экраны ===================================
  display_init();

  // ==== задачи ===================================
  task_list_init();
}

void shSimpleClock::tick()
{
  sscCheckButton();
  clkTasks.tick();
  sscSetDisplayMode();
}

clkDisplayMode shSimpleClock::getDisplayMode() { return ssc_display_mode; }

void shSimpleClock::setDisplayMode(clkDisplayMode _mode) { ssc_display_mode = _mode; }

#if defined(LCD_I2C_DISPLAY)
void shSimpleClock::setBacklightState(bool _state)
{
  clkDisplay.setBacklightState(_state);
}

void shSimpleClock::printTextForScreen(uint8_t _col, uint8_t _line, const char *_str)
{
  sscLcdDisplay.setCursor(_col, _line);
  sscLcdDisplay.print(_str);
}
#endif

#if defined USE_CLOCK_EVENT
void shSimpleClock::setClockEvent(uint16_t _interval,
                                  sceCallback _callback,
                                  bool _active = true)
{
  sscClockEvent.init(_interval, _callback, _active);
}

void shSimpleClock::setClockEventState(bool _state) { sscClockEvent.setState(_state); }

bool shSimpleClock::getClockEventState() { return sscClockEvent.getState(); }
#endif

uint8_t shSimpleClock::getButtonState(clkButtonType _btn)
{
  return (clkButtons.getLastState(_btn));
}

clkButtonFlag shSimpleClock::getButtonFlag(clkButtonType _btn, bool _clear)
{
  return clkButtons.getButtonFlag(_btn, _clear);
}

void shSimpleClock::setButtonFlag(clkButtonType _btn, clkButtonFlag _flag)
{
  clkButtons.setButtonFlag(_btn, _flag);
}

#if defined(MAX72XX_MATRIX_DISPLAY)
void shSimpleClock::setMatrixDirection(uint8_t _dir)
{
  clkDisplay.setDirection(_dir);
}

void shSimpleClock::setMatrixFlipMode(bool _mode)
{
  clkDisplay.setFlip(_mode);
}
#endif

#if defined(WS2812_MATRIX_DISPLAY)
void shSimpleClock::setColorOfNumber(CRGB _color)
{
  clkDisplay.setColorOfNumber(_color);
  write_eeprom_crgb(COLOR_OF_NUMBER_VALUE_EEPROM_INDEX, _color);
}

CRGB shSimpleClock::getColorOfNumber()
{
  return (clkDisplay.getColorOfNumber());
}

void shSimpleClock::setColorOfBackground(CRGB _color)
{
  clkDisplay.setColorOfBackground(_color);
  write_eeprom_crgb(COLOR_OF_BACKGROUND_VALUE_EEPROM_INDEX, _color);
}

CRGB shSimpleClock::getColorOfBackground()
{
  return (clkDisplay.getColorOfBackground());
}

void shSimpleClock::setMaxPSP(uint8_t volts, uint32_t milliamps)
{
  clkDisplay.setMaxPSP(volts, milliamps);
}
#endif

shDateTime shSimpleClock::getCurrentDateTime()
{
  return (clkClock.getCurTime());
}

void shSimpleClock::setCurrentTime(uint8_t _hour, uint8_t _minute, uint8_t _second)
{
#if defined(RTC_DS3231)
  // устанавливаем режим 24 часа
  clkClock.setClockMode(false);
#endif
  clkClock.setCurTime(_hour, _minute, _second);
}

#if defined(USE_CALENDAR)
void shSimpleClock::setCurrentDate(uint8_t _date, uint8_t _month)
{
  clkClock.setCurDate(_date, _month);
}

void shSimpleClock::setCurrentYear(uint8_t _year)
{
  clkClock.setCurYear(_year);
}
#endif

#if __USE_TEMP_DATA__
int8_t shSimpleClock::getTemperature()
{
  return sscGetCurTemp();
}
#endif

#if defined(USE_ALARM)

#if defined USE_CLOCK_EVENT
void shSimpleClock::setAlarmEvent(sceCallback _callback, bool _active = true)
{
  sscAlarmEvent.init(_callback, _active);
}

void shSimpleClock::setAlarmEventState(bool _state) { sscAlarmEvent.setState(_state); }

bool shSimpleClock::getAlarmEventState() { sscAlarmEvent.getState(); }
#endif

uint16_t shSimpleClock::getAlarmPoint() { return (clkAlarm.getAlarmPoint()); }

void shSimpleClock::setAlarmPoint(uint16_t _point) { clkAlarm.setAlarmPoint(_point); }

void shSimpleClock::setAlarmPoint(uint8_t _hour, uint8_t _minute)
{
  clkAlarm.setAlarmPoint(_hour * 60 + _minute);
}

bool shSimpleClock::getOnOffAlarm() { return (clkAlarm.getOnOffAlarm()); }

void shSimpleClock::setOnOffAlarm(bool _state) { clkAlarm.setOnOffAlarm(_state); }

AlarmState shSimpleClock::getAlarmState() { return (clkAlarm.getAlarmState()); }

void shSimpleClock::buzzerStop()
{
  if (clkAlarm.getAlarmState() == ALARM_YES)
  {
    clkAlarm.setAlarmState(ALARM_ON);
  }
}
#endif

#if __USE_SET_BRIGHTNESS_MODE__
uint8_t shSimpleClock::getBrightnessMax()
{
  return (read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX));
}

void shSimpleClock::setBrightnessMax(uint8_t _br)
{
  write_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX, _br);
#if defined(WS2812_MATRIX_DISPLAY)
  _br *= 10;
#endif
  clkDisplay.setBrightness(_br);
}

#if __USE_LIGHT_SENSOR__
uint8_t shSimpleClock::getBrightnessMin()
{
  return (read_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX));
}

void shSimpleClock::setBrightnessMin(uint8_t _br)
{
  write_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX, _br);
#if defined(WS2812_MATRIX_DISPLAY)
  _br *= 10;
#endif
  clkDisplay.setBrightness(_br);
}
#endif
#endif

#if __USE_LIGHT_SENSOR__
uint8_t shSimpleClock::getLightThresholdValue()
{
  return (read_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX));
}

void shSimpleClock::setLightThresholdValue(uint8_t _data)
{
  _data = (_data > 9) ? 9 : (_data == 0) ? 1
                                         : _data;
  write_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX, _data);
}
#endif

#if __USE_MATRIX_DISPLAY__

#if defined(USE_TICKER_FOR_DATA)
void shSimpleClock::setAnimationState(bool _state)
{
  write_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX, (uint8_t)_state);
}

bool shSimpleClock::getAnimationState()
{
  return (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX));
}
#endif

#if __USE_AUTO_SHOW_DATA__
void shSimpleClock::setIntervalForAutoShowData(uint8_t _index)
{
  _index = (_index > 7) ? 1 : _index;
  write_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX, _index);
}

uint8_t shSimpleClock::getIntervalForAutoShowData()
{
  return read_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX);
}
#endif

#if defined(SHOW_SECOND_COLUMN)
void shSimpleClock::setSecondColumnState(bool _state)
{
  write_eeprom_8(SECOND_COLUMN_ON_OF_DATA_EEPROM_INDEX, (uint8_t)_state);
}

bool shSimpleClock::getSecondColumnState()
{
  return ((bool)read_eeprom_8(SECOND_COLUMN_ON_OF_DATA_EEPROM_INDEX));
}
#endif

#endif

// ==== end shSimpleClock ============================

void sscRtcNow()
{

  clkClock.now();
  if (ssc_display_mode == DISPLAY_MODE_SHOW_TIME)
  {
#if defined(USE_TICKER_FOR_DATA)
    // если работает бегущая строка, ничего не делать
    if (clkTasks.getTaskState(clkTasks.ticker))
    {
      return;
    }
#endif

#if __USE_AUTO_SHOW_DATA__
    static bool flag = false;

    if (clkClock.getCurTime().second() > 0)
    {
      flag = false;
    }

    uint8_t x = read_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX);
    if (((sscGetPeriodForAutoShow(x) > 0) &&
         (clkClock.getCurTime().minute() % sscGetPeriodForAutoShow(x) == 0)) &&
        (clkClock.getCurTime().second() == 0) &&
        !flag)
    {
      flag = true;
      ssc_display_mode = DISPLAY_AUTO_SHOW_DATA;
    }
    else
#endif
    {
#if __USE_MATRIX_DISPLAY__
      sscShowTimeData(clkClock.getCurTime().hour(),
                      clkClock.getCurTime().minute());

#if defined(SHOW_SECOND_COLUMN)
      if (ssc_display_mode == DISPLAY_MODE_SHOW_TIME &&
          read_eeprom_8(SECOND_COLUMN_ON_OF_DATA_EEPROM_INDEX))
      {
        sscShowSecondColumn(clkClock.getCurTime().second());
      }
#endif

#else
      sscShowTime(clkClock.getCurTime().hour(),
                  clkClock.getCurTime().minute(),
                  sscBlinkFlag);
#endif
    }
  }
}

void sscBlink()
{
  static uint8_t cur_sec = clkClock.getCurTime().second();
  static unsigned long tmr = 0;
  if (cur_sec != clkClock.getCurTime().second())
  {
#if defined USE_CLOCK_EVENT
    sscClockEvent.run();
#endif
    cur_sec = clkClock.getCurTime().second();
    sscBlinkFlag = false;
    tmr = millis();
  }
  else if (!sscBlinkFlag && millis() - tmr >= 500)
  {
    sscBlinkFlag = true;
  }
}

void sscReturnToDefMode()
{
  switch (ssc_display_mode)
  {
  case DISPLAY_MODE_SET_HOUR:
  case DISPLAY_MODE_SET_MINUTE:
#if defined(USE_CALENDAR)
  case DISPLAY_MODE_SET_DAY:
  case DISPLAY_MODE_SET_MONTH:
  case DISPLAY_MODE_SET_YEAR:
#endif
#if defined(USE_ALARM)
  case DISPLAY_MODE_SET_ALARM_HOUR:
  case DISPLAY_MODE_SET_ALARM_MINUTE:
  case DISPLAY_MODE_ALARM_ON_OFF:
#endif
#if __USE_LIGHT_SENSOR__
  case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
#endif
#if __USE_SET_BRIGHTNESS_MODE__
#if __USE_LIGHT_SENSOR__
  case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
#endif
#if defined(USE_TICKER_FOR_DATA)
  case DISPLAY_MODE_SET_TICKER_ON_OFF:
#endif
#if __USE_AUTO_SHOW_DATA__
  case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
#endif
#if defined(WS2812_MATRIX_DISPLAY)
  case DISPLAY_MODE_SET_COLOR_OF_NUMBER:
#endif
#if defined(SHOW_SECOND_COLUMN)
  case DISPLAY_MODE_SET_SECOND_COLUMN_ON_OFF:
#endif
    clkButtons.setButtonFlag(CLK_BTN_SET, CLK_BTN_FLAG_EXIT);
    break;
#if __USE_TEMP_DATA__
  case DISPLAY_MODE_SHOW_TEMP:
#endif
#if defined(USE_CALENDAR)
  case DISPLAY_MODE_SHOW_DATE:
#endif
#if __USE_AUTO_SHOW_DATA__
  case DISPLAY_AUTO_SHOW_DATA:
    clkTasks.stopTask(clkTasks.auto_show_mode);
#endif
    ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
    break;
  default:
    break;
  }
#if defined(USE_TICKER_FOR_DATA)
  if (ssc_display_mode == DISPLAY_MODE_SHOW_TIME &&
      read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
  {
    sscAssembleString(DISPLAY_MODE_SHOW_TIME);
  }
#endif
  clkTasks.stopTask(clkTasks.return_to_default_mode);
}

void sscShowTimeData(int8_t hour, int8_t minute)
{
  // если наступило время блинка и кнопки Up/Down не нажаты, то стереть соответствующие разряды; при нажатых кнопках Up/Down во время изменения данных ничего не мигает
  if (!sscBlinkFlag &&
      !clkButtons.isButtonClosed(CLK_BTN_UP) &&
      !clkButtons.isButtonClosed(CLK_BTN_DOWN))
  {
    switch (ssc_display_mode)
    {
    case DISPLAY_MODE_SET_HOUR:
#if defined(USE_CALENDAR)
    case DISPLAY_MODE_SET_DAY:
#endif
#if defined(USE_ALARM)
    case DISPLAY_MODE_SET_ALARM_HOUR:
#endif
      hour = -1;
      break;
    case DISPLAY_MODE_SET_MINUTE:
#if defined(USE_CALENDAR)
    case DISPLAY_MODE_SET_MONTH:
    case DISPLAY_MODE_SET_YEAR:
#endif
#if defined(USE_ALARM)
    case DISPLAY_MODE_SET_ALARM_MINUTE:
#endif
      minute = -1;
      break;
    default:
      break;
    }
  }

  clkDisplay.clear();

  bool toColon = ssc_display_mode <= DISPLAY_MODE_SET_MINUTE;
#if defined(USE_CALENDAR)
  toColon = ssc_display_mode != DISPLAY_MODE_SET_YEAR;
#endif
#if __USE_MATRIX_DISPLAY__
  bool toDate = false;
#if defined(USE_CALENDAR)
  toDate = (ssc_display_mode >= DISPLAY_MODE_SET_DAY &&
            ssc_display_mode <= DISPLAY_MODE_SET_YEAR);
  if (ssc_display_mode == DISPLAY_MODE_SET_YEAR)
  {
    sscSetYearString(1, minute);
  }
  else
#endif
  {

    uint8_t x = 1;
#if defined(SHOW_SECOND_COLUMN)
    // если включен секундный столбик, сместить символы влево на один столбец
    if (ssc_display_mode == DISPLAY_MODE_SHOW_TIME &&
        read_eeprom_8(SECOND_COLUMN_ON_OF_DATA_EEPROM_INDEX))
    {
      x = 0;
    }
#endif
    sscSetTimeString(x, hour, minute, toColon, toDate);
  }
#else
  sscShowTime(hour, minute, toColon);
#endif
}

#if defined(SHOW_SECOND_COLUMN)
void sscShowSecondColumn(int8_t second)
{ // формирование секундного столбика
  uint8_t col_sec = 0;
  uint8_t x = second / 5;
  for (uint8_t i = 0; i < x; i++)
  {
    if (i < 6)
    { // нарастание снизу вверх
      col_sec += 1;
      col_sec = col_sec << 1;
    }
    else
    { // убывание снизу вверх
      col_sec = col_sec << 1;
      col_sec &= ~(1 << 7);
    }
  }
  clkDisplay.setColumn(31, col_sec);
}
#endif

void sscCheckData(uint8_t &dt,
                  uint8_t max,
                  bool toUp,
                  uint8_t min = 0,
                  bool toLoop = true)
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

void sscClearButtonFlag()
{
  clkButtons.setButtonFlag(CLK_BTN_SET, CLK_BTN_FLAG_NONE);
  clkButtons.setButtonFlag(CLK_BTN_UP, CLK_BTN_FLAG_NONE);
  clkButtons.setButtonFlag(CLK_BTN_DOWN, CLK_BTN_FLAG_NONE);
}

void sscStopSetting(clkHandle task)
{
  clkTasks.stopTask(task);
  clkTasks.stopTask(clkTasks.return_to_default_mode);
  sscClearButtonFlag();
}

// ==== sscShowTimeSetting ===========================
void _startTimeSettingMode(uint8_t &curHour, uint8_t &curMinute)
{
  clkTasks.startTask(clkTasks.set_time_mode);
  clkTasks.startTask(clkTasks.return_to_default_mode);
  sscClearButtonFlag();
  switch (ssc_display_mode)
  {
  case DISPLAY_MODE_SET_HOUR:
  case DISPLAY_MODE_SET_MINUTE:
    curHour = clkClock.getCurTime().hour();
    curMinute = clkClock.getCurTime().minute();
    break;
#if defined(USE_ALARM)
  case DISPLAY_MODE_SET_ALARM_HOUR:
  case DISPLAY_MODE_SET_ALARM_MINUTE:
    curHour = clkAlarm.getAlarmPoint() / 60;
    curMinute = clkAlarm.getAlarmPoint() % 60;
    break;
  case DISPLAY_MODE_ALARM_ON_OFF:
    curHour = (uint8_t)clkAlarm.getOnOffAlarm();
    break;
#endif
#if defined(SHOW_SECOND_COLUMN)
  case DISPLAY_MODE_SET_SECOND_COLUMN_ON_OFF:
    curHour = (uint8_t)read_eeprom_8(SECOND_COLUMN_ON_OF_DATA_EEPROM_INDEX);
    break;
#endif
#if defined(USE_TICKER_FOR_DATA)
  case DISPLAY_MODE_SET_TICKER_ON_OFF:
    curHour = read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX);
    break;
#endif
  default:
    break;
  }
#if defined(USE_TICKER_FOR_DATA)
  if (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
  {
    switch (ssc_display_mode)
    {
#if defined(USE_ALARM)
    case DISPLAY_MODE_ALARM_ON_OFF:
    case DISPLAY_MODE_SET_ALARM_HOUR:
#endif
#if defined(USE_CALENDAR)
    case DISPLAY_MODE_SET_DAY:
    case DISPLAY_MODE_SET_YEAR:
#endif
#if defined(SHOW_SECOND_COLUMN)
    case DISPLAY_MODE_SET_SECOND_COLUMN_ON_OFF:
#endif
#if defined(USE_TICKER_FOR_DATA)
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

void _checkBtnSetForTmSet(uint8_t &curHour,
                          uint8_t &curMinute,
                          bool &time_checked)
{
  if (clkButtons.getButtonFlag(CLK_BTN_SET) > CLK_BTN_FLAG_NONE)
  {
    if (time_checked)
    {
      switch (ssc_display_mode)
      {
      case DISPLAY_MODE_SET_HOUR:
      case DISPLAY_MODE_SET_MINUTE:
#if defined(RTC_DS3231)
        // устанавливаем режим 24 часа
        clkClock.setClockMode(false);
#endif
        clkClock.setCurTime(curHour, curMinute, 0);
        sscRtcNow();
        break;
#if defined(USE_CALENDAR)
      case DISPLAY_MODE_SET_DAY:
      case DISPLAY_MODE_SET_MONTH:
        clkClock.setCurDate(curHour, curMinute);
        sscRtcNow();
        break;
      case DISPLAY_MODE_SET_YEAR:
        clkClock.setCurYear(curMinute);
        sscRtcNow();
        break;
#endif
#if defined(USE_ALARM)
      case DISPLAY_MODE_SET_ALARM_HOUR:
      case DISPLAY_MODE_SET_ALARM_MINUTE:
        clkAlarm.setAlarmPoint(curHour * 60 + curMinute);
        break;
      case DISPLAY_MODE_ALARM_ON_OFF:
        clkAlarm.setOnOffAlarm((bool)curHour);
        break;
#endif
#if defined(USE_TICKER_FOR_DATA)
      case DISPLAY_MODE_SET_TICKER_ON_OFF:
        write_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX, curHour);
        break;
#endif
#if defined(SHOW_SECOND_COLUMN)
      case DISPLAY_MODE_SET_SECOND_COLUMN_ON_OFF:
        write_eeprom_8(SECOND_COLUMN_ON_OF_DATA_EEPROM_INDEX, curHour);
        break;
#endif
      default:
        break;
      }
      time_checked = false;
    }
    if (clkButtons.getButtonFlag(CLK_BTN_SET, true) == CLK_BTN_FLAG_NEXT)
    {
      switch (ssc_display_mode)
      {
      case DISPLAY_MODE_SET_HOUR:
#if defined(USE_CALENDAR)
      case DISPLAY_MODE_SET_MINUTE:
      case DISPLAY_MODE_SET_DAY:
      case DISPLAY_MODE_SET_MONTH:
#endif
#if defined(USE_ALARM)
      case DISPLAY_MODE_SET_ALARM_HOUR:
#endif
        ssc_display_mode = clkDisplayMode(uint8_t(ssc_display_mode + 1));
        sscStopSetting(clkTasks.set_time_mode);
        break;
#if defined(USE_ALARM)
      case DISPLAY_MODE_ALARM_ON_OFF:
        ssc_display_mode = (curHour) ? DISPLAY_MODE_SET_ALARM_HOUR
                                     : DISPLAY_MODE_SHOW_TIME;
        sscStopSetting(clkTasks.set_time_mode);
        break;
#endif
#if defined(USE_TICKER_FOR_DATA)
      case DISPLAY_MODE_SET_TICKER_ON_OFF:
#if __USE_AUTO_SHOW_DATA__
        ssc_display_mode = DISPLAY_MODE_SET_AUTO_SHOW_PERIOD;
#elif defined(WS2812_MATRIX_DISPLAY)
        ssc_display_mode = DISPLAY_MODE_SET_COLOR_OF_NUMBER;
#else
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
#endif
        sscStopSetting(clkTasks.set_time_mode);
        break;
#endif
#if defined(SHOW_SECOND_COLUMN) || __USE_AUTO_SHOW_DATA__
#if __USE_AUTO_SHOW_DATA__
      case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
#endif
#if defined(SHOW_SECOND_COLUMN)
      case DISPLAY_MODE_SET_SECOND_COLUMN_ON_OFF:
#endif
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
        sscStopSetting(clkTasks.set_time_mode);
        break;
#endif
      default:
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
        sscStopSetting(clkTasks.set_time_mode);
        break;
      }
    }
    else
    {
      ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
      sscStopSetting(clkTasks.set_time_mode);
    }
  }
}

void _checkBtnUpDownForTmSet(uint8_t &curHour,
                             uint8_t &curMinute,
                             bool &time_checked)
{
  if ((clkButtons.getButtonFlag(CLK_BTN_UP) == CLK_BTN_FLAG_NEXT) ||
      (clkButtons.getButtonFlag(CLK_BTN_DOWN, true) == CLK_BTN_FLAG_NEXT))
  {
    bool dir = clkButtons.getButtonFlag(CLK_BTN_UP, true) == CLK_BTN_FLAG_NEXT;
    switch (ssc_display_mode)
    {
    case DISPLAY_MODE_SET_HOUR:
#if defined(USE_ALARM)
    case DISPLAY_MODE_SET_ALARM_HOUR:
#endif
      sscCheckData(curHour, 23, dir);
      break;
    case DISPLAY_MODE_SET_MINUTE:
#if defined(USE_ALARM)
    case DISPLAY_MODE_SET_ALARM_MINUTE:
#endif
      sscCheckData(curMinute, 59, dir);
      break;
#if __USE_ON_OFF_DATA__
#if defined(USE_ALARM)
    case DISPLAY_MODE_ALARM_ON_OFF:
#endif
#if defined(USE_TICKER_FOR_DATA)
    case DISPLAY_MODE_SET_TICKER_ON_OFF:
#endif
#if defined(SHOW_SECOND_COLUMN)
    case DISPLAY_MODE_SET_SECOND_COLUMN_ON_OFF:
#endif
      sscCheckData(curHour, 1, true);
      break;
#endif
#if defined(USE_CALENDAR)
    case DISPLAY_MODE_SET_DAY:
      uint8_t i;
      i = pgm_read_byte(&daysInMonth[curMinute - 1]);
      if (curMinute == 2 && (clkClock.getCurTime().year() % 4 == 0))
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
  }
}

void _setDisplayForTmSet(uint8_t &curHour, uint8_t &curMinute)
{
  if (ssc_display_mode == DISPLAY_MODE_SHOW_TIME)
  {
#if defined(USE_TICKER_FOR_DATA)
    if (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
    {
      sscAssembleString(DISPLAY_MODE_SHOW_TIME);
    }
#endif
  }
  else if (clkTasks.getTaskState(clkTasks.set_time_mode))
  {
#if __USE_ON_OFF_DATA__
    bool _blink = !sscBlinkFlag &&
                  !clkButtons.isButtonClosed(CLK_BTN_UP) &&
                  !clkButtons.isButtonClosed(CLK_BTN_DOWN);
#endif

    switch (ssc_display_mode)
    {
#if defined(USE_ALARM)
    case DISPLAY_MODE_ALARM_ON_OFF:
      sscShowOnOffData(SET_ALARM_TAG,
                       curHour,
                       _blink);
      break;
#endif
#if defined(USE_TICKER_FOR_DATA)
    case DISPLAY_MODE_SET_TICKER_ON_OFF:
      sscShowOnOffData(SET_TICKER_TAG,
                       curHour,
                       _blink);
      break;
#endif
#if defined(SHOW_SECOND_COLUMN)
    case DISPLAY_MODE_SET_SECOND_COLUMN_ON_OFF:
      sscShowOnOffData(SET_SECOND_COLUMN_TAG,
                       curHour,
                       _blink);
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

  if (!clkTasks.getTaskState(clkTasks.set_time_mode))
  {
    _startTimeSettingMode(curHour, curMinute);
    time_checked = false;
#if !__USE_MATRIX_DISPLAY__ && !defined(LCD_I2C_DISPLAY)
    clkDisplay.sleep(); // слегка мигнуть экраном при входе в настройки
    return;
#endif
  }

#if defined(USE_TICKER_FOR_DATA)
  // подождать, пока отработает бегущая строка
  if (clkTasks.getTaskState(clkTasks.ticker))
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
      curHour = clkClock.getCurTime().hour();
      curMinute = clkClock.getCurTime().minute();
      break;
#if defined(USE_CALENDAR)
    case DISPLAY_MODE_SET_DAY:
    case DISPLAY_MODE_SET_MONTH:
      curHour = clkClock.getCurTime().day();
      curMinute = clkClock.getCurTime().month();
      break;
    case DISPLAY_MODE_SET_YEAR:
      curHour = 20;
      curMinute = clkClock.getCurTime().year() % 100;
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

void sscShowDisplay()
{
#if defined(USE_TICKER_FOR_DATA)
  // обновление экрана делать только если в данный момент не работает бегущая строка, она сама обновляет экран, когда ей это нужно
  if (!clkTasks.getTaskState(clkTasks.ticker))
#endif
    clkDisplay.show();
}

void sscCheckSetButton()
{
  switch (clkButtons.getButtonState(CLK_BTN_SET))
  {
  case BTN_ONECLICK:
    switch (ssc_display_mode)
    {
    case DISPLAY_MODE_SET_HOUR:
    case DISPLAY_MODE_SET_MINUTE:
#if defined(USE_CALENDAR)
    case DISPLAY_MODE_SET_DAY:
    case DISPLAY_MODE_SET_MONTH:
    case DISPLAY_MODE_SET_YEAR:
#endif
#if defined(USE_ALARM)
    case DISPLAY_MODE_SET_ALARM_HOUR:
    case DISPLAY_MODE_SET_ALARM_MINUTE:
    case DISPLAY_MODE_ALARM_ON_OFF:
#endif
#if __USE_SET_BRIGHTNESS_MODE__
#if __USE_LIGHT_SENSOR__
    case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
    case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
#endif
#if __USE_LIGHT_SENSOR__
    case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
#endif
#if defined(USE_TICKER_FOR_DATA)
    case DISPLAY_MODE_SET_TICKER_ON_OFF:
#endif
#if __USE_AUTO_SHOW_DATA__
    case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
#endif
#if defined(WS2812_MATRIX_DISPLAY)
    case DISPLAY_MODE_SET_COLOR_OF_NUMBER:
#endif
#if defined(SHOW_SECOND_COLUMN)
    case DISPLAY_MODE_SET_SECOND_COLUMN_ON_OFF:
#endif
    case DISPLAY_MODE_CUSTOM_1:
    case DISPLAY_MODE_CUSTOM_2:
    case DISPLAY_MODE_CUSTOM_3:
    case DISPLAY_MODE_CUSTOM_4:
      clkButtons.setButtonFlag(CLK_BTN_SET, CLK_BTN_FLAG_NEXT);
      break;
    case DISPLAY_MODE_SHOW_TIME:
#if defined(USE_ALARM)
#if defined(USE_ONECLICK_TO_SET_ALARM)
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
#if defined(USE_ALARM)
#if !defined(USE_ONECLICK_TO_SET_ALARM)
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
#if defined(USE_CALENDAR)
    case DISPLAY_MODE_SHOW_DATE:
#if __USE_AUTO_SHOW_DATA__
      clkTasks.stopTask(clkTasks.auto_show_mode);
#endif
      ssc_display_mode = DISPLAY_MODE_SET_DAY;
      break;
#endif
    case DISPLAY_MODE_SET_HOUR:
    case DISPLAY_MODE_SET_MINUTE:
#if defined(USE_CALENDAR)
    case DISPLAY_MODE_SET_DAY:
    case DISPLAY_MODE_SET_MONTH:
    case DISPLAY_MODE_SET_YEAR:
#endif
#if defined(USE_ALARM)
    case DISPLAY_MODE_SET_ALARM_HOUR:
    case DISPLAY_MODE_SET_ALARM_MINUTE:
    case DISPLAY_MODE_ALARM_ON_OFF:
#endif
#if defined(USE_TICKER_FOR_DATA)
    case DISPLAY_MODE_SET_TICKER_ON_OFF:
#endif
#if __USE_AUTO_SHOW_DATA__
    case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
#endif
#if __USE_LIGHT_SENSOR__
    case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
#endif
#if defined(WS2812_MATRIX_DISPLAY)
    case DISPLAY_MODE_SET_COLOR_OF_NUMBER:
#endif
#if defined(SHOW_SECOND_COLUMN)
    case DISPLAY_MODE_SET_SECOND_COLUMN_ON_OFF:
#endif
    case DISPLAY_MODE_CUSTOM_1:
    case DISPLAY_MODE_CUSTOM_2:
    case DISPLAY_MODE_CUSTOM_3:
    case DISPLAY_MODE_CUSTOM_4:
      clkButtons.setButtonFlag(CLK_BTN_SET, CLK_BTN_FLAG_EXIT);
      break;
    default:
      break;
    }
    break;
  }
}

void sscCheckUDbtn(clkButtonType btn)
{
  switch (clkButtons.getLastState(btn))
  {
  case BTN_DOWN:
  case BTN_DBLCLICK:
    clkButtons.setButtonFlag(btn, CLK_BTN_FLAG_NEXT);
    break;
  case BTN_LONGCLICK:
    switch (ssc_display_mode)
    {
#if defined(USE_ALARM)
    case DISPLAY_MODE_ALARM_ON_OFF:
#endif
#if defined(USE_TICKER_FOR_DATA)
    case DISPLAY_MODE_SET_TICKER_ON_OFF:
#endif
#if defined(SHOW_SECOND_COLUMN)
    case DISPLAY_MODE_SET_SECOND_COLUMN_ON_OFF:
#endif
      break; // для этих режимов не реагировать на удержание кнопки, чтобы исключить постоянное переключение параметра
    default:
      clkButtons.setButtonFlag(btn, CLK_BTN_FLAG_NEXT);
      break;
    }
    break;
  }
}

void sscCheckUpDownButton()
{
  clkButtons.getButtonState(CLK_BTN_UP);
  clkButtons.getButtonState(CLK_BTN_DOWN);

  switch (ssc_display_mode)
  {
  case DISPLAY_MODE_SHOW_TIME:
    if (clkButtons.getLastState(CLK_BTN_DOWN) == BTN_LONGCLICK &&
        !clkButtons.isButtonClosed(CLK_BTN_UP))
    {
#if defined(USE_TICKER_FOR_DATA)
      // вход в настройки анимации
      ssc_display_mode = DISPLAY_MODE_SET_TICKER_ON_OFF;
      clkButtons.resetButtonState(CLK_BTN_DOWN);
#elif __USE_AUTO_SHOW_DATA__
      // вход в настройки периода автовывода на экран даты и/или температуры
      ssc_display_mode = DISPLAY_MODE_SET_AUTO_SHOW_PERIOD;
      clkButtons.resetButtonState(CLK_BTN_DOWN);
#elif defined(WS2812_MATRIX_DISPLAY)
      ssc_display_mode = DISPLAY_MODE_SET_COLOR_OF_NUMBER;
      clkButtons.resetButtonState(CLK_BTN_DOWN);
#elif defined(SHOW_SECOND_COLUMN)
      // вход в настройки секундного столбика
      ssc_display_mode = DISPLAY_MODE_SET_SECOND_COLUMN_ON_OFF;
      clkButtons.resetButtonState(CLK_BTN_DOWN);
#endif
    }
#if __USE_TEMP_DATA__
    if (clkButtons.getLastState(CLK_BTN_UP) == BTN_ONECLICK)
    {
      ssc_display_mode = DISPLAY_MODE_SHOW_TEMP;
    }
#endif
#if defined(USE_CALENDAR)
    if (clkButtons.getLastState(CLK_BTN_DOWN) == BTN_ONECLICK)
    {
      ssc_display_mode = DISPLAY_MODE_SHOW_DATE;
    }
#endif
    if (clkButtons.isSecondButtonPressed(CLK_BTN_UP, CLK_BTN_DOWN, BTN_LONGCLICK) ||
        clkButtons.isSecondButtonPressed(CLK_BTN_DOWN, CLK_BTN_UP, BTN_LONGCLICK))
    {
#if __USE_SET_BRIGHTNESS_MODE__
#if __USE_LIGHT_SENSOR__
      ssc_display_mode = DISPLAY_MODE_SET_BRIGHTNESS_MIN;
#else
      ssc_display_mode = DISPLAY_MODE_SET_BRIGHTNESS_MAX;
#endif
#elif __USE_LIGHT_SENSOR__
      ssc_display_mode = DISPLAY_MODE_SET_LIGHT_THRESHOLD;
#endif
    }
    break;
  case DISPLAY_MODE_SET_HOUR:
  case DISPLAY_MODE_SET_MINUTE:
#if defined(USE_CALENDAR)
  case DISPLAY_MODE_SET_DAY:
  case DISPLAY_MODE_SET_MONTH:
  case DISPLAY_MODE_SET_YEAR:
#endif
#if defined(USE_ALARM)
  case DISPLAY_MODE_SET_ALARM_HOUR:
  case DISPLAY_MODE_SET_ALARM_MINUTE:
  case DISPLAY_MODE_ALARM_ON_OFF:
#endif
#if __USE_LIGHT_SENSOR__
  case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
#endif
#if __USE_SET_BRIGHTNESS_MODE__
#if __USE_LIGHT_SENSOR__
  case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
#endif
#if defined(USE_TICKER_FOR_DATA)
  case DISPLAY_MODE_SET_TICKER_ON_OFF:
#endif
#if __USE_AUTO_SHOW_DATA__
  case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
#endif
#if defined(WS2812_MATRIX_DISPLAY)
  case DISPLAY_MODE_SET_COLOR_OF_NUMBER:
#endif
#if defined(SHOW_SECOND_COLUMN)
  case DISPLAY_MODE_SET_SECOND_COLUMN_ON_OFF:
#endif
  case DISPLAY_MODE_CUSTOM_1:
  case DISPLAY_MODE_CUSTOM_2:
  case DISPLAY_MODE_CUSTOM_3:
  case DISPLAY_MODE_CUSTOM_4:
    if (!clkButtons.isButtonClosed(CLK_BTN_DOWN))
    {
      sscCheckUDbtn(CLK_BTN_UP);
    }
    if (!clkButtons.isButtonClosed(CLK_BTN_UP))
    {
      sscCheckUDbtn(CLK_BTN_DOWN);
    }
    break;
#if __USE_TEMP_DATA__
  case DISPLAY_MODE_SHOW_TEMP:
    if (clkButtons.getLastState(CLK_BTN_UP) == BTN_ONECLICK)
    {
      sscReturnToDefMode();
    }
    break;
#endif
#if defined(USE_CALENDAR)
  case DISPLAY_MODE_SHOW_DATE:
    if (clkButtons.getLastState(CLK_BTN_DOWN) == BTN_ONECLICK)
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

void sscSetDisplayMode()
{
  switch (ssc_display_mode)
  {
  case DISPLAY_MODE_SET_HOUR:
  case DISPLAY_MODE_SET_MINUTE:
#if defined(USE_CALENDAR)
  case DISPLAY_MODE_SET_DAY:
  case DISPLAY_MODE_SET_MONTH:
  case DISPLAY_MODE_SET_YEAR:
#endif
#if defined(USE_ALARM)
  case DISPLAY_MODE_SET_ALARM_HOUR:
  case DISPLAY_MODE_SET_ALARM_MINUTE:
  case DISPLAY_MODE_ALARM_ON_OFF:
#endif
#if defined(USE_TICKER_FOR_DATA)
  case DISPLAY_MODE_SET_TICKER_ON_OFF:
#endif
#if defined(SHOW_SECOND_COLUMN)
  case DISPLAY_MODE_SET_SECOND_COLUMN_ON_OFF:
#endif
    if (!clkTasks.getTaskState(clkTasks.set_time_mode))
    {
      sscShowTimeSetting();
    }
    break;
#if __USE_AUTO_SHOW_DATA__
  case DISPLAY_AUTO_SHOW_DATA:
#if defined(USE_CALENDAR)
  case DISPLAY_MODE_SHOW_DATE:
#endif
#if __USE_TEMP_DATA__
  case DISPLAY_MODE_SHOW_TEMP:
#endif
    if (!clkTasks.getTaskState(clkTasks.auto_show_mode))
    {
      sscAutoShowData();
    }
    break;
#endif
#if __USE_OTHER_SETTING__
#if __USE_AUTO_SHOW_DATA__
  case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
#endif
#if __USE_LIGHT_SENSOR__
  case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
#endif
#if __USE_SET_BRIGHTNESS_MODE__
#if __USE_LIGHT_SENSOR__
  case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
#endif
#if defined(WS2812_MATRIX_DISPLAY)
  case DISPLAY_MODE_SET_COLOR_OF_NUMBER:
#endif
    if (!clkTasks.getTaskState(clkTasks.other_setting_mode))
    {
      sscShowOtherSetting();
    }
    break;
#endif
  default:
    break;
  }
}

#if defined(USE_ALARM)
void sscCheckAlarm()
{
  clkAlarm.tick(clkClock.getCurTime());
  if (clkAlarm.getAlarmState() == ALARM_YES &&
      !clkTasks.getTaskState(clkTasks.alarm_buzzer))
  {
    sscRunAlarmBuzzer();
#if defined USE_CLOCK_EVENT
    sscAlarmEvent.run();
#endif
  }
}

void sscRunAlarmBuzzer()
{
#if BUZZER_PIN >= 0
  static uint8_t n = 0;
  static uint8_t k = 0;
  static uint8_t m = 0;
  // "мелодия" пищалки: первая строка - частота, вторая строка - длительность
  static const PROGMEM uint32_t pick[2][8] = {
      {2000, 0, 2000, 0, 2000, 0, 2000, 0},
      {70, 70, 70, 70, 70, 70, 70, 510}};

  if (!clkTasks.getTaskState(clkTasks.alarm_buzzer))
  {
    clkTasks.startTask(clkTasks.alarm_buzzer);
    n = 0;
    k = 0;
    m = 0;
  }
  else if (clkAlarm.getAlarmState() == ALARM_ON)
  { // остановка пищалки, если будильник отключен
    clkTasks.stopTask(clkTasks.alarm_buzzer);
    return;
  }

  tone(BUZZER_PIN,
       pgm_read_dword(&pick[0][n]),
       pgm_read_dword(&pick[1][n]));
  clkTasks.setTaskInterval(clkTasks.alarm_buzzer,
                           pgm_read_dword(&pick[1][n]), true);
  if (++n >= 8)
  {
    n = 0;
    if (++k >= ALARM_DURATION)
    { // приостановка пищалки через заданное число секунд
      k = 0;
      if (++m >= ALARM_REPETITION_COUNT)
      { // отключение пищалки после заданного количества срабатываний
        clkTasks.stopTask(clkTasks.alarm_buzzer);
        clkTasks.setTaskInterval(clkTasks.alarm_buzzer, 50, false);
        clkAlarm.setAlarmState(ALARM_ON);
      }
      else
      {
        clkTasks.setTaskInterval(clkTasks.alarm_buzzer,
                                 ALARM_SNOOZE_DELAY * 1000ul,
                                 true);
      }
    }
  }
#endif
}

#endif

#if __USE_LIGHT_SENSOR__
void sscSetBrightness()
{
#if __USE_SET_BRIGHTNESS_MODE__
  if (clkTasks.getTaskState(clkTasks.other_setting_mode))
  {
    if (ssc_display_mode == DISPLAY_MODE_SET_BRIGHTNESS_MAX ||
        ssc_display_mode == DISPLAY_MODE_SET_BRIGHTNESS_MIN)
    {
      return; // в режиме настройки яркости ничего не регулировать
    }
  }
#endif

  uint8_t x = 1;
#if __USE_LIGHT_SENSOR__ && LIGHT_SENSOR_PIN >= 0
  uint8_t _pin = LIGHT_SENSOR_PIN; // иначе на stm32duino зависает analogRead()
  static uint16_t b = analogRead(_pin);
  b = (b * 2 + analogRead(_pin)) / 3;
  if (b < read_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX) * light_threshold_step)
  {
    x = read_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX);
  }
  else if (b > (read_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX) * light_threshold_step + light_threshold_step / 2))
#endif
  {
    x = read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX);
  }
  clkDisplay.setBrightness(x);
}

#endif

#if __USE_TEMP_DATA__

#if defined(USE_DS18B20)
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
  result = clkClock.getTemperature();
#endif

  return result;
}
#endif

#if __USE_OTHER_SETTING__

// ==== sscShowOtherSetting =============================
void _startOtherSettingMode(uint8_t &x)
{
  clkTasks.startTask(clkTasks.other_setting_mode);
  clkTasks.startTask(clkTasks.return_to_default_mode);
  sscClearButtonFlag();
  switch (ssc_display_mode)
  {
#if __USE_LIGHT_SENSOR__
  case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
    x = read_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX);
    break;
#endif
#if __USE_SET_BRIGHTNESS_MODE__
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
    x = read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX);
    break;
#if __USE_LIGHT_SENSOR__
  case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
    x = read_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX);
    break;
#endif
#endif
#if __USE_AUTO_SHOW_DATA__
  case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
    x = read_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX);
    break;
#endif
#if defined(WS2812_MATRIX_DISPLAY)
  case DISPLAY_MODE_SET_COLOR_OF_NUMBER:
    x = sscGetIndexOfCurrentColorOfNumber();
    break;
#endif
  default:
    break;
  }
#if defined(USE_TICKER_FOR_DATA)
  if (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
  {
    sscAssembleString(ssc_display_mode);
  }
#endif
}

void _checkBtnSetForOthSet(uint8_t &x)
{
  if (clkButtons.getButtonFlag(CLK_BTN_SET) > CLK_BTN_FLAG_NONE)
  {
    bool _next = clkButtons.getButtonFlag(CLK_BTN_SET, true) == CLK_BTN_FLAG_NEXT;
    switch (ssc_display_mode)
    {
#if __USE_LIGHT_SENSOR__
    case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
      write_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX, x);
      ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
      sscStopSetting(clkTasks.other_setting_mode);
      break;
#endif
#if __USE_SET_BRIGHTNESS_MODE__
    case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
      write_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX, x);
#if __USE_LIGHT_SENSOR__
      if (_next)
      {
        ssc_display_mode = DISPLAY_MODE_SET_LIGHT_THRESHOLD;
      }
      else
#endif
      {
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
      }
      sscStopSetting(clkTasks.other_setting_mode);
      break;
#if __USE_LIGHT_SENSOR__
    case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
      write_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX, x);
      if (_next)
      {
        ssc_display_mode = DISPLAY_MODE_SET_BRIGHTNESS_MAX;
      }
      else
      {
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
      }
      sscStopSetting(clkTasks.other_setting_mode);
      break;
#endif
#endif
#if __USE_AUTO_SHOW_DATA__
    case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
      write_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX, x);
#if defined(WS2812_MATRIX_DISPLAY)
      if (_next)
      {
        ssc_display_mode = DISPLAY_MODE_SET_COLOR_OF_NUMBER;
      }
      else
#elif defined(SHOW_SECOND_COLUMN)
      if (_next)
      {
        ssc_display_mode = DISPLAY_MODE_SET_SECOND_COLUMN_ON_OFF;
      }
      else
#endif
      {
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
      }
      sscStopSetting(clkTasks.other_setting_mode);
      break;
#endif
#if defined(WS2812_MATRIX_DISPLAY)
    case DISPLAY_MODE_SET_COLOR_OF_NUMBER:
      write_eeprom_crgb(COLOR_OF_NUMBER_VALUE_EEPROM_INDEX,
                        pgm_read_dword(&color_of_number[x]));
      clkDisplay.setColorOfNumber(pgm_read_dword(&color_of_number[x]));
#if defined(SHOW_SECOND_COLUMN)
      if (_next)
      {
        ssc_display_mode = DISPLAY_MODE_SET_SECOND_COLUMN_ON_OFF;
      }
      else
#endif
      {
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
      }
      sscStopSetting(clkTasks.other_setting_mode);
      break;
#endif
    default:
      break;
    }
  }
}

void _checkBtnUpDownForOthSet(uint8_t &x)
{
  if ((clkButtons.getButtonFlag(CLK_BTN_UP) == CLK_BTN_FLAG_NEXT) ||
      (clkButtons.getButtonFlag(CLK_BTN_DOWN, true) == CLK_BTN_FLAG_NEXT))
  {
    bool dir = (clkButtons.getButtonFlag(CLK_BTN_UP, true) == CLK_BTN_FLAG_NEXT);
    switch (ssc_display_mode)
    {
#if __USE_LIGHT_SENSOR__
    case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
      sscCheckData(x, 9, dir, 1, false);
      break;
#endif
#if __USE_SET_BRIGHTNESS_MODE__
#if __USE_LIGHT_SENSOR__
    case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
    case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
#if defined(MAX72XX_MATRIX_DISPLAY) || defined(MAX72XX_7SEGMENT_DISPLAY)
      sscCheckData(x, 15, dir, 0, false);
#elif defined(WS2812_MATRIX_DISPLAY)
      sscCheckData(x, 25, dir, 1, false);
#elif defined(TM1637_DISPLAY)
      sscCheckData(x, 7, dir, 1, false);
#endif
      break;
#endif
#if __USE_AUTO_SHOW_DATA__
    case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
      sscCheckData(x, 7, dir, 0, false);
      break;
#endif
#if defined(WS2812_MATRIX_DISPLAY)
    case DISPLAY_MODE_SET_COLOR_OF_NUMBER:
      sscCheckData(x, 7, dir, 0, true);
      clkDisplay.setColorOfNumber(pgm_read_dword(&color_of_number[x]));
      break;
#endif
    default:
      break;
    }
  }
}

void _setDisplayDataForOthSet(uint8_t &x)
{
  if (ssc_display_mode == DISPLAY_MODE_SHOW_TIME)
  {
#if defined(USE_TICKER_FOR_DATA)
    if (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
    {
      sscAssembleString(DISPLAY_MODE_SHOW_TIME);
    }
#endif
  }
  else
  {
    bool blink = !sscBlinkFlag &&
                 !clkButtons.isButtonClosed(CLK_BTN_UP) &&
                 !clkButtons.isButtonClosed(CLK_BTN_DOWN);
    switch (ssc_display_mode)
    {
#if __USE_LIGHT_SENSOR__
    case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
#if __USE_MATRIX_DISPLAY__
      sscSetOtherDataString(SET_LIGHT_THRESHOLD_TAG, 1, x, blink);
#else
      sscSetOtherData(SET_LIGHT_THRESHOLD_TAG, x, blink);
#endif
      break;
#endif
#if __USE_SET_BRIGHTNESS_MODE__
#if __USE_LIGHT_SENSOR__
    case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
    case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
      clkDisplay.setBrightness(x);
#if __USE_MATRIX_DISPLAY__
      sscSetOtherDataString(SET_BRIGHTNESS_TAG, 1, x, blink);
#else
      sscSetOtherData(SET_BRIGHTNESS_TAG, x, blink);
#endif
      break;
#endif
#if __USE_AUTO_SHOW_DATA__
    case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
#if __USE_MATRIX_DISPLAY__
      sscSetOtherDataString(SET_AUTO_SHOW_PERIOD_TAG,
                            1,
                            sscGetPeriodForAutoShow(x),
                            blink);
#else
      sscSetOtherData(SET_AUTO_SHOW_PERIOD_TAG,
                      sscGetPeriodForAutoShow(x),
                      blink);
#endif
      break;
#endif
#if defined(WS2812_MATRIX_DISPLAY)
    case DISPLAY_MODE_SET_COLOR_OF_NUMBER:
      sscSetOtherDataString(SET_COLOR_OF_NUMBER_TAG, 1, x, blink);
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
  if (!clkTasks.getTaskState(clkTasks.other_setting_mode))
  {
    _startOtherSettingMode(x);
#if !__USE_MATRIX_DISPLAY__ && !defined(LCD_I2C_DISPLAY)
    clkDisplay.sleep(); // слегка мигнуть экраном при входе в настройки
    return;
#endif
  }

#if defined(USE_TICKER_FOR_DATA)
  // подождать, пока отработает бегущая строка
  if (clkTasks.getTaskState(clkTasks.ticker))
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

#if __USE_AUTO_SHOW_DATA__

void _startAutoShowMode(uint8_t &n, uint8_t &n_max)
{
  switch (ssc_display_mode)
  {
#if defined(USE_CALENDAR)
  case DISPLAY_MODE_SHOW_DATE:
#if __USE_MATRIX_DISPLAY__
    n = 0;
#else
    n = 1;
#endif
    n_max = 3;
    break;
#endif
#if __USE_TEMP_DATA__
  case DISPLAY_MODE_SHOW_TEMP:
    n = 3;
    n_max = 4;
    break;
#endif
  case DISPLAY_AUTO_SHOW_DATA:
#if defined(USE_CALENDAR)
    n = 0;
    n_max = 3;
#else
    n = 3;
#endif
#if __USE_TEMP_DATA__
    n_max = 4;
#endif
    break;
  default:
    break;
  }
  clkTasks.startTask(clkTasks.auto_show_mode);
}

#if __USE_MATRIX_DISPLAY__

void _setDisplayForAutoShowData(uint8_t &n)
{
  switch (n)
  {
#if defined(USE_CALENDAR)
  case 0:
#if defined(USE_TICKER_FOR_DATA)
    if (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
    {
      sscAssembleString(DISPLAY_MODE_SHOW_DOW);
    }
    else
#endif
    {
      sscSetDayOfWeakString(7, getDayOfWeek(clkClock.getCurTime().day(),
                                            clkClock.getCurTime().month(),
                                            clkClock.getCurTime().year()));
    }
    break;
  case 1:
#if defined(USE_TICKER_FOR_DATA)
    if (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
    {
      sscAssembleString(DISPLAY_MODE_SHOW_DAY_AND_MONTH);
    }
    else
#endif
    {
      sscSetTimeString(1,
                       clkClock.getCurTime().day(),
                       clkClock.getCurTime().month(),
                       true,
                       true,
                       false);
    }
    break;
  case 2:
#if defined(USE_TICKER_FOR_DATA)
    if (read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
    {
      sscAssembleString(DISPLAY_MODE_SHOW_YEAR);
    }
    else
#endif
    {
      sscSetYearString(1, clkClock.getCurTime().year());
    }
    break;
#endif
#if __USE_TEMP_DATA__
  case 3:
#if defined(USE_TICKER_FOR_DATA)
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
}

#else

void _setDisplayForAutoShowData(uint8_t &n)
{
  switch (n)
  {
#if defined(USE_CALENDAR)
  case 0:
#if defined(LCD_I2C_DISPLAY)
    clkDisplay.setDispData(0, 0x0d);
    clkDisplay.setDispData(1, 0x0a);
    clkDisplay.setDispData(2, 0x11);
    clkDisplay.setDispData(3, 0x0e);
    clkDisplay.setColon(false, LCD_COLON_NO_COLON);
#else
    clkDisplay.setDispData(0, clkDisplay.encodeDigit(0x0d));
    clkDisplay.setDispData(1, clkDisplay.encodeDigit(0x0a));
#if defined(TM1637_DISPLAY)
    clkDisplay.setDispData(2, 0b01111000);
#elif defined(MAX72XX_7SEGMENT_DISPLAY)
    clkDisplay.setDispData(2, 0b00001111);
#endif
    clkDisplay.setDispData(3, clkDisplay.encodeDigit(0x0e));
#endif
    break;
  case 1:
  case 2:
    sscShowDate(clkClock.getCurTime());
    break;
#endif
#if __USE_TEMP_DATA__
  case 3:
    sscShowTemp(sscGetCurTemp());
    break;
#endif
  default:
    break;
  }
}

#endif

void sscAutoShowData()
{
  static uint8_t n = 0;
  static uint8_t n_max = 0;
  static unsigned long timer = 0;

  if (!clkTasks.getTaskState(clkTasks.auto_show_mode))
  {
    _startAutoShowMode(n, n_max);
  }

#if defined(USE_TICKER_FOR_DATA)
  if (clkTasks.getTaskState(clkTasks.ticker))
  {
    timer = millis();
    return;
  }
#endif

  if (millis() - timer >= 1000)
  {
    timer = millis();
#if defined(USE_TICKER_FOR_DATA)
    if (!read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
    {
      clkDisplay.clear();
    }
#else
    clkDisplay.clear();
#endif
    _setDisplayForAutoShowData(n);

    if (++n > n_max)
    {
      sscReturnToDefMode();
    }
  }
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

#if __USE_ON_OFF_DATA__
void sscShowOnOffData(clkDataType _type, bool _state, bool blink)
{
  clkDisplay.clear();
#if __USE_MATRIX_DISPLAY__
  sscSetOnOffDataString(_type, 1, _state, blink);
#else
  sscSetOnOffData(_type, _state, blink);
#endif
}
#endif

#if __USE_MATRIX_DISPLAY__

void sscSetTimeString(uint8_t offset,
                      int8_t hour,
                      int8_t minute,
                      bool show_colon,
                      bool toDate,
                      bool toStringData)
{
  if (hour >= 0)
  {
    if (toDate)
    {
      sscSetNumString(offset, hour, 5, 1, toStringData, true);
    }
    else
    {
      sscSetNumString(offset, hour, 6, 1, toStringData);
    }
  }
  if (minute >= 0)
  {
    if (toDate)
    {
      for (uint8_t j = 0; j < 3; j++)
      {
        sscSetChar(offset + 13 + j * 6,
                   pgm_read_byte(&months[(minute - 1) * 3 + j]),
                   5,
                   toStringData);
      }
    }
    else
    {
      sscSetNumString(offset + 16, minute, 6, 1, toStringData);
    }
  }
  if (show_colon && !toDate)
  {
    if (toStringData)
    {
      sData.setData(offset + 14, 0b00100100);
    }
    else if (!sscBlinkFlag || ssc_display_mode != DISPLAY_MODE_SHOW_TIME)
    {
      clkDisplay.setColumn(offset + 14, 0b00100100);
    }
  }
}

void sscSetOtherDataString(clkDataType _type,
                           uint8_t offset,
                           uint8_t _data,
                           bool blink,
                           bool toStringData)
{
  clkDisplay.clear();

  switch (_type)
  {
#if __USE_SET_BRIGHTNESS_MODE__
  case SET_BRIGHTNESS_TAG:
    sscSetTag(offset, DISP_BRIGHTNESS_TAG, 5, toStringData);
#if __USE_LIGHT_SENSOR__
    uint8_t x;
    x = (ssc_display_mode == DISPLAY_MODE_SET_BRIGHTNESS_MIN) ? 0x30 : 0x31;
    sscSetChar(offset + 12, x, 5, toStringData);
#endif
    break;
#endif
#if __USE_LIGHT_SENSOR__
  case SET_LIGHT_THRESHOLD_TAG:
    sscSetTag(offset, DISP_LIGHT_THRESHOLD_TAG, 5, toStringData);
    break;
#endif
#if __USE_AUTO_SHOW_DATA__
  case SET_AUTO_SHOW_PERIOD_TAG:
    sscSetTag(offset, DISP_DATE_DISPLAY_INTERVAL_TAG, 5, toStringData);
    break;
#endif
#if defined(WS2812_MATRIX_DISPLAY)
  case SET_COLOR_OF_NUMBER_TAG:
    sscSetTag(offset, DISP_COLOR_TAG, 5, toStringData);
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
    clkDisplay.setColumn(offset + 18, 0x24); // ":"
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
    sscSetChar(offset + i * (width + 1),
               pgm_read_byte(&tags[index * 3 + i]),
               width,
               toStringData);
  }
}

void sscSetOnOffDataString(clkDataType _type,
                           uint8_t offset,
                           bool _state,
                           bool _blink,
                           bool toStringData)
{
  switch (_type)
  {
#if defined(USE_TICKER_FOR_DATA)
  case SET_TICKER_TAG:
    sscSetTag(offset, DISP_ANIMATION_TAG, 5, toStringData);
    break;
#endif
#if defined(USE_ALARM)
  case SET_ALARM_TAG:
    sscSetTag(offset, DISP_ALARM_TAG, 5, toStringData);
    break;
#endif
#if defined(SHOW_SECOND_COLUMN)
  case SET_SECOND_COLUMN_TAG:
    sscSetTag(offset, DISP_SECOND_COLUMN_TAG, 5, toStringData);
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
    clkDisplay.setColumn(offset + 20, 0x24); // ":"
  }

  if (!_blink)
  {
    sscSetChar(offset + 24, ((_state) ? 0x0C : 0x0B), 6, toStringData);
  }
}

void sscSetNumString(uint8_t offset,
                     uint8_t num,
                     uint8_t width,
                     uint8_t space,
                     bool toStringData,
                     bool firstSpace)
{
  if (firstSpace && num < 10)
  {
    uint8_t x = (width == 6) ? num % 10 : num % 10 + 0x30;
    sscSetChar(offset + 5, x, width, toStringData);
  }
  else
  {
    uint8_t x = (width == 6) ? num / 10 : num / 10 + 0x30;
    sscSetChar(offset, x, width, toStringData);
    x = (width == 6) ? num % 10 : num % 10 + 0x30;
    sscSetChar(offset + width + space, x, width, toStringData);
  }
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
        clkDisplay.setColumn(j, chr_data);
      }
    }
  }
}

#if defined(USE_TICKER_FOR_DATA)
void sscAssembleString(clkDisplayMode data_type, uint8_t lenght)
{
  if (!sData.stringInit(lenght))
    return;

  // скопировать в начало строки содержимое экрана
  for (uint8_t i = 0; i < 32; i++)
  {
    sData.setData(i, clkDisplay.getColumn(i));
  }

  // сформировать вторую часть строки
  switch (data_type)
  {
  case DISPLAY_MODE_SHOW_TIME: // время
    sscSetTimeString(lenght - 31,
                     clkClock.getCurTime().hour(),
                     clkClock.getCurTime().minute(),
                     true,
                     false,
                     true);
    break;

#if __USE_TEMP_DATA__
  case DISPLAY_MODE_SHOW_TEMP: // температура
    sscSetTempString(lenght - 31, sscGetCurTemp(), true);
    break;
#endif

#if __USE_SET_BRIGHTNESS_MODE__
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX: // настройка яркости
#if __USE_LIGHT_SENSOR__
  case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
    uint8_t x;
    if (ssc_display_mode == DISPLAY_MODE_SET_BRIGHTNESS_MAX)
    {
      x = read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX);
    }
#if __USE_LIGHT_SENSOR__
    else
    {
      x = read_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX);
    }
#endif
    sscSetOtherDataString(SET_BRIGHTNESS_TAG, lenght - 31, x, false, true);
    break;
#endif

#if __USE_LIGHT_SENSOR__
  case DISPLAY_MODE_SET_LIGHT_THRESHOLD: // настройка порога переключения яркости
    uint8_t y;
    y = read_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX);
    sscSetOtherDataString(SET_LIGHT_THRESHOLD_TAG, lenght - 31, y, false, true);
    break;
#endif

#if defined(USE_ALARM)
  case DISPLAY_MODE_SET_ALARM_HOUR: // настройка часа срабатывания будильника
    sscSetTimeString(lenght - 31,
                     clkAlarm.getAlarmPoint() / 60,
                     clkAlarm.getAlarmPoint() % 60,
                     true,
                     false,
                     true);
    break;
  case DISPLAY_MODE_ALARM_ON_OFF: // настройка включения/выключения будильника
    sscSetOnOffDataString(SET_ALARM_TAG, lenght - 31, clkAlarm.getOnOffAlarm(), false, true);
    break;
#endif

#if defined(SHOW_SECOND_COLUMN)
  case DISPLAY_MODE_SET_SECOND_COLUMN_ON_OFF: // настройка включения/выключения секундного столбика
    sscSetOnOffDataString(SET_SECOND_COLUMN_TAG,
                          lenght - 31,
                          (bool)read_eeprom_8(SECOND_COLUMN_ON_OF_DATA_EEPROM_INDEX),
                          false,
                          true);
    break;
#endif

#if defined(USE_CALENDAR)
  case DISPLAY_MODE_SHOW_DOW: // день недели
    sscSetDayOfWeakString(lenght - 25,
                          getDayOfWeek(clkClock.getCurTime().day(),
                                       clkClock.getCurTime().month(),
                                       clkClock.getCurTime().year()),
                          true);
    break;

  case DISPLAY_MODE_SHOW_DAY_AND_MONTH: // число и месяц
  case DISPLAY_MODE_SET_DAY:            // настройка числа
    sscSetTimeString(lenght - 31,
                     clkClock.getCurTime().day(),
                     clkClock.getCurTime().month(),
                     true,
                     true,
                     true);
    break;

  case DISPLAY_MODE_SHOW_YEAR: // год
  case DISPLAY_MODE_SET_YEAR:  // настройка года
    sscSetYearString(lenght - 31, clkClock.getCurTime().year(), true);
    break;
#endif
#if defined(USE_TICKER_FOR_DATA)
  case DISPLAY_MODE_SET_TICKER_ON_OFF: // настройка включения/выключения анимации
    sscSetOnOffDataString(SET_TICKER_TAG,
                          lenght - 31,
                          read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX),
                          false,
                          true);
    break;
#endif
#if __USE_AUTO_SHOW_DATA__
  case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
    sscSetOtherDataString(SET_AUTO_SHOW_PERIOD_TAG,
                          lenght - 31,
                          sscGetPeriodForAutoShow(read_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX)),
                          false,
                          true);
    break;
#endif
#if defined(WS2812_MATRIX_DISPLAY)
  case DISPLAY_MODE_SET_COLOR_OF_NUMBER:
    sscSetOtherDataString(SET_COLOR_OF_NUMBER_TAG,
                          lenght - 31,
                          sscGetIndexOfCurrentColorOfNumber(),
                          false,
                          true);
    break;
#endif
  default:
    break;
  }
  sscRunTicker();
}

void sscRunTicker()
{
  static uint8_t n = 0;

  if (!clkTasks.getTaskState(clkTasks.ticker))
  {
    clkTasks.startTask(clkTasks.ticker);
    n = 0;
  }

  for (uint8_t i = 0; i < 32; i++)
  {
    clkDisplay.setColumn(i, sData.getData(i + n));
  }
  clkDisplay.show();

  if (n++ >= sData.getDataLenght() - 32)
  {
    clkTasks.stopTask(clkTasks.ticker);
    sData.stringFree();
  }
}

#endif

#if defined(USE_CALENDAR)
void sscSetDayOfWeakString(uint8_t offset, uint8_t dow, bool toStringData)
{
  for (uint8_t j = 0; j < 3; j++)
  {
    sscSetChar(offset + j * 7,
               pgm_read_byte(&day_of_week[dow * 3 + j]), 5, toStringData);
  }
}

void sscSetYearString(uint8_t offset, int16_t _year, bool toStringData)
{
  sscSetNumString(offset, 20, 6, 2, toStringData);
  if (_year >= 0)
  {
    sscSetNumString(offset + 16, _year % 2000, 6, 2, toStringData);
  }
}
#endif

#if __USE_TEMP_DATA__
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

#if defined(WS2812_MATRIX_DISPLAY)
uint8_t sscGetIndexOfCurrentColorOfNumber()
{
  uint8_t result = 1;
  CRGB col;
  col = clkDisplay.getColorOfNumber();
  for (uint8_t i = 0; i < 8; i++)
  {
    CRGB t_col = pgm_read_dword(&color_of_number[i]);
    if (col.r == t_col.r && col.g == t_col.g && col.b == t_col.b)
    {
      result = i;
      break;
    }
  }

  return result;
}
#endif

#else

void sscSetOtherData(clkDataType _type, uint8_t _data, bool blink)
{
  _data %= 100;

  sscSetTag(_type);

#if defined(LCD_I2C_DISPLAY)
  clkDisplay.setDispData(2, ((_data / 10 == 0 || blink) ? 0x10
                                                        : _data / 10));
  clkDisplay.setDispData(3, ((blink) ? 0x10
                                     : _data % 10));
#else
  clkDisplay.setDispData(2, ((_data / 10 == 0 || blink) ? 0x00
                                                        : clkDisplay.encodeDigit(_data / 10)));
  clkDisplay.setDispData(3, ((blink) ? 0x00
                                     : clkDisplay.encodeDigit(_data % 10)));
#endif
}

void sscSetTag(clkDataType _type)
{
  switch (_type)
  {
#if __USE_AUTO_SHOW_DATA__
  case SET_AUTO_SHOW_PERIOD_TAG: // Au
#if defined(LCD_I2C_DISPLAY)
    clkDisplay.setDispData(0, 0x0A);
    clkDisplay.setDispData(1, 0x12);
    clkDisplay.setColon(true, LCD_COLON_COLON_1);
#else
    clkDisplay.setDispData(0, clkDisplay.encodeDigit(0x0A));
#if defined(TM1637_DISPLAY)
    clkDisplay.setDispData(1, 0b10011100);
#elif defined(MAX72XX_7SEGMENT_DISPLAY)
    clkDisplay.setDispData(1, 0b10011100);
#endif
#endif
    break;
#endif
#if defined(USE_ALARM)
  case SET_ALARM_TAG: // AL
#if defined(LCD_I2C_DISPLAY)
    clkDisplay.setDispData(0, 0x0A);
    clkDisplay.setDispData(1, 0x13);
    clkDisplay.setColon(true, LCD_COLON_COLON_1);
#else
    clkDisplay.setDispData(0, clkDisplay.encodeDigit(0x0A));
#if defined(TM1637_DISPLAY)
    clkDisplay.setDispData(1, 0b10111000);
#elif defined(MAX72XX_7SEGMENT_DISPLAY)
    clkDisplay.setDispData(1, 0b10001110);
#endif
#endif
    break;
#endif
#if __USE_LIGHT_SENSOR__
  case SET_LIGHT_THRESHOLD_TAG: // th
#if defined(TM1637_DISPLAY)
    clkDisplay.setDispData(0, 0b01111000);
    clkDisplay.setDispData(1, 0b11110100);
#elif defined(MAX72XX_7SEGMENT_DISPLAY)
    clkDisplay.setDispData(0, 0b00001111);
    clkDisplay.setDispData(1, 0b10010111);
#endif
    break;
#endif
#if __USE_SET_BRIGHTNESS_MODE__
  case SET_BRIGHTNESS_TAG: // br, b0, b1
    clkDisplay.setDispData(0, clkDisplay.encodeDigit(0x0B));
#if defined(TM1637_DISPLAY)
    clkDisplay.setDispData(1, 0b11010000);
#elif defined(MAX72XX_7SEGMENT_DISPLAY)
    clkDisplay.setDispData(1, 0b10000101);
#endif
#if __USE_LIGHT_SENSOR__
    uint8_t x;
    x = (ssc_display_mode == DISPLAY_MODE_SET_BRIGHTNESS_MIN) ? clkDisplay.encodeDigit(0)
                                                              : clkDisplay.encodeDigit(1);
    x |= 0x80;
    clkDisplay.setDispData(1, x);
#endif
    break;
#endif
  default:
    break;
  }
}

void sscSetOnOffData(clkDataType _type, bool _state, bool _blink)
{
  sscSetTag(_type);

#if defined(LCD_I2C_DISPLAY)
  clkDisplay.setDispData(2, 0x10);
  uint8_t x = 0x10;
#else
  clkDisplay.setDispData(2, 0x00);
  uint8_t x = 0x00;
#endif
  if (!_blink)
  {
#if defined(TM1637_DISPLAY)
    x = (_state) ? 0b01011100 : 0b00001000;
#elif defined(MAX72XX_7SEGMENT_DISPLAY)
    x = (_state) ? 0b00011101 : 0b00001000;
#elif defined(LCD_I2C_DISPLAY)
    x = (_state) ? 0x14 : 0x15;
#endif
  }
  clkDisplay.setDispData(3, x);
}

void sscShowTime(int8_t hour, int8_t minute, bool show_colon)
{
  clkDisplay.clear();

  if (hour >= 0)
  {
#if defined(LCD_I2C_DISPLAY)
    clkDisplay.setDispData(0, hour / 10);
    clkDisplay.setDispData(1, hour % 10);
#else
    clkDisplay.setDispData(0, clkDisplay.encodeDigit(hour / 10));
    uint8_t x = clkDisplay.encodeDigit(hour % 10);
    if (show_colon)
    {
      x |= 0x80; // для показа двоеточия установить старший бит во второй цифре
    }
    clkDisplay.setDispData(1, x);
#endif
  }
  if (minute >= 0)
  {
#if defined(LCD_I2C_DISPLAY)
    clkDisplay.setDispData(2, minute / 10);
    clkDisplay.setDispData(3, minute % 10);
    switch (ssc_display_mode)
    {
    case DISPLAY_MODE_SHOW_TIME:
      clkDisplay.setColon(show_colon);
      break;
#if defined(USE_CALENDAR)
    case DISPLAY_MODE_SET_DAY:
    case DISPLAY_MODE_SET_MONTH:
      clkDisplay.setColon(false, LCD_COLON_DOT);
      break;
    case DISPLAY_MODE_SET_YEAR:
#endif
    case DISPLAY_MODE_SET_HOUR:
    case DISPLAY_MODE_SET_MINUTE:
      clkDisplay.setColon(false, LCD_COLON_NO_COLON);
      break;
    default:
      break;
    }
#else
    clkDisplay.setDispData(2, clkDisplay.encodeDigit(minute / 10));
    clkDisplay.setDispData(3, clkDisplay.encodeDigit(minute % 10));
#endif
  }
}

#if defined(USE_CALENDAR)
void sscShowDate(shDateTime date)
{
  static uint8_t n = 0;

  clkDisplay.clear();

  switch (n)
  {
  case 0:
#if defined(LCD_I2C_DISPLAY)
    clkDisplay.setColon(false, LCD_COLON_DOT);
#endif
    sscShowTime(date.day(), date.month(), true);
    break;
  case 1:
#if defined(LCD_I2C_DISPLAY)
    clkDisplay.setColon(false, LCD_COLON_NO_COLON);
#endif
    sscShowTime(20, date.year() % 100, false);
    break;
  }

  if (++n >= 2)
  {
    n = 0;
  }
}
#endif

#if __USE_TEMP_DATA__
void sscShowTemp(int temp)
{
  clkDisplay.clear();

#if defined(LCD_I2C_DISPLAY)
  clkDisplay.setDispData(3, 0x17);

  uint8_t minus = 0x16;
  clkDisplay.setColon(false, LCD_COLON_NO_COLON);
#else
  clkDisplay.setDispData(3, 0x63);

  uint8_t minus = 0x40;
#endif

  // если температура отрицательная, сформировать минус впереди
  if (temp < 0)
  {
    temp = -temp;
    clkDisplay.setDispData(1, minus);
  }
  // если температура выходит за диапазон, сформировать строку минусов
  if (temp > 99)
  {
    for (uint8_t i = 0; i < 4; i++)
    {
      clkDisplay.setDispData(i, minus);
    }
  }
  else
  {
    if (temp > 9)
    {
      if (clkDisplay.getDispData(1) == minus)
      { // если температура ниже -9, переместить минус на крайнюю левую позицию
        clkDisplay.setDispData(0, minus);
      }
#if defined(LCD_I2C_DISPLAY)
      clkDisplay.setDispData(1, temp / 10);
#else
      clkDisplay.setDispData(1, clkDisplay.encodeDigit(temp / 10));
#endif
    }
#if defined(LCD_I2C_DISPLAY)
    clkDisplay.setDispData(2, temp % 10);
#else
    clkDisplay.setDispData(2, clkDisplay.encodeDigit(temp % 10));
#endif
  }
}
#endif

#endif
