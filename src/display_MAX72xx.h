/**
 * @file display_MAX72xx.h
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * @brief Модуль, реализующий работу часов с экранами, основанными на драйверах MAX7219/MAX7221
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
#include "shSimpleRTC.h"
#include <shMAX72xxMini.h> // https://github.com/VAleSh-Soft/shMAX72xxMini

// ==== класс для 7-сегментного индикатора MAX72xx ===

#define NUM_DIGITS 8

template <uint8_t cs_pin>
class DisplayMAX72xx7segment : public shMAX72xx7Segment<cs_pin, 1, NUM_DIGITS>
{
private:
  uint8_t data[4];
  // контрольные данные
  uint8_t _data[4] = {0x00, 0x00, 0x00, 0x00};

  void setSegments(uint8_t *data);

public:
  DisplayMAX72xx7segment() : shMAX72xx7Segment<cs_pin, 1, NUM_DIGITS>() { clear(); }

  /**
   * @brief очистка буфера экрана, сам экран при этом не очищается
   *
   */
  void clear();

  /**
   * @brief очистка экрана
   *
   */
  void sleep();

  /**
   * @brief установка разряда _index буфера экрана
   *
   * @param _index индекс разряда (0..3)
   * @param _data данные для установки
   */
  void setDispData(uint8_t _index, uint8_t _data);

  /**
   * @brief получение значения разряда _index буфера экрана
   *
   * @param _index индекс разряда (0..3)
   * @return uint8_t
   */
  uint8_t getDispData(uint8_t _index);

  /**
   * @brief отрисовка на экране содержимого его буфера
   *
   */
  void show();

  /**
   * @brief установка яркости экрана
   *
   * @param brightness значение яркости (1..7)
   */
  void setBrightness(uint8_t brightness);
};

// ---- DisplayMAX72xx7segment private ----------

template <uint8_t cs_pin>
void DisplayMAX72xx7segment<cs_pin>::setSegments(uint8_t *data)
{
  shMAX72xx7Segment<cs_pin, 1, NUM_DIGITS>::clearAllDevices();
  for (uint8_t i = 0; i < 4; i++)
  {
    shMAX72xx7Segment<cs_pin, 1, NUM_DIGITS>::setChar(7 - i, data[i]);
  }

  shMAX72xx7Segment<cs_pin, 1, NUM_DIGITS>::update();
}

// ---- DisplayMAX72xx7segment public -----------

template <uint8_t cs_pin>
void DisplayMAX72xx7segment<cs_pin>::clear()
{
  for (uint8_t i = 0; i < 4; i++)
  {
    data[i] = 0x00;
  }
}

template <uint8_t cs_pin>
void DisplayMAX72xx7segment<cs_pin>::sleep()
{
  clear();
  shMAX72xx7Segment<cs_pin, 1, NUM_DIGITS>::clearAllDevices(true);
}

template <uint8_t cs_pin>
void DisplayMAX72xx7segment<cs_pin>::setDispData(uint8_t _index, uint8_t _data)
{
  if (_index < 4)
  {
    data[_index] = _data;
  }
}

template <uint8_t cs_pin>
uint8_t DisplayMAX72xx7segment<cs_pin>::getDispData(uint8_t _index)
{
  return ((_index < 4) ? data[_index] : 0);
}

template <uint8_t cs_pin>
void DisplayMAX72xx7segment<cs_pin>::show()
{
  bool flag = false;
  for (uint8_t i = 0; i < 4; i++)
  {
    flag = _data[i] != data[i];
    if (flag)
    {
      break;
    }
  }
  // отрисовка экрана происходит только если изменился хотя бы один разряд
  if (flag)
  {
    for (uint8_t i = 0; i < 4; i++)
    {
      _data[i] = data[i];
    }
    setSegments(data);
  }
}

template <uint8_t cs_pin>
void DisplayMAX72xx7segment<cs_pin>::setBrightness(uint8_t brightness)
{
  brightness = (brightness <= 15) ? brightness : 15;
  shMAX72xxMini<cs_pin, 1>::setBrightness(0, brightness);
}

