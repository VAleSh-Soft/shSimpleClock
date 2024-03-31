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

  void setSegments(uint8_t *data)
  {
    shMAX72xx7Segment<cs_pin, 1, NUM_DIGITS>::clearAllDevices();
    for (uint8_t i = 0; i < 4; i++)
    {
      shMAX72xx7Segment<cs_pin, 1, NUM_DIGITS>::setChar(7 - i, data[i]);
    }

    shMAX72xx7Segment<cs_pin, 1, NUM_DIGITS>::update();
  }

public:
  DisplayMAX72xx7segment() : shMAX72xx7Segment<cs_pin, 1, NUM_DIGITS>() { clear(); }

  /**
   * @brief очистка буфера экрана, сам экран при этом не очищается
   *
   */
  void clear()
  {
    for (uint8_t i = 0; i < 4; i++)
    {
      data[i] = 0x00;
    }
  }

  /**
   * @brief очистка экрана
   *
   */
  void sleep()
  {
    clear();
    shMAX72xx7Segment<cs_pin, 1, NUM_DIGITS>::clearAllDevices(true);
  }

  /**
   * @brief установка разряда _index буфера экрана
   *
   * @param _index индекс разряда (0..3)
   * @param _data данные для установки
   */
  void setDispData(uint8_t _index, uint8_t _data)
  {
    if (_index < 4)
    {
      data[_index] = _data;
    }
  }

  /**
   * @brief получение значения разряда _index буфера экрана
   *
   * @param _index индекс разряда (0..3)
   * @return uint8_t
   */
  uint8_t getDispData(uint8_t _index)
  {
    return ((_index < 4) ? data[_index] : 0);
  }

  /**
   * @brief отрисовка на экране содержимого его буфера
   *
   */
  void show()
  {
    bool flag = false;
    static uint8_t _data[4] = {0x00, 0x00, 0x00, 0x00};
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

  /**
   * @brief вывод на экран  времени; если задать какое-то из значений hour или minute отрицательным, эта часть экрана будет очищена - можно организовать мигание, например, в процессе настройки времени
   *
   * @param hour часы
   * @param minute минуты
   * @param show_colon отображать или нет двоеточие между часами и минутами
   */
  void showTime(int8_t hour, int8_t minute, bool show_colon)
  {
    clear();
    if (hour >= 0)
    {
      data[0] = shMAX72xx7Segment<cs_pin, 1, NUM_DIGITS>::encodeDigit(hour / 10);
      data[1] = shMAX72xx7Segment<cs_pin, 1, NUM_DIGITS>::encodeDigit(hour % 10);
    }
    if (minute >= 0)
    {
      data[2] = shMAX72xx7Segment<cs_pin, 1, NUM_DIGITS>::encodeDigit(minute / 10);
      data[3] = shMAX72xx7Segment<cs_pin, 1, NUM_DIGITS>::encodeDigit(minute % 10);
    }
    if (show_colon)
    {
      data[1] |= 0x80; // для показа двоеточия установить старший бит во второй цифре
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
    data[3] = 0x63;
    // если температура отрицательная, сформировать минус впереди
    if (temp < 0)
    {
      temp = -temp;
      data[1] = minusSegments;
    }
    // если температура выходит за диапазон, сформировать строку минусов
    if (temp > 99)
    {
      for (uint8_t i = 0; i < 4; i++)
      {
        data[i] = minusSegments;
      }
    }
    else
    {
      if (temp > 9)
      {
        if (data[1] == minusSegments)
        { // если температура ниже -9, переместить минус на крайнюю левую позицию
          data[0] = minusSegments;
        }
        data[1] = shMAX72xx7Segment<cs_pin, 1, NUM_DIGITS>::encodeDigit(temp / 10);
      }
      data[2] = shMAX72xx7Segment<cs_pin, 1, NUM_DIGITS>::encodeDigit(temp % 10);
    }
  }

  /**
   * @brief вывод на экран даты
   *
   * @param date текущая дата
   */
  void showDate(DateTime date)
  {
    static uint8_t n = 0;

    switch (n)
    {
    case 0:
      showTime(date.day(), date.month(), true);
      break;
    case 1:
      showTime(20, date.year() % 100, false);
      break;
    }

    if (++n >= 2)
    {
      n = 0;
    }
  }

  /**
   * @brief установка яркости экрана
   *
   * @param brightness значение яркости (1..7)
   */
  void setBrightness(uint8_t brightness)
  {
    brightness = (brightness <= 15) ? brightness : 15;
    shMAX72xxMini<cs_pin, 1>::setBrightness(0, brightness);
  }
};

// ==== класс для матрицы 8х8х4 MAX72xx ==============

template <uint8_t cs_pin>
class DisplayMAX72xxMatrix : public shMAX72xxMini<cs_pin, 4>
{
private:
  void setNumString(uint8_t offset, uint8_t num,
                    uint8_t width = 6, uint8_t space = 1,
                    uint8_t *_data = NULL, uint8_t _data_count = 0)
  {
    uint8_t x = (width == 6) ? num / 10 : num / 10 + 0x30;
    setChar(offset, x, width, _data, _data_count);
    x = (width == 6) ? num % 10 : num % 10 + 0x30;
    setChar(offset + width + space, x, width, _data, _data_count);
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
          shMAX72xxMini<cs_pin, 4>::setColumn(j / 8, j % 8, chr_data);
        }
      }
    }
  }

public:
  DisplayMAX72xxMatrix() : shMAX72xxMini<cs_pin, 4>() { clear(); }

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
      shMAX72xxMini<cs_pin, 4>::setColumn(col / 8, col % 8, _data);
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
      result = shMAX72xxMini<cs_pin, 4>::getColumn(col / 8, col % 8);
    }
    return (result);
  }

  /**
   * @brief очистка экрана
   *
   * @param upd при false очищается только буфер экрана, при true - очищается и сам экран
   */
  void clear(bool upd = false)
  {
    shMAX72xxMini<cs_pin, 4>::clearAllDevices(upd);
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
    setChar(offset, chr, width);
  }

  /**
   * @brief вывести двоеточие в середине экрана
   *
   * @param toDot вместо двоеточия вывести точку
   */
  void setColon(bool toDot = false)
  {
    (toDot) ? shMAX72xxMini<cs_pin, 4>::setColumn(1, 7, 0b00000001)
            : shMAX72xxMini<cs_pin, 4>::setColumn(1, 7, 0b00100100);
  }

  /**
   * @brief отрисовка на экране содержимого его буфера
   *
   */
  void show()
  {
    shMAX72xxMini<cs_pin, 4>::update();
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
    setColumn(3, 7, col_sec);
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
   * @brief установка яркости экрана
   *
   * @param brightness значение яркости (0..15)
   */
  void setBrightness(uint8_t brightness)
  {
    brightness = (brightness <= 15) ? brightness : 15;
    for (uint8_t i = 0; i < 4; i++)
    {
      shMAX72xxMini<cs_pin, 4>::setBrightness(i, brightness);
    }
  }
};
