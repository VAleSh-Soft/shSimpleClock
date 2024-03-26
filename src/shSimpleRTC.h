/**
 * @file shSimpleRTC.h
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * @brief Небольшой модуль для работы с модулями часов реального времени DS3231 и DS1307
 * @version 1.0
 * @date 11.03.2024
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include <Arduino.h>
#include <Wire.h>

#define CLOCK_ADDRESS 0x68
#define SECONDS_FROM_1970_TO_2000 946684800

// DS3231 is smart enough to know this, but keeping it for now so I don't have
// to rewrite their code. -ADW
static const uint8_t daysInMonth[] PROGMEM = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// DateTime (get everything at once) from JeeLabs / Adafruit
// Simple general-purpose date/time class (no TZ / DST / leap second handling!)
class DateTime
{
public:
  DateTime(uint32_t t = 0)
  {
    t -= SECONDS_FROM_1970_TO_2000; // bring to 2000 timestamp from 1970

    ss = t % 60;
    t /= 60;
    mm = t % 60;
    t /= 60;
    hh = t % 24;
    uint16_t days = t / 24;
    uint8_t leap;
    for (yOff = 0;; ++yOff)
    {
      leap = yOff % 4 == 0;
      if (days < 365u + leap)
        break;
      days -= 365 + leap;
    }
    for (m = 1;; ++m)
    {
      uint8_t daysPerMonth = pgm_read_byte(daysInMonth + m - 1);
      if (leap && m == 2)
        ++daysPerMonth;
      if (days < daysPerMonth)
        break;
      days -= daysPerMonth;
    }
    d = days + 1;
  }

  DateTime(uint16_t year, uint8_t month, uint8_t day,
           uint8_t hour = 0, uint8_t min = 0, uint8_t sec = 0)
  {
    yOff = year % 100;
    m = month;
    d = day;
    hh = hour;
    mm = min;
    ss = sec;
  }

  DateTime(const DateTime &copy) : yOff(copy.yOff), m(copy.m), d(copy.d),
                                   hh(copy.hh), mm(copy.mm), ss(copy.ss) {}

  DateTime(const char *date, const char *time);
  uint16_t year() const { return yOff; }
  uint8_t month() const { return m; }
  uint8_t day() const { return d; }
  uint8_t hour() const { return hh; }
  uint8_t minute() const { return mm; }
  uint8_t second() const { return ss; }
  uint8_t dayOfTheWeek() const;

  // 32-bit times as seconds since 1/1/2000
  long secondstime() const;
  // 32-bit times as seconds since 1/1/1970
  // THE ABOVE COMMENT IS CORRECT FOR LOCAL TIME; TO USE THIS COMMAND TO
  // OBTAIN TRUE UNIX TIME SINCE EPOCH, YOU MUST CALL THIS COMMAND AFTER
  // SETTING YOUR CLOCK TO UTC
  uint32_t unixtime(void) const
  {
    uint32_t t;
    uint16_t days = date2days(yOff, m, d);
    t = time2long(days, hh, mm, ss);
    t += SECONDS_FROM_1970_TO_2000; // seconds from 1970 to 2000

    return t;
  }

protected:
  uint8_t yOff, m, d, hh, mm, ss;

private:
  static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d)
  {
    if (y >= 2000)
      y -= 2000;
    uint16_t days = d;
    for (uint8_t i = 1; i < m; ++i)
      days += pgm_read_byte(daysInMonth + i - 1);
    if (m > 2 && y % 4 == 0)
      ++days;
    return days + 365 * y + (y + 3) / 4 - 1;
  }

  static long time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s)
  {
    return ((days * 24L + h) * 60 + m) * 60 + s;
  }
};

class shSimpleRTC
{
private:
  DateTime cur_time;

  uint8_t decToBcd(uint8_t val) { return ((val / 10 * 16) + (val % 10)); }
  uint8_t bcdToDec(uint8_t val) { return ((val / 16 * 10) + (val % 16)); }

  bool isClockPresent()
  {
    Wire.beginTransmission(CLOCK_ADDRESS);
    return (Wire.endTransmission() == 0);
  }

#if defined(RTC_DS3231)
  uint8_t readControlByte(bool which)
  {
    // чтение выбранного контрольного байта
    // первый байт (false) is 0x0e, второй (true) is 0x0f
    Wire.beginTransmission(CLOCK_ADDRESS);
    if (which)
    {
      // второй контрольный байт
      Wire.write(0x0f);
    }
    else
    {
      // первый контрольный байт
      Wire.write(0x0e);
    }
    Wire.endTransmission();
    Wire.requestFrom(CLOCK_ADDRESS, 1);
    return Wire.read();
  }

  void writeControlByte(uint8_t control, bool which)
  {
    // Запись выбранного контрольного байта.
    // which = false -> 0x0e, true -> 0x0f.
    Wire.beginTransmission(CLOCK_ADDRESS);
    if (which)
    {
      Wire.write(0x0f);
    }
    else
    {
      Wire.write(0x0e);
    }
    Wire.write(control);
    Wire.endTransmission();
  }
#endif

public:
  /**
   * @brief конструктор объекта DS3231
   *
   */
  shSimpleRTC() {}

  /**
   * @brief запрос текущих времени и даты из RTC и сохранение их во внутреннем буфере
   *
   */
  void now()
  {
    if (isClockPresent())
    {
      Wire.beginTransmission(CLOCK_ADDRESS);
      Wire.write(0);
      Wire.endTransmission();

      Wire.requestFrom(CLOCK_ADDRESS, 7);
      uint8_t ss = bcdToDec(Wire.read() & 0x7F);
      uint8_t mm = bcdToDec(Wire.read());
      uint8_t hh = bcdToDec(Wire.read());
      Wire.read();
      uint8_t d = bcdToDec(Wire.read());
      uint8_t m = bcdToDec(Wire.read());
      uint16_t y = bcdToDec(Wire.read());
      cur_time = DateTime(y, m, d, hh, mm, ss);
    }
    else
    {
      cur_time = DateTime(0, 1, 1, 0, 0, 0);
    }
  }

  /**
   * @brief получение текущего времени и даты
   *
   * @return DateTime
   */
  DateTime getCurTime() { return (cur_time); }

  /**
   * @brief установка текущего времени
   *
   * @param _hour часы для установки
   * @param _minute минуты для установки
   * @param _second секунды для установки
   */
  void setCurTime(uint8_t _hour, uint8_t _minute, uint8_t _second)
  {
    if (isClockPresent())
    {
      Wire.beginTransmission(CLOCK_ADDRESS);
      Wire.write(0);
      Wire.write(decToBcd(_second));
      Wire.write(decToBcd(_minute));
      Wire.write(decToBcd(_hour));
      Wire.endTransmission();
#if defined(RTC_DS3231)
      // Очищаем флаг OSF
      uint8_t temp_buffer = readControlByte(1);
      writeControlByte((temp_buffer & 0b01111111), 1);
#endif
    }
  }

  /**
   * @brief установка текущей даты
   *
   * @param _date число месяца для установки
   * @param _month номер месяца для установки (1..12)
   */
  void setCurDate(uint8_t _date, uint8_t _month)
  {
    if (isClockPresent())
    {
      Wire.beginTransmission(CLOCK_ADDRESS);
      Wire.write(0x04);
      Wire.write(decToBcd(_date));
      Wire.write(decToBcd(_month));
      Wire.endTransmission();
    }
  }

  /**
   * @brief установка текущего года
   *
   * @param _year год для установки (0..99 )
   */
  void setCurYear(uint8_t _year)
  {
    if (isClockPresent())
    {
      Wire.beginTransmission(CLOCK_ADDRESS);
      Wire.write(0x06);
      Wire.write(decToBcd(_year));
      Wire.endTransmission();
    }
  }

  /**
   * @brief возвращает температуру внутреннего датчика DS3231; работает только с DS3231
   *
   * @return uint8_t
   */
  uint8_t getTemperature()
  {
#if defined(RTC_DS3231)
    uint8_t tMSB, tLSB;
    int16_t temp3231 = -127;

    if (isClockPresent())
    { // временные регистры (11h-12h) обновляются автоматически каждые 64 секунды.
      Wire.beginTransmission(CLOCK_ADDRESS);
      Wire.write(0x11);
      Wire.endTransmission();
      Wire.requestFrom(CLOCK_ADDRESS, 2);

      if (Wire.available())
      {
        tMSB = Wire.read();
        tLSB = Wire.read();

        uint16_t x = ((((short)tMSB << 8) | (short)tLSB) >> 6);
        temp3231 = (x % 4 > 2) ? x / 4 + 1 : x / 4;
      }
    }

    return (temp3231);
#else
    // для не DS3231 вывод температуры без датчиков невозможен
    return -127;
#endif
  }

