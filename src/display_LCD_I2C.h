/**
 * @file display_LCD_I2C.h
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * @brief модуль, реализующий работу часов с текстовыми LCD  экранами,
 *        подключаемыми с помощью адаптера I2C на базе чипа PCF8574
 * @version 1.0
 * @date 17.05.2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include <Arduino.h>
#if defined(ARDUINO_ARCH_ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif
#include <LCD_I2C.h> // https://github.com/blackhack/LCD_I2C

// массивы для отрисовки сегментов цифр
static uint8_t const LT[8] PROGMEM = // 0x00
    {
        0b00111,
        0b01111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111};

static uint8_t const UB[8] PROGMEM = // 0x01
    {
        0b11111,
        0b11111,
        0b11111,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000};

static uint8_t const RT[8] PROGMEM = // 0x02
    {
        0b11100,
        0b11110,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111};

static uint8_t const LL[8] PROGMEM = // 0x03
    {
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b01111,
        0b00111};

static uint8_t const LB[8] PROGMEM = // 0x04
    {
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b11111,
        0b11111,
        0b11111};

static uint8_t const LR[8] PROGMEM = // 0x05
    {
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11110,
        0b11100};

static uint8_t const MB[8] PROGMEM = // 0x06
    {
        0b11111,
        0b11111,
        0b11111,
        0b00000,
        0b00000,
        0b00000,
        0b11111,
        0b11111};

static uint8_t const BM[8] PROGMEM = // 0x07
    {
        0b11111,
        0b11111,
        0b00000,
        0b00000,
        0b00000,
        0b11111,
        0b11111,
        0b11111};
#if __USE_ARDUINO_ESP__
static const uint8_t *const arr_char[] PROGMEM = {LT, UB, RT, LL, LB, LR, MB, BM};
#endif

// массивы сегментов для отрисовки цифр
uint8_t const PROGMEM nums[]{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, // 0
    0x01, 0x02, 0x20, 0x04, 0xFF, 0x04, // 1
    0x01, 0x01, 0x02, 0x00, 0x07, 0x07, // 2
    0x01, 0x06, 0x02, 0x04, 0x04, 0x05, // 3
    0x03, 0x04, 0xFF, 0x20, 0x20, 0xFF, // 4
    0x03, 0x06, 0x06, 0x04, 0x04, 0x05, // 5
    0x00, 0x06, 0x06, 0x03, 0x04, 0x05, // 6
    0x01, 0x01, 0x05, 0x20, 0x00, 0x20, // 7
    0x00, 0x06, 0x02, 0x03, 0x04, 0x05, // 8
    0x00, 0x01, 0x02, 0x07, 0x07, 0x05, // 9
    0x00, 0x01, 0x02, 0xFF, 0x01, 0xFF, // A
    0xFF, 0x04, 0x04, 0xFF, 0x04, 0x05, // b
    0x00, 0x01, 0x01, 0x03, 0x04, 0x04, // C
    0xFF, 0x01, 0x02, 0xFF, 0x04, 0x05, // D
    0xFF, 0x06, 0x01, 0xFF, 0x04, 0x04, // E
    0xFF, 0x06, 0x01, 0xFF, 0x20, 0x20, // F
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, // space
    0x01, 0xFF, 0x01, 0x20, 0xFF, 0x20, // T
    0x04, 0x20, 0x04, 0x03, 0x04, 0x05, // u
    0xFF, 0x20, 0x20, 0xFF, 0x04, 0x04, // L
    0x20, 0x04, 0x20, 0x00, 0x04, 0x02, // on
    0x20, 0x20, 0x20, 0x04, 0x04, 0x04, // off
    0x20, 0x04, 0x04, 0x20, 0x20, 0x20, // minus
    0x67, 0x72, 0x64, 0x20, 0x43, 0x20  // grad
};

// ==== LCD_I2C_Display ==============================
LCD_I2C sscLcdDisplay(BUS_DISPLAY_ADDRESS,
                      NUMBER_OF_CHAR_PER_LINE,
                      NUMBER_OF_LINE_PER_DISPLAY);

#define LCD_COLON_NO_COLON 0 // нет двоеточия
#define LCD_COLON_COLON 1    // часовое двоеточие - качающиеся плюсики
#define LCD_COLON_DOT 2      // точка
#define LCD_COLON_COLON_1 3  // обычное двоеточие

struct LcdColon
{
  bool lcdShow;
  uint8_t lcdType;
};

class LCD_I2C_Display
{
private:
  uint8_t data[4];
  LcdColon col;

  void printChar(uint8_t offset, uint8_t x);
#if __USE_ARDUINO_ESP__
  void createChar();
#else
  void createChar(uint8_t cell, const uint8_t *data);
#endif
  void printColon();
  bool isDisplayPresent();

public:
  LCD_I2C_Display() {}

  void init();

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
   * @brief установить двоеточие
   *
   * @param _show показать или нет
   * @param _type тип двоеточия: никакого, мигающее двоеточие, точка
   */
  void setColon(bool _show, uint8_t _type = LCD_COLON_COLON);

  /**
   * @brief включить или выключить подсветку экрана
   *
   * @param _state true - включить, false - выключить
   */
  void setBacklightState(bool _state);
};

// ---- private ---------------------------------

