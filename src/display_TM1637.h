/**
 * @file display_TM1637.h
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * @brief Модуль, реализующий работу часов с экранами, основанными на драйвере TM1637
 * @version 1.5
 * @date 01.05.2024
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include <Arduino.h>
#include "shSimpleRTC.h"   
#include <TM1637Display.h> // https://github.com/avishorp/TM1637

#if defined(__STM32F1__)
// значение увеличено, иначе при использовании analogRead() при использовании
// аддона от Кларка дисплей начинает вести себя непредсказуемо
#define BIT_DELAY 150ul 
#else
#define BIT_DELAY 100ul
#endif

// ==== DisplayTM1637 ================================

class DisplayTM1637 : public TM1637Display
{
private:
  uint8_t data[4];
  uint8_t _brightness = 1;

public:
  DisplayTM1637(uint8_t clk_pin, uint8_t dat_pin) : TM1637Display(clk_pin, dat_pin, BIT_DELAY)
  {
    clear();
  }

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
   * @param _index разряд буфера
   * @param _data данные для установки
   */
  void setDispData(uint8_t _index, uint8_t _data);

  /**
   * @brief получение значения разряда _index буфера экрана
   *
   * @param _index разряд буфера
   * @return uint8_t
   */
  uint8_t getDispData(uint8_t _index);

  /**
   * @brief отрисовка на экране содержимого его буфера
   *
   */
  void show();

  /**
   * @brief установка яркости экрана; реально яркость будет изменена только после вызова метода show()
   *
   * @param brightness значение яркости (1..7)
   */
  void setBrightness(uint8_t brightness);
};

void DisplayTM1637::clear()
{
  for (uint8_t i = 0; i < 4; i++)
  {
    data[i] = 0x00;
  }
}

void DisplayTM1637::sleep()
{
  clear();
  TM1637Display::setSegments(data);
}

void DisplayTM1637::setDispData(uint8_t _index, uint8_t _data)
{
  if (_index < 4)
  {
    data[_index] = _data;
  }
}

uint8_t DisplayTM1637::getDispData(uint8_t _index)
{
  return ((_index < 4) ? data[_index] : 0);
}

void DisplayTM1637::show()
{
  bool flag = false;
  static uint8_t _data[4] = {0x00, 0x00, 0x00, 0x00};
  static uint8_t br = 0;
  for (uint8_t i = 0; i < 4; i++)
  {
    flag = _data[i] != data[i];
    if (flag)
    {
      break;
    }
  }
  if (!flag)
  {
    flag = br != _brightness;
  }
  // отрисовка экрана происходит только если изменился хотя бы один разряд или изменилась яркость
  if (flag)
  {
    for (uint8_t i = 0; i < 4; i++)
    {
      _data[i] = data[i];
    }
    br = _brightness;
    TM1637Display::setSegments(data);
  }
}

void DisplayTM1637::setBrightness(uint8_t brightness)
{
  _brightness = (brightness <= 7) ? brightness : 7;
  TM1637Display::setBrightness(brightness, true);
}

// ===================================================

DisplayTM1637 clkDisplay(DISPLAY_CLK_PIN, DISPLAY_DAT_PIN);