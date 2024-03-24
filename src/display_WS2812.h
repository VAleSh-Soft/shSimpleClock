/**
 * @file display_WS2812.h
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * @brief Модуль, реализующий работу часов с матрицами, построенными на адресных светодиодах
 * @version 1.0
 * @date 11.03.2024
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "matrix_data.h"
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <FastLED.h> // https://github.com/FastLED/FastLED
#include "shSimpleRTC.h"

// ===================================================

#define __ESPICHIPSET__ defined CHIPSET_LPD6803 || defined CHIPSET_LPD8806 || defined CHIPSET_WS2801 || defined CHIPSET_WS2803 || defined CHIPSET_SM16716 || defined CHIPSET_P9813 || defined CHIPSET_APA102 || defined CHIPSET_SK9822 || defined CHIPSET_DOTSTAR

// ===================================================

static const uint32_t PROGMEM color_of_number[] = {
    0xFFFFFF, // белый (White)
    0xFF0000, // красный (Red)
    0xFF4500, // оранжевый (Orange)
    0xFFCC00, // желтый (Yellow)
    0x00FF00, // зеленый (Green)
    0x00FFFF, // голубой (Blue)
    0x0000FF, // синий (Indigo)
    0x9600D7  // фиолетовый (Violet)
};

// ==== класс для матрицы 8х32 адресных светодиодов ==

/**
 * @brief тип матрицы по расположению светодиодов
 */
enum MatrixType : uint8_t
{
  /**
   * @brief светодиоды расположены по столбцам:
   *    _   _
   * | | | | |
   * |_| |_| |
   *
   */
  BY_COLUMNS,
  /**
   * @brief светодиоды расположены построчно:
   *  ________
   *  ________|
   * |________
   */
  BY_LINE
};

class DisplayWS2812Matrix
{
private:
  CRGB *leds = NULL;
  MatrixType matrix_type = BY_COLUMNS;
  uint8_t row_count = 8;
  uint8_t col_count = 32;
  CRGB color = CRGB::Red;
  CRGB bg_color = CRGB::Black;

  uint8_t getLedIndexOfStrip(uint8_t row, uint8_t col)
  {
    uint8_t result = 0;
    switch (matrix_type)
    {
    case BY_COLUMNS:
      result = col * row_count + (((col >> 0) & 0x01) ? row_count - row - 1 : row);
      break;
    case BY_LINE:
      result = row * col_count + (((row >> 0) & 0x01) ? col_count - col - 1 : col);
      break;
    }
    return (result);
  }

  void setNumString(uint8_t offset, uint8_t num,
                    uint8_t width = 6, uint8_t space = 1,
                    uint8_t *_data = NULL, uint8_t _data_count = 0)
  {
    uint8_t x = (width == 6) ? num / 10 : num / 10 + 0x30;
    setChar(offset, x, width);
    x = (width == 6) ? num % 10 : num % 10 + 0x30;
    setChar(offset + width + space, x, width);
  }

  void setDayOfWeakString(uint8_t offset, DateTime date, uint8_t *_data = NULL, uint8_t _data_count = 0)
  {
    uint8_t dow = getDayOfWeek(date.day(), date.month(), date.year());
    for (uint8_t j = 0; j < 3; j++)
    {
      setChar(offset + j * 7,
              pgm_read_byte(&day_of_week[dow * 3 + j]), 5, _data, _data_count);
    }
  }

  void setTempString(uint8_t offset, int16_t temp, uint8_t *_data = NULL, uint8_t _data_count = 0)
  {
    // если температура выходит за диапазон, сформировать строку минусов
    if (temp > 99 || temp < -99)
    {
      for (uint8_t i = 0; i < 4; i++)
      {
        setChar(offset + 2 + i * 7, 0x2D, 5, _data, _data_count);
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
      setChar(offset + 13, temp % 10, 6, _data, _data_count);
      if (temp > 9)
      {
        // если температура двухзначная, переместить знак на позицию левее
        plus_pos = offset;
        setChar(offset + 6, temp / 10, 6, _data, _data_count);
      }
      // сформировать впереди плюс или минус
      if (temp != 0)
      {
        (plus) ? setChar(plus_pos, 0x2B, 5, _data, _data_count)
               : setChar(plus_pos, 0x2D, 5, _data, _data_count);
      }
      // сформировать в конце знак градуса Цельсия
      setChar(offset + 20, 0xB0, 5, _data, _data_count);
      setChar(offset + 25, 0x43, 5, _data, _data_count);
    }
  }

  void setChar(uint8_t offset, uint8_t chr,
               uint8_t width = 6, uint8_t *_arr = NULL, uint8_t _arr_length = 0)
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

      if (_arr != NULL)
      {
        if (j < _arr_length)
        {
          _arr[j] = chr_data;
        }
      }
      else
      {
        if (j < 32)
        {
          setColumn(j, chr_data);
        }
      }
    }
  }

