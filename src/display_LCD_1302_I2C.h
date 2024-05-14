#pragma once
#include <Arduino.h>
#if defined(ARDUINO_ARCH_ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif
#include <LiquidCrystal_I2C.h> // https://github.com/johnrickman/LiquidCrystal_I2C

// массивы для отрисовки сегментов цифр
byte const PROGMEM LT[8] = // 0x00
    {
        B00111,
        B01111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111};

byte const PROGMEM UB[8] = // 0x01
    {
        B11111,
        B11111,
        B11111,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000};

byte const PROGMEM RT[8] = // 0x02
    {
        B11100,
        B11110,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111};

byte const PROGMEM LL[8] = // 0x03
    {
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B01111,
        B00111};

byte const PROGMEM LB[8] = // 0x04
    {
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B11111,
        B11111,
        B11111};

byte const PROGMEM LR[8] = // 0x05
    {
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11110,
        B11100};

byte const PROGMEM MB[8] = // 0x06
    {
        B11111,
        B11111,
        B11111,
        B00000,
        B00000,
        B00000,
        B11111,
        B11111};

byte const PROGMEM BM[8] = // 0x07
    {
        B11111,
        B11111,
        B00000,
        B00000,
        B00000,
        B11111,
        B11111,
        B11111};

// массивы сегментов для отрисовки цифр
byte const PROGMEM nums[]{
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
    0xFF, 0x06, 0x04, 0xFF, 0x04, 0x04, // E
    0xFF, 0x06, 0x04, 0xFF, 0x20, 0x20, // F
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, // space
    0x01, 0xFF, 0x01, 0x20, 0xFF, 0x20, // T
    0x04, 0x20, 0x04, 0x03, 0x04, 0x05  // u
};

// ==== LCD_1602_I2C =================================
LiquidCrystal_I2C sscLcdDisplay(BUS_DISPLAY_ADDRESS, 16, 2);

#define LCD_COLON_NO_COLON 0 // нет двоеточия
#define LCD_COLON_COLON 1    // часовое двоеточие - качающиеся плюсики
#define LCD_COLON_DOT 2      // точка
#define LCD_COLON_COLON_1 3  // обычное двоеточие

struct LcdColon
{
  bool lcdShow;
  uint8_t lcdType;
};

class LCD_1602_I2C
{
private:
  uint8_t data[4];
  LcdColon col;

  void printChar(uint8_t offset, uint8_t x);
  void createChar(uint8_t cell, uint8_t *data);
  void printColon();

public:
  LCD_1602_I2C() {}

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
};

// ---- private ---------------------------------

void LCD_1602_I2C::printChar(uint8_t offset, uint8_t x)
{
  sscLcdDisplay.setCursor(offset, 0);
  for (uint8_t i = 0; i < 3; i++)
  {
    sscLcdDisplay.write(pgm_read_byte(&nums[x * 6 + i]));
  }
  sscLcdDisplay.setCursor(offset, 1);
  for (uint8_t i = 3; i < 6; i++)
  {
    sscLcdDisplay.write(pgm_read_byte(&nums[x * 6 + i]));
  }
}

void LCD_1602_I2C::createChar(uint8_t cell, uint8_t *data)
{
  uint8_t arr[8];
  for (uint8_t i = 0; i < 8; i++)
  {
    arr[i] = pgm_read_byte(&data[i]);
  }
  sscLcdDisplay.createChar(cell, arr);
}

void LCD_1602_I2C::printColon()
{
  switch (col.lcdType)
  {
  case LCD_COLON_COLON:
    if (col.lcdShow)
    {
      sscLcdDisplay.setCursor(7, 0);
      sscLcdDisplay.print(" +");
      sscLcdDisplay.setCursor(7, 1);
      sscLcdDisplay.print("+ ");
    }
    else
    {
      sscLcdDisplay.setCursor(7, 0);
      sscLcdDisplay.print("+ ");
      sscLcdDisplay.setCursor(7, 1);
      sscLcdDisplay.print(" +");
    }
    break;
  case LCD_COLON_DOT:
    sscLcdDisplay.setCursor(7, 0);
    sscLcdDisplay.print("  ");
    sscLcdDisplay.setCursor(7, 1);
    sscLcdDisplay.print(". ");
    break;
  case LCD_COLON_COLON_1:
    sscLcdDisplay.setCursor(7, 0);
    sscLcdDisplay.print("+ ");
    sscLcdDisplay.setCursor(7, 1);
    sscLcdDisplay.print("+ ");
    break;
  default:
    sscLcdDisplay.setCursor(7, 0);
    sscLcdDisplay.print("  ");
    sscLcdDisplay.setCursor(7, 1);
    sscLcdDisplay.print("  ");
  }
}

// ---- public ----------------------------------

void LCD_1602_I2C::init()
{
  sscLcdDisplay.init();
  sscLcdDisplay.backlight();

  createChar(0x00, LT);
  createChar(0x01, UB);
  createChar(0x02, RT);
  createChar(0x03, LL);
  createChar(0x04, LB);
  createChar(0x05, LR);
  createChar(0x06, MB);
  createChar(0x07, BM);
}

void LCD_1602_I2C::clear()
{
  col.lcdShow = false;
  for (uint8_t i = 0; i < 4; i++)
  {
    data[i] = 0x10;
  }
}

void LCD_1602_I2C::sleep()
{
  clear();
  sscLcdDisplay.clear();
}

void LCD_1602_I2C::setDispData(uint8_t _index, uint8_t _data)
{
  if (_index < 4)
  {
    data[_index] = _data;
  }
}

uint8_t LCD_1602_I2C::getDispData(uint8_t _index)
{
  return ((_index < 4) ? data[_index] : 0);
}

void LCD_1602_I2C::show()
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
    // sscLcdDisplay.clear();
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

void LCD_1602_I2C::setColon(bool _show, uint8_t _type = LCD_COLON_COLON)
{
  col.lcdShow = _show;
  col.lcdType = _type;
}

// ===================================================

LCD_1602_I2C clkDisplay;
