#pragma once

#include <EEPROM.h>
#include <FastLED.h>

// ===================================================

#if defined(__ARDUINO_ESP__)
#define EEPROM_SIZE 256
#endif

// ===================================================

#define CRGB_UPDATE_DATA 0x7F

// ===================================================

#if __ARDUINO_ESP__
void eeprom_update(uint16_t _index, uint8_t _data)
{
  if (EEPROM.read(_index) != _data)
  {
    EEPROM.write(_index, _data);
  }
}
#endif

uint8_t read_eeprom_8(uint16_t _index)
{
#if __ARDUINO_ESP__
  EEPROM.begin(EEPROM_SIZE);
#endif

  uint8_t result = EEPROM.read(_index);

#if __ARDUINO_ESP__
  EEPROM.end();
#endif

  return result;
}

uint16_t read_eeprom_16(uint16_t _index)
{
#if __ARDUINO_ESP__
  EEPROM.begin(EEPROM_SIZE);
#endif

  uint16_t _data;
  EEPROM.get(_index, _data);

#if __ARDUINO_ESP__
  EEPROM.end();
#endif

  return (_data);
}

void read_eeprom_crgb(uint16_t _index, CRGB &_color)
{
#if __ARDUINO_ESP__
  EEPROM.begin(EEPROM_SIZE);
#endif

  _color.r = EEPROM.read(_index + 1);
  _color.g = EEPROM.read(_index + 2);
  _color.b = EEPROM.read(_index + 3);

#if __ARDUINO_ESP__
  EEPROM.end();
#endif
}

void write_eeprom_8(uint16_t _index, uint8_t _data)
{
#if __ARDUINO_ESP__
  EEPROM.begin(EEPROM_SIZE);
  eeprom_update(_index, _data);
  EEPROM.commit();
  EEPROM.end();
#else

  EEPROM.update(_index, _data);

#endif
}

void write_eeprom_16(uint16_t _index, uint16_t _data)
{
#if __ARDUINO_ESP__
  EEPROM.begin(EEPROM_SIZE);
#endif

  EEPROM.put(_index, _data);

#if __ARDUINO_ESP__
  EEPROM.commit();
  EEPROM.end();
#endif
}

void write_eeprom_crgb(uint16_t _index, CRGB _color)
{
#if __ARDUINO_ESP__
  EEPROM.begin(EEPROM_SIZE);

  eeprom_update(_index + 1, _color.r);
  eeprom_update(_index + 2, _color.g);
  eeprom_update(_index + 3, _color.b);
  eeprom_update(_index, CRGB_UPDATE_DATA);

  EEPROM.commit();
  EEPROM.end();
#else

  EEPROM.update(_index + 1, _color.r);
  EEPROM.update(_index + 2, _color.g);
  EEPROM.update(_index + 3, _color.b);
  EEPROM.update(_index, CRGB_UPDATE_DATA);

#endif
}