public:
  /**
   * @brief конструктор
   *
   * @param _leds массив светодиодов
   * @param _color цвет
   * @param _type тип матрицы, собрана по столбцам или построчно
   */
  DisplayWS2812Matrix(CRGB *_leds, CRGB _color, MatrixType _type)
  {
    leds = _leds;
    color = _color;
    bg_color = COLOR_OF_BACKGROUND;
    matrix_type = _type;
    clear(true);
    setMaxPSP(POWER_SUPPLY_VOLTAGE, POWER_SUPPLY_CURRENT);
  }

  /**
   * @brief запись столбца в буфер экрана
   *
   * @param col столбец
   * @param _data байт для записи
   */
  void setColumn(uint8_t col, uint8_t _data)
  {
    if (col < 32)
    {
      for (uint8_t i = 0; i < 8; i++)
      {
        leds[getLedIndexOfStrip(i, col)] =
            (((_data) >> (7 - i)) & 0x01) ? color : bg_color;
      }
    }
  }

  /**
   * @brief получение битовой маски столбца из буфера устройства
   *
   * @param column столбец (координата X)
   * @return результат
   */
  uint8_t getColumn(uint8_t col)
  {
    uint8_t result = 0x00;
    if (col < 32)
    {
      for (uint8_t i = 0; i < 8; i++)
      {
        CRGB x = leds[getLedIndexOfStrip(i, col)];
        CRGB y = COLOR_OF_BACKGROUND;
        ((x.r != y.r || x.g != y.g || x.b != y.b)) ? (result) |= (1UL << (7 - i))
                                                   : (result) &= ~(1UL << (7 - i));
      }
    }
    return (result);
  }

  /**
   * @brief очистка буфера экрана
   *
   * @param upd при false очищается только буфер экрана, при true - очищается и сам экран
   */
  void clear(bool upd = false)
  {
    for (uint8_t i = 0; i < 32; i++)
    {
      setColumn(i, 0x00);
    }
    if (upd)
    {
      FastLED.show();
    }
  }

  /**
   * @brief запись символа в буфера экрана
   *
   * @param offset индекс столбца, с которого начинается отрисовка символа (0..31)
   * @param chr символ для записи
   * @param width ширина символа, может иметь значение 5 или 6, определяет, какой набор символов будет использован: 5х7 (для текста) или 6х8 (для вывода цифр)
   */
  void setDispData(uint8_t offset, uint8_t chr, uint8_t width = 6)
  {
    if (offset < 32)
    {
      setChar(offset, chr, width);
    }
  }

  /**
   * @brief вывести двоеточие в середине экрана
   *
   * @param toDot вместо двоеточия вывести точку
   */
  void setColon(bool toDot = false)
  {
    (toDot) ? setColumn(15, 0b00000001) : setColumn(15, 0b00100100);
  }

  /**
   * @brief отрисовка на экране содержимого его буфера
   *
   */
  void show()
  {
    FastLED.show();
  }

  /**
   * @brief установка яркости экрана
   *
   * @param brightness значение яркости (0..25)
   */
  void setBrightness(uint8_t brightness)
  {
    brightness = (brightness <= 25) ? brightness : 25;
    FastLED.setBrightness(brightness * 10);
  }

  /**
   * @brief вывод на экран  времени; если задать какое-то из значений hour или minute отрицательным, эта часть экрана будет очищена - можно организовать мигание, например, в процессе настройки времени
   *
   * @param hour часы
   * @param minute минуты
   * @param second секунды
   * @param show_colon отображать или нет двоеточие между часами и минутами
   * @param date флаг, показывающий, что выводится дата, а не время
   */
  void showTime(int8_t hour, int8_t minute, uint8_t second, bool show_colon, bool date = false)
  {
    clear();

    if (hour >= 0)
    {
      if (date)
      {
        setNumString(1, hour, 5, 1);
      }
      else
      {
        setNumString(1, hour, 6, 1);
      }
    }
    if (minute >= 0)
    {
      if (date)
      {
        for (uint8_t j = 0; j < 3; j++)
        {
          setChar(15 + j * 6,
                  pgm_read_byte(&months[(minute - 1) * 3 + j]), 5);
        }
      }
      else
      {
        setNumString(17, minute, 6, 1);
      }
    }
    if (show_colon && !date)
    {
      setColon(false);
    }

#if defined(SHOW_SECOND_COLUMN)
    // формирование секундного столбца
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
    setColumn(31, col_sec);
#endif
  }

  /**
   * @brief вывод года на экран; если _year < 0, то вторая часть года стирается, что позволяет организовать мигание значения при его настройке
   *
   * @param _year значение для вывода (вторая часть)
   */
  void showYear(int8_t _year)
  {
    clear();

    setNumString(1, 20, 6, 1);

    if (_year >= 0)
    {
      setNumString(17, _year, 6, 1);
    }
  }

  /**
   * @brief вывод на экран температуры в диапазоне от -99 до +99 градусов; вне диапазона выводится строка минусов
   *
   * @param temp данные для вывода
   */
  void showTemp(int temp)
  {
    clear();
    setTempString(1, temp);
  }

  /**
   * @brief установка цвета символов
   *
   * @param _color новый цвет
   */
  void setColorOfNumber(CRGB _color)
  {
    color = _color;
  }

  /**
   * @brief получение текущего цвета символов
   *
   * @return CRGB
   */
  CRGB getColorOfNumber()
  {
    return (color);
  }

  /**
   * @brief установка цвета фона
   *
   * @param _color новый цвет
   */
  void setColorOfBackground(CRGB _color)
  {
    bg_color = _color;
  }

  /**
   * @brief получение текущего цвета фона
   *
   * @return CRGB
   */
  CRGB getColorOfBackground()
  {
    return(bg_color);
  }

  /**
   * @brief установка максимальной мощности блока питания матрицы
   *
   * @param volts напряжение, Вольт
   * @param milliamps максимальный ток, милиампер
   */
  void setMaxPSP(uint8_t volts, uint32_t milliamps)
  {
    FastLED.setMaxPowerInVoltsAndMilliamps(volts, milliamps);
  }
};

