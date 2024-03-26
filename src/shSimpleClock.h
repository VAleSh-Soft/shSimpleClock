#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <avr/pgmspace.h>
#include <shButton.h> // https://github.com/VAleSh-Soft/shButton
#include "_eeprom.h"
#include "shSimpleRTC.h"
#include "shClockEvent.h"
#include "clkTaskManager.h"

// ===================================================

enum clkDataType : uint8_t
{
  NO_TAG
#if defined(USE_TICKER_FOR_DATA)
  ,
  TICKER_TAG
#endif
#if defined(USE_ALARM)
  ,
  ALARM_TAG
#endif
#if USE_AUTO_SHOW_DATA
  ,
  AUTO_SHOW_PERIOD_TAG
#endif
#if defined(USE_SET_BRIGHTNESS_MODE)
  ,
  SET_BRIGHTNESS_TAG
#endif
#if defined(USE_LIGHT_SENSOR)
  ,
  SET_LIGHT_THRESHOLD_TAG
#endif
#if defined(WS2812_MATRIX_DISPLAY)
  ,
  SET_COLOR_OF_NUMBER_TAG
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
#if defined(USE_TEMP_DATA)
  ,
  DISPLAY_MODE_SHOW_TEMP // режим вывода температуры
#endif
#if USE_AUTO_SHOW_DATA
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
#if defined(USE_LIGHT_SENSOR)
  ,
  DISPLAY_MODE_SET_LIGHT_THRESHOLD // режим настройки порога переключения яркости
#endif
#if defined(USE_SET_BRIGHTNESS_MODE)
#if defined(USE_LIGHT_SENSOR)
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
#if USE_AUTO_SHOW_DATA
  ,
  DISPLAY_MODE_SET_AUTO_SHOW_PERIOD // режим настройки периода автовывода даты и/или температуры
#endif
#if defined(WS2812_MATRIX_DISPLAY)
  ,
  DISPLAY_MODE_SET_COLOR_OF_NUMBER // режим настройки цвета символов для адресных светодиодов
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
void sscShowTimeSetting();
void sscSetDisp();
void sscCheckButton();
void sscSetDisplay();
#if defined(USE_ALARM)
void sscCheckAlarm();
void sscRunAlarmBuzzer();
#endif
#if defined(USE_LIGHT_SENSOR)
void sscSetBrightness();
#endif
#if defined(USE_TEMP_DATA)
int8_t sscGetCurTemp();
#if defined(USE_DS18B20)
void sscCheckDS18b20();
#endif
#endif
void sscClearButtonFlag();
void sscStopSetting(clkHandle task);
#if USE_OTHER_SETTING
void sscShowOtherSetting();
#endif
#if USE_AUTO_SHOW_DATA
void sscAutoShowData();
uint8_t sscGetPeriodForAutoShow(uint8_t index);
#endif
void sscShowOnOffData(clkDataType _type, bool _state, bool blink);

#if USE_MATRIX_DISPLAY

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

#if defined(USE_TICKER_FOR_DATA)
void sscAssembleString(clkDisplayMode data_type, uint8_t lenght = 80);
void sscRunTicker();
#endif

#if defined(USE_CALENDAR)
void sscSetDayOfWeakString(uint8_t offset, uint8_t dow, bool toStringData = false);
void sscSetYearString(uint8_t offset, uint16_t _year, bool toStringData = false);
#endif

#if defined(USE_TEMP_DATA)
void sscSetTempString(uint8_t offset, int16_t temp, bool toStringData = false);
#endif

#if defined(WS2812_MATRIX_DISPLAY)
uint8_t sscGetIndexOfCurrentColorOfNumber();
#endif

#else

void sscSetOtherData(clkDataType _type, uint8_t _data, bool blink);
void sscSetTag(clkDataType _type);
void sscSetOnOffData(clkDataType _type, bool _state, bool _blink);
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
#if defined(USE_ALARM)
#include "alarm.h"
#endif
#if defined(USE_TEMP_DATA)
#if defined(USE_DS18B20)
#include "ds1820.h"
#elif defined(USE_NTC)
#include "ntc.h"
#endif
#endif

shClockEvent sscClockEvent;
shClockEvent sscAlarmEvent;

clkTaskManager sscTasks;

// -----------------------------------------
bool sscBlinkFlag = false; // флаг блинка, используется всем, что должно мигать

// ---- экраны -----------------------------
#if defined(TM1637_DISPLAY)
DisplayTM1637 clkDisp(DISPLAY_CLK_PIN, DISPLAY_DAT_PIN);
#elif defined(MAX72XX_7SEGMENT_DISPLAY)
DisplayMAX72xx7segment<DISPLAY_CS_PIN> clkDisp;
#elif defined(MAX72XX_MATRIX_DISPLAY)
DisplayMAX72xxMatrix<DISPLAY_CS_PIN> clkDisp;
#elif defined(WS2812_MATRIX_DISPLAY)
CRGB ssc_leds[256];
DisplayWS2812Matrix clkDisp(ssc_leds, COLOR_OF_NUMBER, MX_TYPE);
#endif

// ---- модуль RTC -------------------------
shSimpleRTC sscClock;

// ---- будильник --------------------------
#if defined(USE_ALARM)
Alarm sscAlarm(ALARM_LED_PIN, ALARM_DATA_EEPROM_INDEX);
#endif

// ---- датчики температуры ----------------
#if defined(USE_TEMP_DATA)
#if defined(USE_DS18B20)
DS1820 sscTempSensor(DS18B20_PIN);
#elif defined(USE_NTC)
NTCSensor sscTempSensor(NTC_PIN,
                        RESISTOR_STD,
                        BALANCE_RESISTOR,
                        BETA_COEFFICIENT);
#endif
#endif

// ==== clkButton ====================================
enum clkButtonFlag : uint8_t
{
  CLK_BTN_FLAG_NONE, // флаг кнопки - ничего не делать
  CLK_BTN_FLAG_NEXT, // флаг кнопки - изменить значение
  CLK_BTN_FLAG_EXIT  // флаг кнопки - возврат в режим показа текущего времени
};

class clkButton : public shButton
{
private:
  clkButtonFlag _flag = CLK_BTN_FLAG_NONE;

public:
  clkButton(uint8_t button_pin, bool serial_mode = false)
      : shButton(button_pin, BTN_INPUT_TYPE, BTN_TYPE)
  {
    shButton::setTimeoutOfLongClick(TIMEOUT_OF_LONGCLICK);
    shButton::setLongClickMode(LCM_ONLYONCE);
    shButton::setVirtualClickOn(true);
    shButton::setTimeoutOfDebounce(TIMEOUT_OF_DEBOUNCE);
    if (serial_mode)
    {
      shButton::setLongClickMode(LCM_CLICKSERIES);
      shButton::setIntervalOfSerial(INTERVAL_OF_SERIAL);
    }
  }

  clkButtonFlag getButtonFlag(bool _clear = false)
  {
    clkButtonFlag result = _flag;
    if (_clear)
    {
      _flag = CLK_BTN_FLAG_NONE;
    }
    return (result);
  }

