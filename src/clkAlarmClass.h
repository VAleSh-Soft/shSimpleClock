/**
 * @file clkAlarmClass.h
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * @brief Класс, реализующий будильник
 * @version 1.5
 * @date 01.05.2024
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include <Arduino.h>
#include "shSimpleRTC.h"
#include "_eeprom.h"

#define MAX_DATA 1439 // максимальное количество минут для установки будильника (23 ч, 59 мин)

/*
 * смещение от стартового индекса в EEPROM для хранения настроек
 * общий размер настроек - 3 байта
 */
uint8_t constexpr ALARM_STATE = 0; // состояние будильника, включен/нет, uint8_t
uint8_t constexpr ALARM_POINT = 1; // точка срабатывания будильника в минутах от полуночи, uint16_t

// ==== clkAlarmClass ================================

enum AlarmState : uint8_t // состояние будильника
{
  ALARM_OFF, // будильник выключен
  ALARM_ON,  // будильник включен
  ALARM_YES  // будильник сработал
};

class clkAlarmClass
{
private:
  uint16_t eeprom_index;
  AlarmState state;

#if ALARM_LED_PIN >= 0
  uint8_t led_pin;

  void setLed();
#endif

public:
  clkAlarmClass(int8_t _led_pin, uint16_t _eeprom_index);

  /**
   * @brief инициализация будильника
   * 
   */
  void init();

  /**
   * @brief получение текущего состояния будильника
   *
   * @return AlarmState
   */
  AlarmState getAlarmState();

  /**
   * @brief установка текущего состояния будильника
   *
   * @param _state новое значение состояния будильника
   */
  void setAlarmState(AlarmState _state);

  /**
   * @brief получение информации о состоянии будильника - включен/выключен
   *
   * @return true
   * @return false
   */
  bool getOnOffAlarm();

  /**
   * @brief включение/выключение будильника
   *
   * @param _state флаг для установки состояния будильника
   */
  void setOnOffAlarm(bool _state);

  /**
   * @brief получение установленного времени срабатывания будильника в минутах от начала суток
   *
   * @return uint16_t
   */
  uint16_t getAlarmPoint();

  /**
   * @brief установка времени срабатывания будильника
   *
   * @param _time время в минутах от начала суток
   */
  void setAlarmPoint(uint16_t _time);

  /**
   * @brief проверка текущего состояния будильника
   *
   * @param _time текущее время
   */
  void tick(shDateTime _time);
};

// ---- clkAlarmClass private -------------------

#if ALARM_LED_PIN >= 0
void clkAlarmClass::setLed()
{
  uint8_t led_state = LOW;
  switch (state)
  {
  case ALARM_ON: // при включенном будильнике светодиод горит
    led_state = HIGH;
    break;
  case ALARM_YES: // при сработавшем будильнике светодиод мигает с частотой вызова метода tick()
    led_state = !led_state;
    break;
  default:
    break;
  }
  digitalWrite(led_pin, led_state);
}
#endif

// ---- clkAlarmClass public --------------------

clkAlarmClass::clkAlarmClass(int8_t _led_pin, uint16_t _eeprom_index)
{
#if ALARM_LED_PIN >= 0
  led_pin = _led_pin;
  pinMode(led_pin, OUTPUT);
#endif

  eeprom_index = _eeprom_index;
}

void clkAlarmClass::init()
{
  if (read_eeprom_8(eeprom_index + ALARM_STATE) > 1)
  {
    write_eeprom_8(eeprom_index + ALARM_STATE, 0);
  }
  if (read_eeprom_16(eeprom_index + ALARM_POINT) > MAX_DATA)
  {
    write_eeprom_16(eeprom_index + ALARM_POINT, 360);
  }
  state = (AlarmState)read_eeprom_8(eeprom_index + ALARM_STATE);
}

AlarmState clkAlarmClass::getAlarmState() { return (state); }

void clkAlarmClass::setAlarmState(AlarmState _state) { state = _state; }

bool clkAlarmClass::getOnOffAlarm() { return (bool)read_eeprom_8(eeprom_index + ALARM_STATE); }

void clkAlarmClass::setOnOffAlarm(bool _state)
{
  write_eeprom_8(eeprom_index + ALARM_STATE, (uint8_t)_state);
  state = (AlarmState)_state;
}

uint16_t clkAlarmClass::getAlarmPoint() { return (read_eeprom_16(eeprom_index + ALARM_POINT)); }

void clkAlarmClass::setAlarmPoint(uint16_t _time) { write_eeprom_16(eeprom_index + ALARM_POINT, _time); }

void clkAlarmClass::tick(shDateTime _time)
{
#if ALARM_LED_PIN >= 0
  setLed();
#endif
  switch (state)
  {
  case ALARM_ON:
    uint32_t tm;
    tm = _time.hour() * 3600ul + _time.minute() * 60ul + _time.second();
    if (tm == getAlarmPoint() * 60ul)
    {
      state = ALARM_YES;
    }
    break;
  default:
    break;
  }
}

// ==== end clkAlarmClass ============================

clkAlarmClass clkAlarm(ALARM_LED_PIN, ALARM_DATA_EEPROM_INDEX);