#if defined(RTC_DS3231)
  /**
   * устанавливает режим 12-часовой (true) или 24-часовой (false).
   * Одна вещь, которая меня беспокоит в том, как я это написал,
   * заключается в том, что если чтение и правка происходят в правильную
   * почасовую миллисекунду, часы будут переведены на час назад.
   * Однако не знаю, как это сделать лучше, и пока режим не устанавливается
   * часто, риск очень минимален.
   * Риск нулевой, если вы вызываете это ДО установки часа, поскольку
   * функция setHour() не меняет этот режим.
   */
  void setClockMode(bool h12)
  {
    // sets the mode to 12-hour (true) or 24-hour (false).
    // One thing that bothers me about how I've written this is that
    // if the read and right happen at the right hourly millisecnd,
    // the clock will be set back an hour. Not sure how to do it better,
    // though, and as long as one doesn't set the mode frequently it's
    // a very minimal risk.
    // It's zero risk if you call this BEFORE setting the hour, since
    // the setHour() function doesn't change this mode.

    if (isClockPresent())
    {
      uint8_t temp_buffer;

      // старт считывания байта 0x02.
      Wire.beginTransmission(CLOCK_ADDRESS);
      Wire.write(0x02);
      Wire.endTransmission();
      Wire.requestFrom(CLOCK_ADDRESS, 1);
      temp_buffer = Wire.read();

      // установка заданного флага:
      if (h12)
      {
        temp_buffer = temp_buffer | 0b01000000;
      }
      else
      {
        temp_buffer = temp_buffer & 0b10111111;
      }

      // запись байта
      Wire.beginTransmission(CLOCK_ADDRESS);
      Wire.write(0x02);
      Wire.write(temp_buffer);
      Wire.endTransmission();
    }
  }
#endif

  /**
   * @brief проверяем, запущен ли генератор
   * 
   * @return true 
   * @return false 
   */
  bool isRunning()
  {
    if (isClockPresent())
    {
      Wire.beginTransmission(CLOCK_ADDRESS);
#if defined(RTC_DS1307)
      Wire.write(0x00);
#elif defined(RTC_DS3231)
      Wire.write(0x0f);
#endif
      Wire.endTransmission();

      Wire.requestFrom(CLOCK_ADDRESS, 1);

      return (!(Wire.read() >> 7));
    }
  }

#if defined(RTC_DS3231)
  /**
   * @brief запускаем генератор, в том числе включаем работу от батареи
   * 
   */
  void startRTC()
  {
    uint8_t temp_buffer = readControlByte(0) & 0b11100111;
    // поднимаем флаг BBSQW - работа от батареи
    temp_buffer = temp_buffer | 0b01000000;
    // устанавливаем ~EOSC и INTCN в 0 - запускаем генератор
    temp_buffer = temp_buffer & 0b01111011;
    // записываем контрольный бит
    writeControlByte(temp_buffer, 0);
  }
#endif
};