  void setButtonFlag(clkButtonFlag flag)
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
      if (sscTasks.getTaskState(sscTasks.return_to_default_mode()))
      {
        sscTasks.startTask(sscTasks.return_to_default_mode());
      }
#if defined(USE_BUZZER_FOR_BUTTON)
      if (_state != BTN_LONGCLICK)
      {
        tone(BUZZER_PIN, 2000, 25); // на каждый клик срабатывает пищалка
      }
#endif
#if defined(USE_ALARM)
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

// ==== clkButtonGroup ===============================
enum clkButtonType : uint8_t
{
  CLK_BTN_SET,
  CLK_BTN_UP,
  CLK_BTN_DOWN
};

class clkButtonGroup
{
private:
  clkButton *buttons[3] = {NULL, NULL, NULL};

  clkButton *btn_set = NULL;
  clkButton *btn_up = NULL;
  clkButton *btn_down = NULL;

  bool isValid(clkButtonType _btn)
  {
    bool result = false;

    if (buttons != NULL)
    {
      result = buttons[(byte)_btn] != NULL;
    }

    return result;
  }

public:
  clkButtonGroup() {}

  void addButton(clkButtonType _btn)
  {
    switch (_btn)
    {
    case CLK_BTN_SET:
      if (BTN_SET_PIN >= 0)
      {
        btn_set = (clkButton *)calloc(1, sizeof(clkButton));
      }
      if (btn_set != NULL)
      {
        btn_set = &(clkButton){BTN_SET_PIN};
        buttons[0] = btn_set;
      }
      break;
    case CLK_BTN_UP:
      if (BTN_UP_PIN >= 0)
      {
        btn_up = (clkButton *)calloc(1, sizeof(clkButton));
      }
      if (btn_up != NULL)
      {
        btn_up = &(clkButton){BTN_UP_PIN, true};
        buttons[1] = btn_up;
      }
      break;
    case CLK_BTN_DOWN:
      if (BTN_DOWN_PIN >= 0)
      {
        btn_down = (clkButton *)calloc(1, sizeof(clkButton));
      }
      if (btn_down != NULL)
      {
        btn_down = &(clkButton){BTN_DOWN_PIN, true};
        buttons[2] = btn_down;
      }
      break;
    }
  }

  void setButtonFlag(clkButtonType _btn, clkButtonFlag _flag)
  {
    if (isValid(_btn))
    {
      buttons[(byte)_btn]->setButtonFlag(_flag);
    }
  }

  clkButtonFlag getButtonFlag(clkButtonType _btn, bool _clear = false)
  {
    clkButtonFlag result = CLK_BTN_FLAG_NONE;
    if (isValid(_btn))
    {
      result = buttons[(byte)_btn]->getButtonFlag(_clear);
    }
    return result;
  }

  uint8_t getButtonState(clkButtonType _btn)
  {
    uint8_t result = BTN_RELEASED;
    if (isValid(_btn))
    {
      result = buttons[(byte)_btn]->getButtonState();
    }
    return result;
  }

  uint8_t getLastState(clkButtonType _btn)
  {
    uint8_t result = BTN_RELEASED;
    if (isValid(_btn))
    {
      result = buttons[(byte)_btn]->getLastState();
    }
    return result;
  }

  void resetButtonState(clkButtonType _btn)
  {
    if (isValid(_btn))
    {
      buttons[(byte)_btn]->resetButtonState();
    }
  }

  bool isButtonClosed(clkButtonType _btn)
  {
    bool result = false;
    if (isValid(_btn))
    {
      result = buttons[(byte)_btn]->isButtonClosed();
    }
    return result;
  }

  bool isSecondButtonPressed(clkButtonType _btn1,
                             clkButtonType _btn2,
                             uint8_t _state)
  {
    bool result = false;

    if (isValid(_btn1) && isValid(_btn2))
    {
      result = buttons[(byte)_btn1]->isSecondButtonPressed(*buttons[(byte)_btn2],
                                                           _state);
    }

    return result;
  }
};
// ==== end clkButtonGroup ===========================

clkDisplayMode ssc_display_mode = DISPLAY_MODE_SHOW_TIME;

clkButtonGroup buttons;

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
    // ==== кнопки ===================================
    buttons.addButton(CLK_BTN_SET);
    buttons.addButton(CLK_BTN_UP);
    buttons.addButton(CLK_BTN_DOWN);

    // ==== RTC ======================================
    Wire.begin();
#if defined(RTC_DS3231)
    sscClock.setClockMode(false);
#endif
    sscRtcNow();

    // ==== валидация EEPROM =========================
#if USE_AUTO_SHOW_DATA
    if (read_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX) > 7)
    {
      write_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX, 1);
    }
#endif

#if defined(USE_LIGHT_SENSOR)
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

// ==== датчики ====================================
#if defined(USE_NTC)
    sscTempSensor.setADCbitDepth(BIT_DEPTH); // установить разрядность АЦП вашего МК, для AVR обычно равна 10 бит
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
#if defined(USE_LIGHT_SENSOR)
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
    clkDisp.setBrightness(read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX));
#endif

// ==== экраны =======================================
#if defined(WS2812_MATRIX_DISPLAY)
    setFastLEDData(ssc_leds, 256);
#elif defined(MAX72XX_MATRIX_DISPLAY) || defined(MAX72XX_7SEGMENT_DISPLAY)
    clkDisp.shutdownAllDevices(false);
#if defined(MAX72XX_MATRIX_DISPLAY)
    clkDisp.setDirection(2);
    clkDisp.setFlip(false);
#endif
#endif

// выставить яркость в минимум, чтобы при включении не сверкало максимальной яркостью
#if defined(WS2812_MATRIX_DISPLAY)
    FastLED.setBrightness(0);
#elif defined(MAX72XX_MATRIX_DISPLAY) || defined(MAX72XX_7SEGMENT_DISPLAY)
    clkDisp.setBrightness(0);
#else
    clkDisp.setBrightness(1);
#endif

    // ==== задачи ===================================
    uint8_t task_count = 5; // базовое количество задач
#if defined(USE_ALARM)
    task_count += 2;
#endif
#if USE_AUTO_SHOW_DATA
    task_count++;
#endif
#if defined(USE_TEMP_DATA) && defined(USE_DS18B20)
    task_count++;
#endif
#if defined(USE_LIGHT_SENSOR)
    task_count++;
#endif
#if USE_OTHER_SETTING
    task_count++;
#endif
#if defined(USE_TICKER_FOR_DATA)
    task_count++;
#endif
    sscTasks.init(task_count);

    sscTasks.rtc_guard(sscTasks.addTask(50ul, sscRtcNow));
    sscTasks.blink_timer(sscTasks.addTask(50ul, sscBlink));
    sscTasks.return_to_default_mode(sscTasks.addTask(AUTO_EXIT_TIMEOUT * 1000ul,
                                                     sscReturnToDefMode,
                                                     false));
    sscTasks.set_time_mode(sscTasks.addTask(100ul, sscShowTimeSetting, false));
#if defined(USE_TEMP_DATA) && defined(USE_DS18B20)
    sscTasks.ds18b20_guard(sscTasks.addTask(3000ul, sscCheckDS18b20));
#endif
#if USE_AUTO_SHOW_DATA
    sscTasks.auto_show_mode(sscTasks.addTask(100ul, sscAutoShowData, false));
