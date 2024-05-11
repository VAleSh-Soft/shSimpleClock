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
#if defined(ARDUINO_ARCH_ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif
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

  uint16_t const LEDS_COUNT = 256;

class DisplayWS2812Matrix
{
private:
  CRGB leds[LEDS_COUNT];
  MatrixType matrix_type = BY_COLUMNS;
  uint8_t row_count = 8;
  uint8_t col_count = 32;
  CRGB color = CRGB::Red;
  CRGB bg_color = CRGB::Black;

  uint8_t getLedIndexOfStrip(uint8_t row, uint8_t col);

  // void setNumString(uint8_t offset, uint8_t num,
  //                   uint8_t width = 6, uint8_t space = 1,
  //                   uint8_t *_data = NULL, uint8_t _data_count = 0);

  void setChar(uint8_t offset, uint8_t chr,
               uint8_t width = 6, uint8_t *_arr = NULL, uint8_t _arr_length = 0);

#if __ESPICHIPSET__
  void setESpiLedsData(CRGB *data, uint16_t leds_count);
#else

  void setLedsData(CRGB *data, uint16_t leds_count);
#endif

public:
  /**
   * @brief конструктор
   *
   * @param _leds массив светодиодов
   * @param _color цвет
   * @param _type тип матрицы, собрана по столбцам или построчно
   */
  DisplayWS2812Matrix(CRGB _color, MatrixType _type);

  /**
   * @brief запись столбца в буфер экрана
   *
   * @param col столбец
   * @param _data байт для записи
   */
  void setColumn(uint8_t col, uint8_t _data);

  /**
   * @brief получение битовой маски столбца из буфера устройства
   *
   * @param column столбец (координата X)
   * @return результат
   */
  uint8_t getColumn(uint8_t col);

  /**
   * @brief очистка буфера экрана
   *
   * @param upd при false очищается только буфер экрана, при true - очищается и сам экран
   */
  void clear(bool upd = false);

  /**
   * @brief запись символа в буфера экрана
   *
   * @param offset индекс столбца, с которого начинается отрисовка символа (0..31)
   * @param chr символ для записи
   * @param width ширина символа, может иметь значение 5 или 6, определяет, какой набор символов будет использован: 5х7 (для текста) или 6х8 (для вывода цифр)
   */
  void setDispData(uint8_t offset, uint8_t chr, uint8_t width = 6);

  /**
   * @brief вывести двоеточие в середине экрана
   *
   * @param toDot вместо двоеточия вывести точку
   */
  void setColon(bool toDot = false);

  /**
   * @brief отрисовка на экране содержимого его буфера
   *
   */
  void show();

  /**
   * @brief установка яркости экрана
   *
   * @param brightness значение яркости (0..25)
   */
  void setBrightness(uint8_t brightness);

  /**
   * @brief установка цвета символов
   *
   * @param _color новый цвет
   */
  void setColorOfNumber(CRGB _color);

  /**
   * @brief получение текущего цвета символов
   *
   * @return CRGB
   */
  CRGB getColorOfNumber();

  /**
   * @brief установка цвета фона
   *
   * @param _color новый цвет
   */
  void setColorOfBackground(CRGB _color);

  /**
   * @brief получение текущего цвета фона
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

  /**
   * @brief инициализация матрицы
   *
   * @param data
   * @param leds_count
   */
  void init();
};

// ---- DisplayWS2812Matrix private -------------

uint8_t DisplayWS2812Matrix::getLedIndexOfStrip(uint8_t row, uint8_t col)
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

// void DisplayWS2812Matrix::setNumString(uint8_t offset, uint8_t num,
//                                        uint8_t width, uint8_t space,
//                                        uint8_t *_data, uint8_t _data_count)
// {
//   uint8_t x = (width == 6) ? num / 10 : num / 10 + 0x30;
//   setChar(offset, x, width);
//   x = (width == 6) ? num % 10 : num % 10 + 0x30;
//   setChar(offset + width + space, x, width);
// }

void DisplayWS2812Matrix::setChar(uint8_t offset, uint8_t chr,
                                  uint8_t width, uint8_t *_arr, uint8_t _arr_length)
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

