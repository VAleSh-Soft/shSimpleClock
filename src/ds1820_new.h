/**
 * @file ds1820_new.h
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * 
 * @brief Небольшая библиотека для работы с датчиками температуры DS18b20, 
 *        DS18s20, DS1825, DS28EA00 или DS1822; используется для МК RP2040.
 * 
 *        Работает только с одним датчиком, выдает температуру в градусах 
 *        Цельсия в формате int16_t, имеет контроль наличия датчика на линии, в 
 *        случае, если датчика нет, выдает -127 градусов.
 *
 *        Для работы требует наличие библиотеки OneWireNg.h - 
 *        https://github.com/pstolarz/OneWireNg
 *
 *        Методы библиотеки
 *
 *        DS1820 temp_sensor() - конструктор;
 * 
 *        void init(int8_t data_pin) - инициализация датчика; здесь:
 *           data_pin - пин, к которому подключен датчик (наличие резистора 
 *                      4.7кОм между пином данных и VCC обязательно)
 *
 *        void readData() - опрос датчика; следует вызывать не чаще одного раза
 *                          в секунду, а лучше реже, т.к. при слишком частом 
 *                          опросе микросхема датчика начинает вносить искажения
 *                          в температуру за счет собственного разогрева; 
 *                          считанные данные помещаются в поле temp;
 *
 *        int16_t getTemp() - получение ранее считанной из дачика температуры;
 * 
 * @version 1.0
 * @date 23.05.2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <Arduino.h>
#include <OneWireNg_CurrentPlatform.h>
#include <drivers/DSTherm.h>
#include <utils/Placeholder.h>

#define ERROR_TEMP -127

static Placeholder<OneWireNg_CurrentPlatform> ow;

class DS1820
{
private:
  int16_t temp = ERROR_TEMP;

  bool checkData(const DSTherm::Scratchpad &scrpd);

public:
  DS1820();

  /**
   * @brief инициализация датчика
   * 
   * @param data_pin номер пина, к которому подключен датчик
   */
  void init(int8_t data_pin);

  /**
   * @brief считывание показаний датчика; вызывать не чаще одного раза в секунду
   *
   */
  void readData();

  /**
   * @brief получение ранее считанной температуры
   *
   * @return int16_t
   */
  int16_t getTemp();
};

// ---- private ---------------------------------

bool DS1820::checkData(const DSTherm::Scratchpad &scrpd)
{
  int16_t t = scrpd.getTemp2();
  bool result = t;
  // проверка данных - при отвале датчика библиотека выдает температуру 0 С
  if (!result)
  {
    // в этом влучае проверяем заданные пороги мин и макс температуры;
    // если они не нулевые, значит все в порядке, температура реально 0 С
    result = (scrpd.getTl() != 0) && (scrpd.getTh() != 0);
  }
  if (result)
  {
    temp = (t % 16 >= 8) ? t / 16 + 1 : t / 16;
  }
  else
  {
    temp = ERROR_TEMP;
  }

  return (result);
}

// ---- public ----------------------------------

DS1820::DS1820() {}

void DS1820::init(int8_t data_pin)
{
  new (&ow) OneWireNg_CurrentPlatform(data_pin, false);

  DSTherm drv(ow);

  Placeholder<DSTherm::Scratchpad> scrpd;
  OneWireNg::ErrorCode ec = drv.readScratchpadSingle(scrpd);

  if (ec == OneWireNg::EC_SUCCESS)
  {
    // задаем разрешение 9 бит - для комнатной температуры больше нам не нужно
    // задаем пороги мин и макс температуры; все равно какие, лишь бы
    // не равнялись нулю - эти данные будут нужны при проверке валидности
    // полученной температуры
    drv.writeScratchpad(scrpd->getId(), -10, 60, DSTherm::RES_9_BIT);
    drv.copyScratchpad(scrpd->getId(), false);
    drv.convertTemp(scrpd->getId());
  }
}

void DS1820::readData()
{
  DSTherm drv(ow);
  static Placeholder<DSTherm::Scratchpad> scrpd;

  OneWireNg::ErrorCode ec = drv.readScratchpadSingle(scrpd);
  if (ec == OneWireNg::EC_SUCCESS)
  {
    if (checkData(scrpd))
    {
      // если пришли корректные данные, послать команду для следующей итерации
      drv.convertTemp(scrpd->getId(), 0);
    }
  }
  else
  {
    temp = ERROR_TEMP;
  }
}

int16_t DS1820::getTemp() { return (temp); }

// ===================================================

DS1820 sscTempSensor;