#endif
#if defined(USE_ALARM)
    sscTasks.alarm_guard(sscTasks.addTask(200ul, sscCheckAlarm));
    sscTasks.alarm_buzzer(sscTasks.addTask(50ul, sscRunAlarmBuzzer, false));
#endif
    sscTasks.display_guard(sscTasks.addTask(50ul, sscSetDisp));
#if defined(USE_LIGHT_SENSOR)
    sscTasks.light_sensor_guard(sscTasks.addTask(100ul, sscSetBrightness));
#else
    clkDisp.setBrightness(read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX));
#endif
#if USE_OTHER_SETTING
    sscTasks.other_setting_mode(sscTasks.addTask(100ul, sscShowOtherSetting, false));
#endif
#if defined(USE_TICKER_FOR_DATA)
    sscTasks.ticker(sscTasks.addTask(1000ul / TICKER_SPEED, sscRunTicker, false));
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
  void setClockEvent(uint16_t _interval,
                     sceCallback _callback,
                     bool _active = true)
  {
    sscClockEvent.init(_interval, _callback, _active);
  }

  /**
   * @brief установить статус ежесекундного события
   *
   * @param _state true - событие активно; false - событие не активно
   */
  void setClockEventState(bool _state) { sscClockEvent.setState(_state); }

  /**
   * @brief получить текущее состояние или событие кнопки
   *
   * @param _btn идентификатор кнопки, может иметь значение: CLK_BTN_SET, CLK_BTN_UP, CLK_BTN_DOWN;
   * @return uint8_t
   */
  uint8_t getButtonState(clkButtonType _btn)
  {
    return (buttons.getLastState(_btn));
  }

  /**
   * @brief получить флаг кнопки
   *
   * @param _btn идентификатор кнопки, может иметь значение: CLK_BTN_SET, CLK_BTN_UP, CLK_BTN_DOWN;
   * @param _clear если true, то флаг кнопки после считывания будет очищен (установлено значение CLK_BTN_FLAG_NONE);
   * @return clkButtonFlag возможные варианты: CLK_BTN_FLAG_NONE, CLK_BTN_FLAG_NEXT, CLK_BTN_FLAG_EXIT
   */
  clkButtonFlag getButtonFlag(clkButtonType _btn, bool _clear = false)
  {
    return buttons.getButtonFlag(_btn, _clear);
  }

  /**
   * @brief установить флаг кнопки
   *
   * @param _btn  идентификатор кнопки, может иметь значение: CLK_BTN_SET, CLK_BTN_UP, CLK_BTN_DOWN;
   * @param _flag устанавливаемый флаг; возможные варианты: CLK_BTN_FLAG_NONE, CLK_BTN_FLAG_NEXT, CLK_BTN_FLAG_EXIT
   */
  void setButtonFlag(clkButtonType _btn, clkButtonFlag _flag)
  {
    buttons.setButtonFlag(_btn, _flag);
  }

#if defined(MAX72XX_MATRIX_DISPLAY)
  /**
   * @brief установить угол поворота изображения
   *
   * @param _dir угол поворота изображения, 0..3
   */
  void setMatrixDirection(uint8_t _dir)
  {
    clkDisp.setDirection(_dir);
  }

  /**
   * @brief включить отражение изображения по горизонтали (по строкам)
   *
   * @param _mode true - включить отражение, false - отключить отражение
   */
  void setMatrixFlipMode(bool _mode)
  {
    clkDisp.setFlip(_mode);
  }
#endif

#if defined(WS2812_MATRIX_DISPLAY)
  /**
   * @brief установить цвет для вывода информации на матрице из адресных светодиодов
   *
   * @param _color цвет, например, для красного CRGB::Red
   */
  void setColorOfNumber(CRGB _color)
  {
    clkDisp.setColorOfNumber(_color);
    write_eeprom_crgb(COLOR_OF_NUMBER_VALUE_EEPROM_INDEX, _color);
  }

  /**
   * @brief получение текущего цвета символов экрана из адресных светдиодов
   *
   * @return CRGB
   */
  CRGB getColorOfNumber()
  {
    return (clkDisp.getColorOfNumber());
  }

  /**
   * @brief установить цвет фона на матрице из адресных светодиодов
   *
   * @param _color цвет, например, для синего CRGB::Blue
   */
  void setColorOfBackground(CRGB _color)
  {
    clkDisp.setColorOfBackground(_color);
    write_eeprom_crgb(COLOR_OF_BACKGROUND_VALUE_EEPROM_INDEX, _color);
  }

  /**
   * @brief получение текущего цвета фона экрана из адресных светдиодов
   *
   * @return CRGB
   */
  CRGB getColorOfBackground()
  {
    return (clkDisp.getColorOfBackground());
  }

  /**
   * @brief установка максимальной мощности блока питания матрицы
   *
   * @param volts напряжение, Вольт
   * @param milliamps максимальный ток, милиампер
   */
  void setMaxPSP(uint8_t volts, uint32_t milliamps)
  {
    clkDisp.setMaxPSP(volts, milliamps);
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

#if defined(USE_CALENDAR)
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

#if defined(USE_TEMP_DATA)
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

#if defined(USE_ALARM)
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

#if defined(USE_SET_BRIGHTNESS_MODE)
  /**
   * @brief получение значения максимальной яркости экрана
   *
   * @return uint8_t
   */
  uint8_t getBrightnessMax()
  {
    return (read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX));
  }

  /**
   * @brief установка максимальной яркости экрана
   *
   * @param _br новое значение
   */
  void setBrightnessMax(uint8_t _br)
  {
    write_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX, _br);
#if defined(WS2812_MATRIX_DISPLAY)
    _br *= 10;
#endif
    clkDisp.setBrightness(_br);
  }

#if defined(USE_LIGHT_SENSOR)
  /**
   * @brief получение значения минимальной яркости экрана
   *
   * @return uint8_t
   */
  uint8_t getBrightnessMin()
  {
    return (read_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX));
  }

  /**
   * @brief установка минимальной яркости экрана
   *
   * @param _br
   */
  void setBrightnessMin(uint8_t _br)
  {
    write_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX, _br);
#if defined(WS2812_MATRIX_DISPLAY)
    _br *= 10;
#endif
    clkDisp.setBrightness(_br);
  }
#endif
#endif

#if defined(USE_LIGHT_SENSOR)
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

#if USE_MATRIX_DISPLAY

#if defined(USE_TICKER_FOR_DATA)
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
#if defined(USE_TICKER_FOR_DATA)
    // если работает бегущая строка, ничего не делать
    if (sscTasks.getTaskState(sscTasks.ticker()))
    {
      return;
    }
#endif

#if USE_AUTO_SHOW_DATA
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
#if USE_MATRIX_DISPLAY
      clkDisp.showTime(sscClock.getCurTime().hour(),
                       sscClock.getCurTime().minute(),
                       sscClock.getCurTime().second(),
                       sscBlinkFlag);
#else
      clkDisp.showTime(sscClock.getCurTime().hour(),
                       sscClock.getCurTime().minute(),
                       sscBlinkFlag);
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
#if defined(USE_LIGHT_SENSOR)
  case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
#endif
#if defined(USE_SET_BRIGHTNESS_MODE)
#if defined(USE_LIGHT_SENSOR)
  case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