#if __ESPICHIPSET__
void DisplayWS2812Matrix::setESpiLedsData(CRGB *data, uint16_t leds_count)
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

void DisplayWS2812Matrix::setLedsData(CRGB *data, uint16_t leds_count)
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

// ---- DisplayWS2812Matrix public --------------

DisplayWS2812Matrix::DisplayWS2812Matrix(CRGB _color, MatrixType _type)
{
  color = _color;
  bg_color = COLOR_OF_BACKGROUND;
  matrix_type = _type;
  clear(true);
  setMaxPSP(POWER_SUPPLY_VOLTAGE, POWER_SUPPLY_CURRENT);
}

void DisplayWS2812Matrix::setColumn(uint8_t col, uint8_t _data)
{
  if (col < 32)
  {
    for (uint8_t i = 0; i < 8; i++)
    {
      uint8_t grd = 0;
      grd = (grd > 7) ? 0 : grd;

      if (grd > 0)
      {
        // Варианты градиента для вывода символов; grd - выбор градиента
        uint8_t j;

        switch (grd)
        {
        case 1: // градиент по диагонали 1
          j = (i + col) % 7 + 1;
          break;
        case 2: // градиент по диагонали 2
          j = (i + (7 - col % 7)) % 7 + 1;
          break;
        case 3: // градиент по вертикали - красный -- красный
        case 4: // градиент по вертикали - желтый -- желтый
        case 5: // градиент по вертикали - голубой -- голубой
        case 6: // градиент по вертикали - фиолетовый -- фиолетовый
          j = (i + (grd - 3) * 2) % 7 + 1;
          break;
        case 7: // градиент по горизонтали
          j = col % 7 + 1;
          break;
        }

        leds[getLedIndexOfStrip(i, col)] =
            (((_data) >> (7 - i)) & 0x01) ? pgm_read_dword(&color_of_number[j])
                                          : bg_color;
      }
      else
      {
        leds[getLedIndexOfStrip(i, col)] =
            (((_data) >> (7 - i)) & 0x01) ? color : bg_color;
      }
    }
  }
}

uint8_t DisplayWS2812Matrix::getColumn(uint8_t col)
{
  uint8_t result = 0x00;
  if (col < 32)
  {
    for (uint8_t i = 0; i < 8; i++)
    {
      CRGB x = leds[getLedIndexOfStrip(i, col)];
      (x.r != bg_color.r ||
       x.g != bg_color.g ||
       x.b != bg_color.b)
          ? (result) |= (1UL << (7 - i))
          : (result) &= ~(1UL << (7 - i));
    }
  }
  return (result);
}

void DisplayWS2812Matrix::clear(bool upd)
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

void DisplayWS2812Matrix::setDispData(uint8_t offset, uint8_t chr, uint8_t width)
{
  if (offset < 32)
  {
    setChar(offset, chr, width);
  }
}

void DisplayWS2812Matrix::setColon(bool toDot)
{
  (toDot) ? setColumn(15, 0b00000001) : setColumn(15, 0b00100100);
}

void DisplayWS2812Matrix::show()
{
  FastLED.show();
}

void DisplayWS2812Matrix::setBrightness(uint8_t brightness)
{
  brightness = (brightness <= 25) ? brightness : 25;
  FastLED.setBrightness(brightness * 10);
}

void DisplayWS2812Matrix::setColorOfNumber(CRGB _color)
{
  color = _color;
}

CRGB DisplayWS2812Matrix::getColorOfNumber()
{
  return (color);
}

void DisplayWS2812Matrix::setColorOfBackground(CRGB _color)
{
  bg_color = _color;
}

CRGB DisplayWS2812Matrix::getColorOfBackground()
{
  return (bg_color);
}

void DisplayWS2812Matrix::setMaxPSP(uint8_t volts, uint32_t milliamps)
{
  FastLED.setMaxPowerInVoltsAndMilliamps(volts, milliamps);
}

void DisplayWS2812Matrix::init()
{
#if __ESPICHIPSET__
  setESpiLedsData(leds, LEDS_COUNT);
#else
  setLedsData(leds, LEDS_COUNT);
#endif
}

// ===================================================

DisplayWS2812Matrix clkDisplay(COLOR_OF_NUMBER, MX_TYPE);
