#pragma once

#include <EEPROM.h>

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

  void write_eeprom_8(uint16_t _index, uint8_t _data)
  {
    EEPROM.update(_index, _data);
  }

  void write_eeprom_16(uint16_t _index, uint16_t _data)
  {
    EEPROM.put(_index, _data);
  }