#endif
#if defined(USE_TICKER_FOR_DATA)
  case DISPLAY_MODE_SET_TICKER_ON_OFF:
#endif
#if USE_AUTO_SHOW_DATA
  case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
#endif
#if defined(WS2812_MATRIX_DISPLAY)
  case DISPLAY_MODE_SET_COLOR_OF_NUMBER:
#endif
    buttons.setButtonFlag(CLK_BTN_SET, CLK_BTN_FLAG_EXIT);
    break;
#if defined(USE_TEMP_DATA)
  case DISPLAY_MODE_SHOW_TEMP:
#endif
#if defined(USE_CALENDAR)
  case DISPLAY_MODE_SHOW_DATE:
#endif
#if USE_AUTO_SHOW_DATA
  case DISPLAY_AUTO_SHOW_DATA:
    sscTasks.stopTask(sscTasks.auto_show_mode());
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
  sscTasks.stopTask(sscTasks.return_to_default_mode());
}

void sscShowTimeData(uint8_t hour, uint8_t minute)
{
  // если наступило время блинка и кнопки Up/Down не нажаты, то стереть соответствующие разряды; при нажатых кнопках Up/Down во время изменения данных ничего не мигает
  if (!sscBlinkFlag &&
      !buttons.isButtonClosed(CLK_BTN_UP) &&
      !buttons.isButtonClosed(CLK_BTN_DOWN))
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

  bool toDate = false;
  bool toColon = false;
#if defined(USE_CALENDAR)
  toDate = (ssc_display_mode >= DISPLAY_MODE_SET_DAY &&
            ssc_display_mode <= DISPLAY_MODE_SET_YEAR);
  toColon = ssc_display_mode != DISPLAY_MODE_SET_YEAR;
#endif
#if USE_MATRIX_DISPLAY
#if defined(USE_CALENDAR)
  if (ssc_display_mode == DISPLAY_MODE_SET_YEAR)
  {
    clkDisp.showYear(minute);
  }
  else
#endif
  {
    clkDisp.showTime(hour, minute, 0, toColon, toDate);
  }
#else
  clkDisp.showTime(hour, minute, toColon);
#endif
}

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
  buttons.setButtonFlag(CLK_BTN_SET, CLK_BTN_FLAG_NONE);
  buttons.setButtonFlag(CLK_BTN_UP, CLK_BTN_FLAG_NONE);
  buttons.setButtonFlag(CLK_BTN_DOWN, CLK_BTN_FLAG_NONE);
}

void sscStopSetting(clkHandle task)
{
  sscTasks.stopTask(task);
  sscTasks.stopTask(sscTasks.return_to_default_mode());
  sscClearButtonFlag();
}

// ==== sscShowTimeSetting ===========================
void _startTimeSettingMode(uint8_t &curHour, uint8_t &curMinute)
{
  sscTasks.startTask(sscTasks.set_time_mode());
  sscTasks.startTask(sscTasks.return_to_default_mode());
  sscClearButtonFlag();
  switch (ssc_display_mode)
  {
#if defined(USE_ALARM)
  case DISPLAY_MODE_SET_ALARM_HOUR:
  case DISPLAY_MODE_SET_ALARM_MINUTE:
    curHour = sscAlarm.getAlarmPoint() / 60;
    curMinute = sscAlarm.getAlarmPoint() % 60;
    break;
  case DISPLAY_MODE_ALARM_ON_OFF:
    curHour = (uint8_t)sscAlarm.getOnOffAlarm();
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
  if (buttons.getButtonFlag(CLK_BTN_SET) > CLK_BTN_FLAG_NONE)
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
#if defined(USE_CALENDAR)
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
#if defined(USE_ALARM)
      case DISPLAY_MODE_SET_ALARM_HOUR:
      case DISPLAY_MODE_SET_ALARM_MINUTE:
        sscAlarm.setAlarmPoint(curHour * 60 + curMinute);
        break;
      case DISPLAY_MODE_ALARM_ON_OFF:
        sscAlarm.setOnOffAlarm((bool)curHour);
        break;
#endif
#if defined(USE_TICKER_FOR_DATA)
      case DISPLAY_MODE_SET_TICKER_ON_OFF:
        EEPROM.write(TICKER_STATE_VALUE_EEPROM_INDEX, curHour);
        break;
#endif
      default:
        break;
      }
      time_checked = false;
    }
    if (buttons.getButtonFlag(CLK_BTN_SET, true) == CLK_BTN_FLAG_NEXT)
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
        sscStopSetting(sscTasks.set_time_mode());
        break;
#if defined(USE_ALARM)
      case DISPLAY_MODE_ALARM_ON_OFF:
        ssc_display_mode = (curHour) ? DISPLAY_MODE_SET_ALARM_HOUR
                                     : DISPLAY_MODE_SHOW_TIME;
        sscStopSetting(sscTasks.set_time_mode());
        break;
#endif
#if defined(USE_TICKER_FOR_DATA)
      case DISPLAY_MODE_SET_TICKER_ON_OFF:
#if USE_AUTO_SHOW_DATA
        ssc_display_mode = DISPLAY_MODE_SET_AUTO_SHOW_PERIOD;
#elif defined(WS2812_MATRIX_DISPLAY)
        ssc_display_mode = DISPLAY_MODE_SET_COLOR_OF_NUMBER;
#else
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
#endif
        sscStopSetting(sscTasks.set_time_mode());
        break;
#endif
#if USE_AUTO_SHOW_DATA
      case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
        sscStopSetting(sscTasks.set_time_mode());
        break;
#endif
      default:
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
        sscStopSetting(sscTasks.set_time_mode());
        break;
      }
    }
    else
    {
      ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
      sscStopSetting(sscTasks.set_time_mode());
    }
  }
}