// ==== класс для матрицы 8х8х4 MAX72xx ==============

template <uint8_t cs_pin>
class DisplayMAX72xxMatrix : public shMAX72xxMini<cs_pin, 4>
{
private:
  // void setNumString(uint8_t offset, uint8_t num,
  //                   uint8_t width = 6, uint8_t space = 1,
  //                   uint8_t *_data = NULL, uint8_t _data_count = 0);

  void setChar(uint8_t offset, uint8_t chr,
               uint8_t width = 6, uint8_t *_arr = NULL, uint8_t _arr_length = 0);

public:
  DisplayMAX72xxMatrix() : shMAX72xxMini<cs_pin, 4>() { clear(); }

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
   * @brief очистка экрана
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
   * @param brightness значение яркости (0..15)
   */
  void setBrightness(uint8_t brightness);
};

// ---- DisplayMAX72xxMatrix private ------------

// template <uint8_t cs_pin>
// void DisplayMAX72xxMatrix<cs_pin>::setNumString(uint8_t offset, uint8_t num,
//                                         uint8_t width, uint8_t space,
//                                         uint8_t *_data, uint8_t _data_count)
// {
//   uint8_t x = (width == 6) ? num / 10 : num / 10 + 0x30;
//   setChar(offset, x, width, _data, _data_count);
//   x = (width == 6) ? num % 10 : num % 10 + 0x30;
//   setChar(offset + width + space, x, width, _data, _data_count);
// }

template <uint8_t cs_pin>
void DisplayMAX72xxMatrix<cs_pin>::setChar(uint8_t offset, uint8_t chr,
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
        shMAX72xxMini<cs_pin, 4>::setColumn(j / 8, j % 8, chr_data);
      }
    }
  }
}

// ---- DisplayMAX72xxMatrix public -------------

template <uint8_t cs_pin>
void DisplayMAX72xxMatrix<cs_pin>::setColumn(uint8_t col, uint8_t _data)
{
  if (col < 32)
  {
    shMAX72xxMini<cs_pin, 4>::setColumn(col / 8, col % 8, _data);
  }
}

template <uint8_t cs_pin>
uint8_t DisplayMAX72xxMatrix<cs_pin>::getColumn(uint8_t col)
{
  uint8_t result = 0x00;
  if (col < 32)
  {
    result = shMAX72xxMini<cs_pin, 4>::getColumn(col / 8, col % 8);
  }
  return (result);
}

template <uint8_t cs_pin>
void DisplayMAX72xxMatrix<cs_pin>::clear(bool upd)
{
  shMAX72xxMini<cs_pin, 4>::clearAllDevices(upd);
}

template <uint8_t cs_pin>
void DisplayMAX72xxMatrix<cs_pin>::setDispData(uint8_t offset, uint8_t chr, uint8_t width)
{
  setChar(offset, chr, width);
}

template <uint8_t cs_pin>
void DisplayMAX72xxMatrix<cs_pin>::setColon(bool toDot)
{
  (toDot) ? shMAX72xxMini<cs_pin, 4>::setColumn(1, 7, 0b00000001)
          : shMAX72xxMini<cs_pin, 4>::setColumn(1, 7, 0b00100100);
}

template <uint8_t cs_pin>
void DisplayMAX72xxMatrix<cs_pin>::show()
{
  shMAX72xxMini<cs_pin, 4>::update();
}

template <uint8_t cs_pin>
void DisplayMAX72xxMatrix<cs_pin>::setBrightness(uint8_t brightness)
{
  brightness = (brightness <= 15) ? brightness : 15;
  for (uint8_t i = 0; i < 4; i++)
  {
    shMAX72xxMini<cs_pin, 4>::setBrightness(i, brightness);
  }
}

// ====================================================

#if defined(MAX72XX_7SEGMENT_DISPLAY)
DisplayMAX72xx7segment<DISPLAY_CS_PIN> clkDisplay;
#else
DisplayMAX72xxMatrix<DISPLAY_CS_PIN> clkDisplay;
#endif