void LCD_I2C_Display::printChar(uint8_t offset, uint8_t x)
{
  offset += OFFSET_FOR_FIRST_CHAR;

  sscLcdDisplay.setCursor(offset, FIRST_LINE_NUMBER);
  for (uint8_t i = 0; i < 3; i++)
  {
    sscLcdDisplay.write(pgm_read_byte(&nums[x * 6 + i]));
  }
  sscLcdDisplay.setCursor(offset, SECOND_LINE_NUMBER);
  for (uint8_t i = 3; i < 6; i++)
  {
    sscLcdDisplay.write(pgm_read_byte(&nums[x * 6 + i]));
  }
}

#if __USE_ARDUINO_ESP__
void LCD_I2C_Display::createChar()
{
  for (uint8_t j = 0; j < 8; j++)
  {
    uint8_t arr[8];
    for (uint8_t i = 0; i < 8; i++)
    {
      arr[i] = pgm_read_byte(&arr_char[j][i]);
    }
    sscLcdDisplay.createChar(j, arr);
  }
}
#else
void LCD_I2C_Display::createChar(uint8_t cell, const uint8_t *data)
{
  uint8_t arr[8];
  for (uint8_t i = 0; i < 8; i++)
  {
    arr[i] = pgm_read_byte(&data[i]);
  }
  sscLcdDisplay.createChar(cell, arr);
}
#endif

void LCD_I2C_Display::printColon()
{
  uint8_t offset = OFFSET_FOR_FIRST_CHAR + 7;
  switch (col.lcdType)
  {
  case LCD_COLON_COLON:
    if (col.lcdShow)
    {
      sscLcdDisplay.setCursor(offset, FIRST_LINE_NUMBER);
      sscLcdDisplay.print(" +");
      sscLcdDisplay.setCursor(offset, SECOND_LINE_NUMBER);
      sscLcdDisplay.print("+ ");
    }
    else
    {
      sscLcdDisplay.setCursor(offset, FIRST_LINE_NUMBER);
      sscLcdDisplay.print("+ ");
      sscLcdDisplay.setCursor(offset, SECOND_LINE_NUMBER);
      sscLcdDisplay.print(" +");
    }
    break;
  case LCD_COLON_DOT:
    sscLcdDisplay.setCursor(offset, FIRST_LINE_NUMBER);
    sscLcdDisplay.print("  ");
    sscLcdDisplay.setCursor(offset, SECOND_LINE_NUMBER);
    sscLcdDisplay.print(". ");
    break;
  case LCD_COLON_COLON_1:
    sscLcdDisplay.setCursor(offset, FIRST_LINE_NUMBER);
    sscLcdDisplay.print("* ");
    sscLcdDisplay.setCursor(offset, SECOND_LINE_NUMBER);
    sscLcdDisplay.print("* ");
    break;
  default:
    sscLcdDisplay.setCursor(offset, FIRST_LINE_NUMBER);
    sscLcdDisplay.print("  ");
    sscLcdDisplay.setCursor(offset, SECOND_LINE_NUMBER);
    sscLcdDisplay.print("  ");
  }
}

bool LCD_I2C_Display::isDisplayPresent()
{
  Wire.beginTransmission(BUS_DISPLAY_ADDRESS);
  return (Wire.endTransmission() == 0);
}

// ---- public ----------------------------------

void LCD_I2C_Display::init()
{
  if (isDisplayPresent())
  {
    sscLcdDisplay.begin(false);
    sscLcdDisplay.backlight();

#if __USE_ARDUINO_ESP__
    createChar();
#else
    createChar(0x00, LT);
    createChar(0x01, UB);
    createChar(0x02, RT);
    createChar(0x03, LL);
    createChar(0x04, LB);
    createChar(0x05, LR);
    createChar(0x06, MB);
    createChar(0x07, BM);
#endif
  }
}

void LCD_I2C_Display::clear()
{
  col.lcdShow = false;
  for (uint8_t i = 0; i < 4; i++)
  {
    data[i] = 0x10;
  }
}

void LCD_I2C_Display::sleep()
{
  clear();
  if (isDisplayPresent())
  {
    sscLcdDisplay.clear();
  }
}

void LCD_I2C_Display::setDispData(uint8_t _index, uint8_t _data)
{
  if (_index < 4)
  {
    data[_index] = _data;
  }
}

uint8_t LCD_I2C_Display::getDispData(uint8_t _index)
{
  return ((_index < 4) ? data[_index] : 0);
}

void LCD_I2C_Display::show()
{
  bool flag = false;
  static uint8_t _data[4];
  static LcdColon _col = col;
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
    flag = (_col.lcdShow != col.lcdShow) || (_col.lcdType != col.lcdType);
  }

  // отрисовка экрана происходит только если изменился хотя бы один разряд
  if (flag)
  {
    if (isDisplayPresent())
    { 
      for (uint8_t i = 0; i < 4; i++)
      {
        _data[i] = data[i];
        uint8_t offset = (i < 2) ? i * 4 : i * 4 + 1;

        printChar(offset, data[i]);
      }
      _col.lcdShow = col.lcdShow;
      _col.lcdType = col.lcdType;
      printColon();
    }
  }
}

void LCD_I2C_Display::setColon(bool _show, uint8_t _type)
{
  col.lcdShow = _show;
  col.lcdType = _type;
}

void LCD_I2C_Display::setBacklightState(bool _state)
{
  if (isDisplayPresent())
  {
    (_state) ? sscLcdDisplay.backlight() : sscLcdDisplay.noBacklight();
  }
}

// ===================================================

LCD_I2C_Display clkDisplay;