void _checkBtnUpDownForTmSet(uint8_t &curHour,
                             uint8_t &curMinute,
                             bool &time_checked)
{
#if defined(USE_CALENDAR)
  static const uint8_t PROGMEM days_of_month[] =
      {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
#endif
  if ((buttons.getButtonFlag(CLK_BTN_UP) == CLK_BTN_FLAG_NEXT) ||
      (buttons.getButtonFlag(CLK_BTN_DOWN, true) == CLK_BTN_FLAG_NEXT))
  {
    bool dir = buttons.getButtonFlag(CLK_BTN_UP, true) == CLK_BTN_FLAG_NEXT;
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
#if defined(USE_ALARM)
    case DISPLAY_MODE_ALARM_ON_OFF:
      sscCheckData(curHour, 1, true);
      break;
#endif
#if defined(USE_CALENDAR)
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
#if defined(USE_TICKER_FOR_DATA)
    case DISPLAY_MODE_SET_TICKER_ON_OFF:
      sscCheckData(curHour, 1, true);
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
  else if (sscTasks.getTaskState(sscTasks.set_time_mode()))
  {
    switch (ssc_display_mode)
    {
#if defined(USE_ALARM)
    case DISPLAY_MODE_ALARM_ON_OFF:
      sscShowOnOffData(ALARM_TAG,
                       curHour,
                       (!sscBlinkFlag &&
                        !buttons.isButtonClosed(CLK_BTN_UP) &&
                        !buttons.isButtonClosed(CLK_BTN_DOWN)));
      break;
#endif
#if defined(USE_TICKER_FOR_DATA)
    case DISPLAY_MODE_SET_TICKER_ON_OFF:
      sscShowOnOffData(TICKER_TAG,
                       curHour,
                       (!sscBlinkFlag &&
                        !buttons.isButtonClosed(CLK_BTN_UP) &&
                        !buttons.isButtonClosed(CLK_BTN_DOWN)));
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

  if (!sscTasks.getTaskState(sscTasks.set_time_mode()))
  {
    _startTimeSettingMode(curHour, curMinute);
    time_checked = false;
#if !USE_MATRIX_DISPLAY
    clkDisp.sleep(); // слегка мигнуть экраном при входе в настройки
    return;
#endif
  }

#if defined(USE_TICKER_FOR_DATA)
  // подождать, пока отработает бегущая строка
  if (sscTasks.getTaskState(sscTasks.ticker()))
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
#if defined(USE_CALENDAR)
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
#if defined(USE_TICKER_FOR_DATA)
  // обновление экрана делать только если в данный момент не работает бегущая строка, она сама обновляет экран, когда ей это нужно
  if (!sscTasks.getTaskState(sscTasks.ticker()))
#endif
    clkDisp.show();
}

void sscCheckSetButton()
{
  switch (buttons.getButtonState(CLK_BTN_SET))
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
#if defined(USE_SET_BRIGHTNESS_MODE)
#if defined(USE_LIGHT_SENSOR)
    case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
    case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
#endif
#if defined(USE_LIGHT_SENSOR)
    case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
#endif
#if defined(USE_TICKER_FOR_DATA)
    case DISPLAY_MODE_SET_TICKER_ON_OFF:
#endif
#if USE_AUTO_SHOW_DATA
    case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
#endif
#if defined(WS2812_MATRIX_DISPLAY)
    case DISPLAY_MODE_SET_COLOR_OF_NUMBER:
#endif
    case DISPLAY_MODE_CUSTOM_1:
    case DISPLAY_MODE_CUSTOM_2:
    case DISPLAY_MODE_CUSTOM_3:
    case DISPLAY_MODE_CUSTOM_4:
      buttons.setButtonFlag(CLK_BTN_SET, CLK_BTN_FLAG_NEXT);
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
#if USE_AUTO_SHOW_DATA
      sscTasks.stopTask(sscTasks.auto_show_mode());
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
#if USE_AUTO_SHOW_DATA
    case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
#endif
#if defined(USE_LIGHT_SENSOR)
    case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
#endif
#if defined(WS2812_MATRIX_DISPLAY)
    case DISPLAY_MODE_SET_COLOR_OF_NUMBER:
#endif
    case DISPLAY_MODE_CUSTOM_1:
    case DISPLAY_MODE_CUSTOM_2:
    case DISPLAY_MODE_CUSTOM_3:
    case DISPLAY_MODE_CUSTOM_4:
      buttons.setButtonFlag(CLK_BTN_SET, CLK_BTN_FLAG_EXIT);
      break;
    default:
      break;
    }
    break;
  }
}

void sscCheckUDbtn(clkButtonType btn)
{
  switch (buttons.getLastState(btn))
  {
  case BTN_DOWN:
  case BTN_DBLCLICK:
    buttons.setButtonFlag(btn, CLK_BTN_FLAG_NEXT);
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
      return;
    default:
      buttons.setButtonFlag(btn, CLK_BTN_FLAG_NEXT);
      break;
    }
    break;
  }
}

void sscCheckUpDownButton()
{
  buttons.getButtonState(CLK_BTN_UP);
  buttons.getButtonState(CLK_BTN_DOWN);

  switch (ssc_display_mode)
  {
  case DISPLAY_MODE_SHOW_TIME:
    if (buttons.getLastState(CLK_BTN_DOWN) == BTN_LONGCLICK &&
        !buttons.isButtonClosed(CLK_BTN_UP))
    {
#if defined(USE_TICKER_FOR_DATA)
      // вход в настройки анимации
      ssc_display_mode = DISPLAY_MODE_SET_TICKER_ON_OFF;
      buttons.resetButtonState(CLK_BTN_DOWN);
#elif USE_AUTO_SHOW_DATA
      // вход в настройки периода автовывода на экран даты и/или температуры
      ssc_display_mode = DISPLAY_MODE_SET_AUTO_SHOW_PERIOD;
      buttons.resetButtonState(CLK_BTN_DOWN);
#elif defined(WS2812_MATRIX_DISPLAY)
      disp = DISPLAY_MODE_SET_COLOR_OF_NUMBER;
      buttons.resetButtonState(CLK_BTN_DOWN);
#endif
    }
#if defined(USE_TEMP_DATA)
    if (buttons.getLastState(CLK_BTN_UP) == BTN_ONECLICK)
    {
      ssc_display_mode = DISPLAY_MODE_SHOW_TEMP;
    }
#endif
#if defined(USE_CALENDAR)
    if (buttons.getLastState(CLK_BTN_DOWN) == BTN_ONECLICK)
    {
      ssc_display_mode = DISPLAY_MODE_SHOW_DATE;
    }
#endif
    if (buttons.isSecondButtonPressed(CLK_BTN_UP, CLK_BTN_DOWN, BTN_LONGCLICK) ||
        buttons.isSecondButtonPressed(CLK_BTN_DOWN, CLK_BTN_UP, BTN_LONGCLICK))
    {
#if defined(USE_SET_BRIGHTNESS_MODE)
#if defined(USE_LIGHT_SENSOR)
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
#if defined(USE_LIGHT_SENSOR)
  case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
#endif
#if defined(USE_SET_BRIGHTNESS_MODE)
#if defined(USE_LIGHT_SENSOR)
  case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
#endif
#if defined(USE_TICKER_FOR_DATA)
  case DISPLAY_MODE_SET_TICKER_ON_OFF:
#endif
#if USE_AUTO_SHOW_DATA
  case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
#endif
#if defined(WS2812_MATRIX_DISPLAY)
  case DISPLAY_MODE_SET_COLOR_OF_NUMBER:
#endif
  case DISPLAY_MODE_CUSTOM_1:
  case DISPLAY_MODE_CUSTOM_2:
  case DISPLAY_MODE_CUSTOM_3:
  case DISPLAY_MODE_CUSTOM_4:
    if (!buttons.isButtonClosed(CLK_BTN_DOWN))
    {
      sscCheckUDbtn(CLK_BTN_UP);
    }
    if (!buttons.isButtonClosed(CLK_BTN_UP))
    {
      sscCheckUDbtn(CLK_BTN_DOWN);
    }
    break;
#if defined(USE_TEMP_DATA)
  case DISPLAY_MODE_SHOW_TEMP:
    if (buttons.getLastState(CLK_BTN_UP) == BTN_ONECLICK)
    {
      sscReturnToDefMode();
    }
    break;
#endif
#if defined(USE_CALENDAR)
  case DISPLAY_MODE_SHOW_DATE:
    if (buttons.getLastState(CLK_BTN_DOWN) == BTN_ONECLICK)
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
    if (!sscTasks.getTaskState(sscTasks.set_time_mode()))
    {
      sscShowTimeSetting();
    }
    break;
#if USE_AUTO_SHOW_DATA
  case DISPLAY_AUTO_SHOW_DATA:
#if defined(USE_CALENDAR)
  case DISPLAY_MODE_SHOW_DATE:
#endif
#if defined(USE_TEMP_DATA)
  case DISPLAY_MODE_SHOW_TEMP:
#endif
    if (!sscTasks.getTaskState(sscTasks.auto_show_mode()))
    {
      sscAutoShowData();
    }
    break;
#endif
#if USE_OTHER_SETTING
#if USE_AUTO_SHOW_DATA
  case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
#endif
#if defined(USE_LIGHT_SENSOR)
  case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
#endif
#if defined(USE_SET_BRIGHTNESS_MODE)
#if defined(USE_LIGHT_SENSOR)
  case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
#endif
#if defined(WS2812_MATRIX_DISPLAY)
  case DISPLAY_MODE_SET_COLOR_OF_NUMBER:
#endif
    if (!sscTasks.getTaskState(sscTasks.other_setting_mode()))
    {
      sscShowOtherSetting();
    }
    break;
#endif
  default:
    break;
  }
}

#if defined(USE_CALENDAR)
#endif

#if defined(USE_ALARM)
void sscCheckAlarm()
{
  sscAlarm.tick(sscClock.getCurTime());
  if (sscAlarm.getAlarmState() == ALARM_YES &&
      !sscTasks.getTaskState(sscTasks.alarm_buzzer()))
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

  if (!sscTasks.getTaskState(sscTasks.alarm_buzzer()))
  {
    sscTasks.startTask(sscTasks.alarm_buzzer());
    n = 0;
    k = 0;
    m = 0;
  }
  else if (sscAlarm.getAlarmState() == ALARM_ON)
  { // остановка пищалки, если будильник отключен
    sscTasks.stopTask(sscTasks.alarm_buzzer());
    return;
  }

  tone(BUZZER_PIN,
       pgm_read_dword(&pick[0][n]),
       pgm_read_dword(&pick[1][n]));
  sscTasks.setTaskInterval(sscTasks.alarm_buzzer(),
                           pgm_read_dword(&pick[1][n]), true);
  if (++n >= 8)
  {
    n = 0;
    if (++k >= ALARM_DURATION)
    { // приостановка пищалки через заданное число секунд
      k = 0;
      if (++m >= ALARM_REPETITION_COUNT)
      { // отключение пищалки после заданного количества срабатываний
        sscTasks.stopTask(sscTasks.alarm_buzzer());
        sscTasks.setTaskInterval(sscTasks.alarm_buzzer(), 50, false);
        sscAlarm.setAlarmState(ALARM_ON);
      }
      else
      {
        sscTasks.setTaskInterval(sscTasks.alarm_buzzer(),
                                 ALARM_SNOOZE_DELAY * 1000ul,
                                 true);
      }
    }
  }
}

#endif

#if defined(USE_LIGHT_SENSOR)
void sscSetBrightness()
{
#if defined(USE_SET_BRIGHTNESS_MODE)
  if (sscTasks.getTaskState(sscTasks.other_setting_mode()))
  {
    if (ssc_display_mode == DISPLAY_MODE_SET_BRIGHTNESS_MAX ||
        ssc_display_mode == DISPLAY_MODE_SET_BRIGHTNESS_MIN ||
        ssc_display_mode == DISPLAY_MODE_SET_LIGHT_THRESHOLD)
    {
      return; // в режиме настройки яркости и порога переключения ничего не регулировать
    }
  }
#endif

  uint8_t x = 1;
#if defined(USE_LIGHT_SENSOR)
  static uint16_t b;
  b = (b * 2 + analogRead(LIGHT_SENSOR_PIN)) / 3;
  if (b < read_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX) * 100)
  {
    x = read_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX);
  }
  else if (b > (read_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX) * 100u + 50))