// ==== инициализация матрицы ========================

#if __ESPICHIPSET__
void setESpiLedsData(CRGB *data, uint16_t leds_count)
{
#if defined CHIPSET_LPD6803
  ESPIChipsets const chip = LPD6803;
#elif defined CHIPSET_LPD8806
  ESPIChipsets const chip = LPD8806;
#elif defined CHIPSET_WS2801
  ESPIChipsets const chip = WS2801;
#elif defined CHIPSET_WS2803
  ESPIChipsets const chip = WS2803;
#elif defined CHIPSET_SM16716
  ESPIChipsets const chip = SM16716;
#elif defined CHIPSET_P9813
  ESPIChipsets const chip = P9813;
#elif defined CHIPSET_APA102
  ESPIChipsets const chip = APA102;
#elif defined CHIPSET_SK9822
  ESPIChipsets const chip = SK9822;
#elif defined CHIPSET_DOTSTAR
  ESPIChipsets const chip = DOTSTAR;
#endif

#if defined(USE_HARDWARE_SPI)
  FastLED.addLeds<chip, EORDER>(data, leds_count);
#else
  FastLED.addLeds<chip, DISPLAY_DIN_PIN, DISPLAY_CLK_PIN, EORDER>(data, leds_count);
#endif
}
#else

void setLedsData(CRGB *data, uint16_t leds_count)
{
#if defined CHIPSET_NEOPIXEL
  FastLED.addLeds<NEOPIXEL, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_SM16703
  FastLED.addLeds<SM16703, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_TM1829
  FastLED.addLeds<TM1829, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_TM1812
  FastLED.addLeds<TM1812, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_TM1809
  FastLED.addLeds<TM1809, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_TM1804
  FastLED.addLeds<TM1804, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_TM1803
  FastLED.addLeds<M1803, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_UCS1903
  FastLED.addLeds<UCS1903, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_UCS1903B
  FastLED.addLeds<UCS1903B, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_UCS1904
  FastLED.addLeds<UCS1904, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_UCS2903
  FastLED.addLeds<UCS2903, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_WS2812
  FastLED.addLeds<WS2812, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_WS2852
  FastLED.addLeds<WS2852, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_WS2812B
  FastLED.addLeds<WS2812B, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_GS1903
  FastLED.addLeds<GS1903, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_SK6812
  FastLED.addLeds<SK6812, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_SK6822
  FastLED.addLeds<SK6822, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_APA106
  FastLED.addLeds<APA106, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_PL9823
  FastLED.addLeds<PL9823, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_WS2811
  FastLED.addLeds<WS2811, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_WS2813
  FastLED.addLeds<WS2813, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_APA104
  FastLED.addLeds<APA104, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_WS2811_400
  FastLED.addLeds<WS2811_400, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_GE8822
  FastLED.addLeds<GE8822, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_GW6205
  FastLED.addLeds<GW6205, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_GW6205_400
  FastLED.addLeds<GW6205_400, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_LPD1886
  FastLED.addLeds<LPD1886, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#elif defined CHIPSET_LPD1886_8BIT
  FastLED.addLeds<LPD1886_8BIT, DISPLAY_DIN_PIN, EORDER>(data, leds_count);
#endif
}
#endif

void setFastLEDData(CRGB *data, uint16_t leds_count)
{
#if __ESPICHIPSET__
  setESpiLedsData(data, leds_count);
#else
  setLedsData(data, leds_count);
#endif
}