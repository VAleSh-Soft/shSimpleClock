#pragma once

#include <EEPROM.h>
#if defined(WS2812_MATRIX_DISPLAY)
#include <FastLED.h>
#define CRGB_UPDATE_DATA 0x7F
#endif

// ===================================================

#if __USE_EEPROM_IN_FLASH__
void eeprom_update(uint16_t _index, uint8_t _data)
{
  if (EEPROM.read(_index) != _data)
  {
    EEPROM.write(_index, _data);
    EEPROM.commit();
  }
}
#endif

uint8_t read_eeprom_8(uint16_t _index)
{
  uint8_t result = EEPROM.read(_index);
  return result;
}

uint16_t read_eeprom_16(uint16_t _index)
{
  uint16_t _data;

#if defined(__STM32F1__)
  _data = EEPROM.read(_index);
#else
  EEPROM.get(_index, _data);
#endif
  return (_data);
}

// TODO: для stm32f4 под stm32duino наблюдается довольно длительная запись в EEPROM

void write_eeprom_8(uint16_t _index, uint8_t _data)
{
#if __USE_EEPROM_IN_FLASH__
  eeprom_update(_index, _data);
#else
  EEPROM.update(_index, _data);
#endif
}
void write_eeprom_16(uint16_t _index, uint16_t _data)
{
#if defined(__STM32F1__)
  EEPROM.update(_index, _data);
#else
  EEPROM.put(_index, _data);
#endif
  EEPROM.commit();
}

#if defined(WS2812_MATRIX_DISPLAY)
void read_eeprom_crgb(uint16_t _index, CRGB &_color)
{
  _color.r = EEPROM.read(_index + 1);
  _color.g = EEPROM.read(_index + 2);
  _color.b = EEPROM.read(_index + 3);
}

void write_eeprom_crgb(uint16_t _index, CRGB _color)
{
#if __USE_EEPROM_IN_FLASH__
  eeprom_update(_index + 1, _color.r);
  eeprom_update(_index + 2, _color.g);
  eeprom_update(_index + 3, _color.b);
  eeprom_update(_index, CRGB_UPDATE_DATA);

  EEPROM.commit();
#else
  EEPROM.update(_index + 1, _color.r);
  EEPROM.update(_index + 2, _color.g);
  EEPROM.update(_index + 3, _color.b);
  EEPROM.update(_index, CRGB_UPDATE_DATA);
#endif
}
#endif