#endif
  {
    x = read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX);
  }
  clkDisp.setBrightness(x);
}

#endif

#if defined(USE_TEMP_DATA)

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
  result = sscClock.getTemperature();
#endif

  return result;
}
#endif

#if USE_OTHER_SETTING

// ==== sscShowOtherSetting =============================
void _startOtherSettingMode(uint8_t &x)
{
  sscTasks.startTask(sscTasks.other_setting_mode());
  sscTasks.startTask(sscTasks.return_to_default_mode());
  sscClearButtonFlag();
  switch (ssc_display_mode)
  {
#if defined(USE_LIGHT_SENSOR)
  case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
    x = read_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX);
    break;
#endif
#if defined(USE_SET_BRIGHTNESS_MODE)
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
    x = read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX);
    break;
#if defined(USE_LIGHT_SENSOR)
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
  if (buttons.getButtonFlag(CLK_BTN_SET) > CLK_BTN_FLAG_NONE)
  {
    bool _next = buttons.getButtonFlag(CLK_BTN_SET, true) == CLK_BTN_FLAG_NEXT;
    switch (ssc_display_mode)
    {
#if defined(USE_LIGHT_SENSOR)
    case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
      write_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX, x);
      ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
      sscStopSetting(sscTasks.other_setting_mode());
      break;
#endif
#if defined(USE_SET_BRIGHTNESS_MODE)
    case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
      write_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX, x);
#if defined(USE_LIGHT_SENSOR)
      if (_next)
      {
        ssc_display_mode = DISPLAY_MODE_SET_LIGHT_THRESHOLD;
      }
      else
#endif
      {
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
      }
      sscStopSetting(sscTasks.other_setting_mode());
      break;
#if defined(USE_LIGHT_SENSOR)
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
      sscStopSetting(sscTasks.other_setting_mode());
      break;
#endif
#endif
#if USE_AUTO_SHOW_DATA
    case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
      write_eeprom_8(INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX, x);
#if defined(WS2812_MATRIX_DISPLAY)
      if (_next)
      {
        ssc_display_mode = DISPLAY_MODE_SET_COLOR_OF_NUMBER;
      }
      else
#endif
      {
        ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
      }
      sscStopSetting(sscTasks.other_setting_mode());
      break;
#endif
#if defined(WS2812_MATRIX_DISPLAY)
    case DISPLAY_MODE_SET_COLOR_OF_NUMBER:
      write_eeprom_crgb(COLOR_OF_NUMBER_VALUE_EEPROM_INDEX,
                        pgm_read_dword(&color_of_number[x]));
      clkDisp.setColorOfNumber(pgm_read_dword(&color_of_number[x]));
      ssc_display_mode = DISPLAY_MODE_SHOW_TIME;
      sscStopSetting(sscTasks.other_setting_mode());
      break;
#endif
    default:
      break;
    }
  }
}

