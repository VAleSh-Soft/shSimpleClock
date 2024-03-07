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
      if (days < 365 + leap)
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
    Wire.beginTransmission(CLOCK_ADDRESS);
    Wire.write(0);
    Wire.write(decToBcd(_second));
    Wire.write(decToBcd(_minute));
    Wire.write(decToBcd(_hour));
    Wire.endTransmission();
  }

  /**
   * @brief установка текущей даты
   *
   * @param _date число месяца для установки
   * @param _month номер месяца для установки (1..2)
   */
  void setCurDate(uint8_t _date, uint8_t _month)
  {
    Wire.beginTransmission(CLOCK_ADDRESS);
    Wire.write(0x04);
    Wire.write(decToBcd(_date));
    Wire.write(decToBcd(_month));
    Wire.endTransmission();
  }

  /**
   * @brief установка текущего года
   *
   * @param _year год для установки (0..99 )
   */
  void setCurYear(uint8_t _year)
  {
    Wire.beginTransmission(CLOCK_ADDRESS);
    Wire.write(0x06);
    Wire.write(decToBcd(_year));
    Wire.endTransmission();
  }

  /**
   * @brief возвращает температуру внутреннего датчика DS3231; работает только с DS3231
   * 
   * @return uint8_t 
   */
  uint8_t getTemperature()
  {
#if defined(RTC_DS3231)
    // Updated / modified a tiny bit from "Coding Badly" and "Tri-Again"
    // http://forum.arduino.cc/index.php/topic,22301.0.html

    byte tMSB, tLSB;
    float temp3231;

    // temp registers (11h-12h) get updated automatically every 64s
    Wire.beginTransmission(CLOCK_ADDRESS);
    Wire.write(0x11);
    Wire.endTransmission();
    Wire.requestFrom(CLOCK_ADDRESS, 2);

    // Should I do more "if available" checks here?
    if (Wire.available())
    {
      tMSB = Wire.read(); // 2's complement int portion
      tLSB = Wire.read(); // fraction portion

      temp3231 = ((((short)tMSB << 8) | (short)tLSB) >> 6) / 4.0;
    }
    else
    {
      temp3231 = -127; 
    }

    return ((int)round(temp3231));
#else
    // для не DS3231 вывод температуры без датчиков невозможен
    return -127;
#endif
  }
};