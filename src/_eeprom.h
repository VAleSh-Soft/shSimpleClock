#pragma once

#include <EEPROM.h>
#include <FastLED.h>

#define CRGB_UPDATE_DATA 0x7F

uint8_t read_eeprom_8(uint16_t _index)
{
  return (EEPROM.read(_index));
}

uint16_t read_eeprom_16(uint16_t _index)
{
  uint16_t _data;
  EEPROM.get(_index, _data);
  return (_data);
}

void read_eeprom_crgb(uint16_t _index, CRGB &_color)
{
  _color.r = EEPROM.read(_index + 1);
  _color.g = EEPROM.read(_index + 2);
  _color.b = EEPROM.read(_index + 3);
}

void write_eeprom_8(uint16_t _index, uint8_t _data)
{
  EEPROM.update(_index, _data);
}

void write_eeprom_16(uint16_t _index, uint16_t _data)
{
  EEPROM.put(_index, _data);
}

void write_eeprom_crgb(uint16_t _index, CRGB _color)
{
  EEPROM.update(_index + 1, _color.r);
  EEPROM.update(_index + 2, _color.g);
  EEPROM.update(_index + 3, _color.b);
  EEPROM.update(_index, CRGB_UPDATE_DATA);
}