void _checkBtnUpDownForOthSet(uint8_t &x)
{
  if ((buttons.getButtonFlag(CLK_BTN_UP) == CLK_BTN_FLAG_NEXT) ||
      (buttons.getButtonFlag(CLK_BTN_DOWN, true) == CLK_BTN_FLAG_NEXT))
  {
    bool dir = (buttons.getButtonFlag(CLK_BTN_UP, true) == CLK_BTN_FLAG_NEXT);
    switch (ssc_display_mode)
    {
#if defined(USE_LIGHT_SENSOR)
    case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
      sscCheckData(x, 9, dir, 1, false);
      break;
#endif
#if defined(USE_SET_BRIGHTNESS_MODE)
#if defined(USE_LIGHT_SENSOR)
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
#if USE_AUTO_SHOW_DATA
    case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
      sscCheckData(x, 7, dir, 0, false);
      break;
#endif
#if defined(WS2812_MATRIX_DISPLAY)
    case DISPLAY_MODE_SET_COLOR_OF_NUMBER:
      sscCheckData(x, 7, dir, 0, true);
      clkDisp.setColorOfNumber(pgm_read_dword(&color_of_number[x]));
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
                 !buttons.isButtonClosed(CLK_BTN_UP) &&
                 !buttons.isButtonClosed(CLK_BTN_DOWN);
    switch (ssc_display_mode)
    {
#if defined(USE_LIGHT_SENSOR)
    case DISPLAY_MODE_SET_LIGHT_THRESHOLD:
#if USE_MATRIX_DISPLAY
      sscSetOtherDataString(SET_LIGHT_THRESHOLD_TAG, 1, x, blink);
#else
      sscSetOtherData(SET_LIGHT_THRESHOLD_TAG, x, blink);
#endif
      break;
#endif
#if defined(USE_SET_BRIGHTNESS_MODE)
#if defined(USE_LIGHT_SENSOR)
    case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
    case DISPLAY_MODE_SET_BRIGHTNESS_MAX:
      clkDisp.setBrightness(x);
#if USE_MATRIX_DISPLAY
      sscSetOtherDataString(SET_BRIGHTNESS_TAG, 1, x, blink);
#else
      sscSetOtherData(SET_BRIGHTNESS_TAG, x, blink);
#endif
      break;
#endif
#if USE_AUTO_SHOW_DATA
    case DISPLAY_MODE_SET_AUTO_SHOW_PERIOD:
#if USE_MATRIX_DISPLAY
      sscSetOtherDataString(AUTO_SHOW_PERIOD_TAG,
                            1,
                            sscGetPeriodForAutoShow(x),
                            blink);
#else
      sscSetOtherData(AUTO_SHOW_PERIOD_TAG,
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
  if (!sscTasks.getTaskState(sscTasks.other_setting_mode()))
  {
    _startOtherSettingMode(x);
#if !USE_MATRIX_DISPLAY
    clkDisp.sleep(); // слегка мигнуть экраном при входе в настройки
    return;
#endif
  }

#if defined(USE_TICKER_FOR_DATA)
  // подождать, пока отработает бегущая строка
  if (sscTasks.getTaskState(sscTasks.ticker()))
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

#if USE_AUTO_SHOW_DATA

void _startAutoShowMode(uint8_t &n, uint8_t &n_max)
{
  switch (ssc_display_mode)
  {
#if defined(USE_CALENDAR)
  case DISPLAY_MODE_SHOW_DATE:
#if USE_MATRIX_DISPLAY
    n = 0;
#else
    n = 1;
#endif
    n_max = 3;
    break;
#endif
#if defined(USE_TEMP_DATA)
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
#if defined(USE_TEMP_DATA)
    n_max = 4;
#endif
    break;
  default:
    break;
  }
  sscTasks.startTask(sscTasks.auto_show_mode());
}

#if USE_MATRIX_DISPLAY

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
      sscSetDayOfWeakString(7, getDayOfWeek(sscClock.getCurTime().day(),
                                            sscClock.getCurTime().month(),
                                            sscClock.getCurTime().year()));
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
                       sscClock.getCurTime().day(),
                       sscClock.getCurTime().month(),
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
      sscSetYearString(1, sscClock.getCurTime().year());
    }
    break;
#endif
#if defined(USE_TEMP_DATA)
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

#if defined(USE_TICKER_FOR_DATA)
  if (!read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX))
  {
    clkDisp.show();
  }
#else
  clkDisp.show();
#endif
}

#else

void _setDisplayForAutoShowData(uint8_t &n)
{
  DateTime dt = sscClock.getCurTime();

  switch (n)
  {
#if defined(USE_CALENDAR)
  case 0:
    clkDisp.setDispData(0, clkDisp.encodeDigit(0x0d));
    clkDisp.setDispData(1, clkDisp.encodeDigit(0x0a));
#if defined(TM1637_DISPLAY)
    clkDisp.setDispData(2, 0b01111000);
#elif defined(MAX72XX_7SEGMENT_DISPLAY)
    clkDisp.setDispData(2, 0b00001111);
#endif
    clkDisp.setDispData(3, clkDisp.encodeDigit(0x0e));
    clkDisp.show();
    break;
  case 1:
  case 2:
    clkDisp.showDate(dt);
    break;
#endif
#if defined(USE_TEMP_DATA)
  case 3:
    clkDisp.showTemp(sscGetCurTemp());
    break;
#endif
  default:
    break;
  }
  clkDisp.show();
}

#endif

void sscAutoShowData()
{
  static uint8_t n = 0;
  static uint8_t n_max = 0;
  static uint32_t timer = 0;

  if (!sscTasks.getTaskState(sscTasks.auto_show_mode()))
  {
    _startAutoShowMode(n, n_max);
  }

#if defined(USE_TICKER_FOR_DATA)
  if (sscTasks.getTaskState(sscTasks.ticker()))
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
      clkDisp.clear();
    }
#else
    clkDisp.clear();
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

void sscShowOnOffData(clkDataType _type, bool _state, bool blink)
{
  clkDisp.clear();
#if USE_MATRIX_DISPLAY
  sscSetOnOffDataString(_type, 1, _state, blink);
#else
  sscSetOnOffData(_type, _state, blink);
#endif
}

#if USE_MATRIX_DISPLAY

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
      sscSetNumString(offset, hour, 5, 1, toStringData);
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
        sscSetChar(offset + 14 + j * 6,
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
    uint8_t colon = /*(toDate) ? 0x01 : */ 0x24;
    if (toStringData)
    {
      sData.setData(offset + 14, colon);
    }
    else
    {
      clkDisp.setColumn(offset + 14, colon);
    }
  }
}

void sscSetOtherDataString(clkDataType _type,
                           uint8_t offset,
                           uint8_t _data,
                           bool blink,
                           bool toStringData)
{
  clkDisp.clear();

  switch (_type)
  {
#if defined(USE_SET_BRIGHTNESS_MODE)
  case SET_BRIGHTNESS_TAG:
    sscSetTag(offset, DISP_BRIGHTNESS_TAG, 5, toStringData);
#if defined(USE_LIGHT_SENSOR)
    uint8_t x;
    x = (ssc_display_mode == DISPLAY_MODE_SET_BRIGHTNESS_MIN) ? 0x30 : 0x31;
    sscSetChar(offset + 12, x, 5, toStringData);
#endif
    break;
#endif
#if defined(USE_LIGHT_SENSOR)
  case SET_LIGHT_THRESHOLD_TAG:
    sscSetTag(offset, DISP_LIGHT_THRESHOLD_TAG, 5, toStringData);
    break;
#endif
#if USE_AUTO_SHOW_DATA
  case AUTO_SHOW_PERIOD_TAG:
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
    clkDisp.setColumn(offset + 18, 0x24); // ":"
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
  case TICKER_TAG:
    sscSetTag(offset, DISP_ANIMATION_TAG, 5, toStringData);
    break;
#endif
#if defined(USE_ALARM)
  case ALARM_TAG:
    sscSetTag(offset, DISP_ALARM_TAG, 5, toStringData);
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
    clkDisp.setColumn(offset + 20, 0x24); // ":"
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
                     bool toStringData)
{
  uint8_t x = (width == 6) ? num / 10 : num / 10 + 0x30;
  sscSetChar(offset, x, width, toStringData);
  x = (width == 6) ? num % 10 : num % 10 + 0x30;
  sscSetChar(offset + width + space, x, width, toStringData);
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
        clkDisp.setColumn(j, chr_data);
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
    sData.setData(i, clkDisp.getColumn(i));
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

#if defined(USE_TEMP_DATA)
  case DISPLAY_MODE_SHOW_TEMP: // температура
    sscSetTempString(lenght - 31, sscGetCurTemp(), true);
    break;
#endif

#if defined(USE_SET_BRIGHTNESS_MODE)
  case DISPLAY_MODE_SET_BRIGHTNESS_MAX: // настройка яркости
#if defined(USE_LIGHT_SENSOR)
  case DISPLAY_MODE_SET_BRIGHTNESS_MIN:
#endif
    uint8_t x;
    if (ssc_display_mode == DISPLAY_MODE_SET_BRIGHTNESS_MAX)
    {
      x = read_eeprom_8(MAX_BRIGHTNESS_VALUE_EEPROM_INDEX);
    }
#if defined(USE_LIGHT_SENSOR)
    else
    {
      x = read_eeprom_8(MIN_BRIGHTNESS_VALUE_EEPROM_INDEX);
    }
#endif
    sscSetOtherDataString(SET_BRIGHTNESS_TAG, lenght - 31, x, false, true);
    break;
#endif

#if defined(USE_LIGHT_SENSOR)
  case DISPLAY_MODE_SET_LIGHT_THRESHOLD: // настройка порога переключения яркости
    uint8_t y;
    y = read_eeprom_8(LIGHT_THRESHOLD_EEPROM_INDEX);
    sscSetOtherDataString(SET_LIGHT_THRESHOLD_TAG, lenght - 31, y, false, true);
    break;
#endif

#if defined(USE_ALARM)
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

#if defined(USE_CALENDAR)
  case DISPLAY_MODE_SHOW_DOW: // день недели
    sscSetDayOfWeakString(lenght - 25,
                          getDayOfWeek(sscClock.getCurTime().day(),
                                       sscClock.getCurTime().month(),
                                       sscClock.getCurTime().year()),
                          true);
    break;

  case DISPLAY_MODE_SHOW_DAY_AND_MONTH: // число и месяц
  case DISPLAY_MODE_SET_DAY:            // настройка числа
    sscSetTimeString(lenght - 31,
                     sscClock.getCurTime().day(),
                     sscClock.getCurTime().month(),
                     true,
                     true,
                     true);
    break;

  case DISPLAY_MODE_SHOW_YEAR: // год
  case DISPLAY_MODE_SET_YEAR:  // настройка года
    sscSetYearString(lenght - 31, sscClock.getCurTime().year(), true);
    break;
#endif
#if defined(USE_TICKER_FOR_DATA)
  case DISPLAY_MODE_SET_TICKER_ON_OFF: // настройка включения/выключения анимации
    sscSetOnOffDataString(TICKER_TAG,
                          lenght - 31,
                          read_eeprom_8(TICKER_STATE_VALUE_EEPROM_INDEX),
                          false,
                          true);
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

  if (!sscTasks.getTaskState(sscTasks.ticker()))
  {
    sscTasks.startTask(sscTasks.ticker());
    n = 0;
  }

  for (uint8_t i = 0; i < 32; i++)
  {
    clkDisp.setColumn(i, sData.getData(i + n));
  }
  clkDisp.show();

  if (n++ >= sData.getDataLenght() - 32)
  {
    sscTasks.stopTask(sscTasks.ticker());
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

void sscSetYearString(uint8_t offset, uint16_t _year, bool toStringData)
{
  sscSetNumString(offset, 20, 6, 2, toStringData);
  sscSetNumString(offset + 16, _year % 2000, 6, 2, toStringData);
}
#endif

#if defined(USE_TEMP_DATA)
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
  col = clkDisp.getColorOfNumber();
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

  clkDisp.setDispData(2, ((_data / 10 == 0 || blink) ? 0x00
                                                     : clkDisp.encodeDigit(_data / 10)));
  clkDisp.setDispData(3, ((blink) ? 0x00
                                  : clkDisp.encodeDigit(_data % 10)));

  clkDisp.show();
}

void sscSetTag(clkDataType _type)
{
  switch (_type)
  {
#if USE_AUTO_SHOW_DATA
  case AUTO_SHOW_PERIOD_TAG: // Au
    clkDisp.setDispData(0, clkDisp.encodeDigit(0x0A));
#if defined(TM1637_DISPLAY)
    clkDisp.setDispData(1, 0b10011100);
#elif defined(MAX72XX_7SEGMENT_DISPLAY)
    clkDisp.setDispData(1, 0b10011100);
#endif
    break;
#endif
#if defined(USE_ALARM)
  case ALARM_TAG: // AL
    clkDisp.setDispData(0, clkDisp.encodeDigit(0x0A));
#if defined(TM1637_DISPLAY)
    clkDisp.setDispData(1, 0b10111000);
#elif defined(MAX72XX_7SEGMENT_DISPLAY)
    clkDisp.setDispData(1, 0b10001110);
#endif
    break;
#endif
#if defined(USE_LIGHT_SENSOR)
  case SET_LIGHT_THRESHOLD_TAG: // th
#if defined(TM1637_DISPLAY)
    clkDisp.setDispData(0, 0b01111000);
    clkDisp.setDispData(1, 0b11110100);
#elif defined(MAX72XX_7SEGMENT_DISPLAY)
    clkDisp.setDispData(0, 0b00001111);
    clkDisp.setDispData(1, 0b10010111);
#endif
    break;
#endif
#if defined(USE_SET_BRIGHTNESS_MODE)
  case SET_BRIGHTNESS_TAG: // br, b0, b1
    clkDisp.setDispData(0, clkDisp.encodeDigit(0x0B));
#if defined(TM1637_DISPLAY)
    clkDisp.setDispData(1, 0b11010000);
#elif defined(MAX72XX_7SEGMENT_DISPLAY)
    clkDisp.setDispData(1, 0b10000101);
#endif
#if defined(USE_LIGHT_SENSOR)
    uint8_t x;
    x = (ssc_display_mode == DISPLAY_MODE_SET_BRIGHTNESS_MIN) ? clkDisp.encodeDigit(0)
                                                                      : clkDisp.encodeDigit(1);
    x |= 0x80;
    clkDisp.setDispData(1, x);
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

  clkDisp.setDispData(2, 0x00);
  uint8_t x = 0x00;
  if (!_blink)
  {
#if defined(TM1637_DISPLAY)
    x = (_state) ? 0b01011100 : 0b00001000;
#elif defined(MAX72XX_7SEGMENT_DISPLAY)
    x = (_state) ? 0b00011101 : 0b00001000;
#endif
  }
  clkDisp.setDispData(3, x);

  clkDisp.show();
}

#endif
