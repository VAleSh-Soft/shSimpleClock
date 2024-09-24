## shSimpleClock - библиотека, реализующая часы на модулях DS3231/DS1307/PCF8563/PCF8523

- [Описание библиотеки](#описание-библиотеки)
- [История версий](#история-версий)
- [Использованные сторонние библиотеки](#использованные-сторонние-библиотеки)
- [Аддоны, используемые для работы с другими платами](#аддоны-используемые-для-работы-с-другими-платами)
  - [ESP8266](#esp8266)
  - [ESP32](#esp32)
  - [RP2040](#rp2040)
  - [STM32](#stm32)
- [Известные проблемы](#известные-проблемы)
  - [FastLED](#fastled)
  - [STM32](#stm32-1)
- [Смотри так же](#смотри-так-же)

### Описание библиотеки

Библиотека позволяет легко и быстро создавать часы с использованием различных модулей часов реального времени (RTC), нескольких типов экранов и различным набором опций (календарь, будильник, датчики освещенности и температуры и т.д) на выбор пользователя. 

Часы могут быть [построены](docs/rtc.md) на базе как модуля **DS3231**, так и модулей **DS1307**, **PCF8563** или **PCF8523**.

В часах могут быть использованы несколько типов [экранов](docs/displays.md) - как семисегментных индикаторов, так и светодиодных матриц или вообще текстовых LCD экранов.

Так же есть возможность использовать [датчик освещенности](docs/light_sensor.md) в виде фоторезистора для автоматического снижения яркости экрана в темное время суток.

Часы могут выводить данные о [температуре](docs/show_temp.md), при этом есть возможность использовать как внутренний датчик температуры микросхемы **DS3231**, так и внешние [датчики](docs/temp_sensors.md).

В часах используются три [кнопки](docs/buttons.md) для [настройки](docs/setting.md) поддерживаемых опций.

Часы полностью автономны и не нуждаются в программном управлении, однако взаимодействие библиотеки со сторонним кодом тоже [предусмотрено](docs/api.md).

Все базовые настройки часов, как то: используемые модули, используемый набор опций, пины для подключения периферии - задаются в файле [**clockSetting.h**](docs/clock_setting.md), который нужно поместить в папку со скетчем.

***Важно!!!** - объявление файла **clockSetting.h** в скетче должно быть **ДО** объявления библиотеки **shSimpleClock.h** (пример см. ниже):*

```
#include "clockSetting.h"
#include <shSimpleClock.h>
```

Образец файла [**clockSetting.h**](clockSetting.h) идет вместе с библиотекой.

### История версий

Версия 1.8 - 13.07.2024
 + добавлена поддержка **STM32**; работа библиотеки протестирована на **STM32F103C8T6 BluePill**, частично на **STM32F401CCU6 BlackPill**, **STM32F411CEU6 BlackPill**; возможные проблемы смотрите в разделе [Известные проблемы](#stm32-1);
 * исправлена проблема с неустойчивой работой экрана **TM1637** на **BluePill** при использовании аддона **Arduino_STM32 by Roger Clark**, если часы используют аналоговые датчики - датчик освещенности и/или NTC-термистор как датчик температуры;
 + добавлен пример использования библиотеки на **STM32**;
 * заменена библиотека для работы с текстовыми экранами **LCD1602/2004**; вместо **LCD_I2C** теперь используется **LiquidCrystal_PCF8574**;
 * решена проблема неустойчивой работы текстовых экранов **LCD1602/2004** на платах **BluePill**;
 * мелкие исправления и доработки;

Версия 1.7 - 23.05.2024
 + добавлена поддержка **RP2040**;
 + добавлен пример использования библиотеки на **RP2040**;
 + решена проблема зависания **RP2040** при использовании датчика температуры **DS28b20**;
 * мелкие исправления и доработки;

Версия 1.6 - 19.05.2024
 + добавлена поддержка текстовых **LCD 1602** экранов, подключаемых с помощью адаптера **I2C** на базе чипа **PCF8574**; данные выводятся крупными цифрами в обе строки одновременно;
 + добавлен метод `void setBacklightState(bool _state)`, позволяющий включать и выключать подсветку LCD экрана;
 + добавлен пример использования библиотеки с экраном **LCD 1602**;
 + добавлена поддержка текстовых **LCD 2004** экранов, подключаемых с помощью адаптера **I2C** на базе чипа **PCF8574**; данные выводятся крупными цифрами в выбранные две строки дисплея;
 + в файле **clockSetting.h** в блоке **настройки экранов** - **LCD_I2C** добавлен рад параметров для настройки экрана - количество строк и символов, строки для вывода времени и т.д.;
 + добавлена метод `void printTextForScreen(uint8_t _offset, uint8_t _line, const char *_str)`, позволяющий выводить произвольный текст на **LCD** экран;
 + добавлен пример использования библиотеки с экраном **LCD 2004**;
 * мелкие исправления и доработки;

Версия 1.5 - 02.05.2024
 + в библиотеку внедрен код библиотеки для работы с тактовыми кнопками; библиотека [**shButton.h**](https://github.com/VAleSh-Soft/shButton) в список зависимостей больше не входит;
 + в файле **clockSetting.h** в блоке **кнопки** - **настройка параметров кнопок** добавлена переменная `TIMEOUT_OF_DBLCLICK`, определяющая интервал двойного клика кнопкой в милисекундах;
 * обновлены примеры к библиотеке;
 * мелкие исправления и доработки;

Версия 1.4 - 29.04.2024
 + добавлена возможность не использовать индикаторный светодиод будильника; для этого достаточно задать `int8_t constexpr ALARM_LED_PIN = -1;`;
 + добавлена поддержка **ESP8266**; с оглядкой на доступные пины поддерживаются все опции (возможно, не в любых сочетаниях);
 + добавлена поддержка **ESP32**; здесь с пинами гораздо богаче, по крайней мере можно одновременно использовать и датчик освещенности, и NTC-термистор в качестве датчика температуры :);
 + добавлен метод, позволяющий установить действующую разрядность АЦП микроконтроллера для корректного расчета уровней освещенности и/или температуры при использовании NTC-термистора в качестве датчика температуры;
 + добавлен интерфейс включения/отключения отображения секундного столбика для матричных экранов;
 + добавлены методы, позволяющие получить текущий и установить новый статус отображения секундного столбика;
 + добавлена поддержка модулей RTC **PCF8563** и **PCF8523**;
 + в файле **clockSetting.h** добавлена опция `#define USE_CLOCK_EVENT`, определяющая, будут ли использоваться ежесекундное событие часов и событие будильника; если их использование не планируется, эту опцию можно закомментировать для уменьшения размера прошивки;
 * мелкие исправления и доработки;

Версия 1.3 - 26.03.2024
 + добавлены четыре пользовательских режима экрана для вывода на экран произвольной информации;
 + добавлен пример работы с пользовательским режимом экрана;
 + добавлен автостарт модуля RTC, если он был остановлен из-за отключенного питания;
 * исправление ошибок и недоработок;

Версия 1.2 - 24.03.2024
 - удалена опция показа даты бегущей строкой на матричных экранах, т.к. по большому счету воспринимать информацию в таком виде не слишком удобно;
 + для матричных экранов добавлена анимация смены информации на экране; анимация - бегущая строка :); добавлен интерфейс включения/отключения анимации смены информации; вход в настройку - удержание нажатой кнопки **Down**;
 + добавлены методы, позволяющие включать/отключать анимацию вывода информации на экран, а так же получать текущее значение опции;
 + добавлен автовывод даты и/или температуры через равные промежутки времени; добавлен интерфейс настройки интервала автовывода даты и/или температуры на экран; настраиваемые интервалы: 0, 1, 5, 10, 15, 30 и 60 минут; при интервале 0 автовывод отключен; вход в настройку - удержание нажатой кнопки **Down**;
 + добавлены методы, позволяющие получить текущее значение и установить новый интервал автовывода информации на экран;
 + добавлена возможность регулировки порога переключения яркости при использовании датчика освещенности; добавлен интерфейс регулировки порога переключения яркости; вход в настройку - удержание одновременно нажатыми кнопки **Up** и **Down**;
 + добавлены методы, позволяющие получать текущее значение и устанавливать новое значение порога переключения яркости;
 + добавлена возможность настройки цвета выводимых символов для экранов, построенных на адресных светодиодах; добавлен интерфейс настройки цвета символов; вход в настройку - удержание нажатой кнопки **Down**;
 + добавлены методы, позволяющие получить текущий и установить новый цвет символов и фона экрана из адресных светодиодов;
 + для матричных экранов в выводе даты месяц теперь отображается текстом;
 + добавлена проверка модуля RTC - при его отсутствии или отвале будет отображаться время **00:00 01.01.2000**; двоеточие мигать при этом не будет;
 * исправление ошибок и недоработок;

Версия 1.1 - 11.03.2024
 + добавлено ежесекундное событие; событие происходит при каждой смене секунд;
 + добавлена возможность подключения к ежесекундному событию callback-функции, которая будет вызываться через заданное количество секунд;
 + добавлено событие будильника; событие происходит при срабатывании будильника;
 + добавлена возможность подключения к событию будильника callback-функции, которая будет вызываться при его срабатывании; 

Версия 1.0 - 11.03.2024
 + первая версия; библиотека выросла из проекта [simple_clock](https://github.com/VAleSh-Soft/simple_clock);

<hr>

***Важно!!!** - при обновлении библиотеки не забывайте проверять файлы **clockSetting.h** в ваших проектах на предмет появления новых опций.*

<hr>

### Использованные сторонние библиотеки

Для работы с экранами используются библиотеки<br>
**TM1637Display.h** - https://github.com/avishorp/TM1637<br>
или<br>
**shMAX72xxMini.h** - https://github.com/VAleSh-Soft/shMAX72xxMini<br>
или <br>
**FastLED.h** - https://github.com/FastLED/FastLED<br>
или <br>
**LiquidCrystal_PCF8574.h** - // https://github.com/mathertel/LiquidCrystal_PCF8574<br>

для работы с датчиком **DS18b20** используется библиотека<br>
**OneWire.h** - https://github.com/PaulStoffregen/OneWire<br>
или <br>
**OneWireNg.h** - https://github.com/pstolarz/OneWireNg, если используется **RP2040**

### Аддоны, используемые для работы с другими платами

Инструкцию по установке аддонов смотри в документации к **Arduino IDE.**

#### ESP8266

Для работы с **ESP8266** используется аддон **esp8266 by ESP8266 Community**, ссылка для установки:
- http://arduino.esp8266.com/stable/package_esp8266com_index.json

Если не знаете, какую плату выбрать, выбирайте **NodeMCU 1.0 (ESP-12E Module)**.

#### ESP32

Для работы с **ESP32** используется аддон **esp32 by Espressif Systems**, ссылка для установки:
- https://dl.espressif.com/dl/package_esp32_index.json

Если не знаете, какую плату выбрать, выбирайте **ESP32 Dev Module** (или **ESP32C3 Dev Module**, **ESP32S3 Dev Module**, **ESP32S2 Dev Module** - в зависимости от того, какой процессор используется на вашей плате).

#### RP2040

Для работы с **RP2040** используется аддон **Raspberry Pi Pico/RP2040**, ссылка для установки:
- https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

#### STM32

Для работы с **STM32** можно использовать как аддон **STM32 MCU based boards (stm32duino)**, ссылка для установки:
- https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json

так и аддон **Arduino_STM32 by Roger Clark**, ссылка на **github.com**:
- https://github.com/rogerclarkmelbourne/Arduino_STM3

Инструкция по установке:
- https://github.com/rogerclarkmelbourne/Arduino_STM32/wiki/Installation

**ВАЖНО!!! - аддон stm32duino разрабатывается и поддерживается производителем микроконтроллеров STM32 - STMicroelectronics и, соответственно, более предпочтителен к использованию.**


### Известные проблемы

#### FastLED

Библиотека [FastLED.h](https://github.com/FastLED/FastLED) - популярная библиотека для работы с адресными светодиодами. Поддерживает работу с множеством чипов и микроконтроллеров. Однако подобная универсальность иногда выходит боком в виде невозможности компиляции какого-то кода.

Библиотека **shSimpleClock** разрабатывалась и тестировалась с использованием библиотеки **FastLED** версии 3.5, поэтому, если у вас возникают проблемы с использованием более поздних версий, попробуйте откатиться к этой версии.

Известные на данный момент проблемы:
1. Версия **FastLED** 3.6 не компилируется при использовании **RP2040**; возможно, в будущем эту ошибку поправят, но обновляется **FastLED** не слишком часто;
2. Есть серьезные проблемы при использовании **STM32**:
  - при использовании официального аддона от **STMicroelectronics** заставить работать **FastLED** не удалось;
  - при использовании аддона от **Roger Clark** библиотека работает только после замены нескольких файлов на [доработанныe](https://github.com/ademuri/FastLED-STM32/tree/stm32f103/platforms/arm/stm32) (**FastLED/src/platforms/arm/stm32/**);

#### STM32

1. Работа библиотеки не тестировалась на **BlackPill** (**STM32F401CCU6** и **STM32F411CEU6**) с аддоном **Arduino_STM32 by Roger Clark**; как минимум для этих плат в нем не реализована работа с **EEPROM**; разбираться с этим вопросом пока лень :) ;

С аддоном **STM32 MCU based boards (stm32duino)** **BlackPill** работает без проблем;

2. ~~Текстовые экраны **LCD 1602/2004** могут работать неустойчиво от экземпляра к экземпляру, при чем, один и тот же экран может глючить с одной платой и нормально работать с другой и наоборот; проблема была выявлена при использовании **BluePill** с аддоном **stm32duino**; с аддоном **Arduino_STM32** экраны отказываются работать напрочь; с этим разбираться пока тоже некогда :~~ Решено

На платах **BlackPill** проблема замечена не была;

<hr>

### Смотри так же
- [Используемые модули RTC](docs/rtc.md)
- [Используемые в часах экраны](docs/displays.md)
- [Управление часами - кнопки](docs/buttons.md)
- Дополнительные опции:
  - [Календарь](docs/calendar.md)
  - [Будильник](docs/alarm.md)
  - [Регулировка уровней яркости экрана](docs/br_adjust.md)
  - [Отображение температуры](docs/show_temp.md)
  - [Опции для матричных экранов](docs/matrix.md)
- Внешние датчики:
  - [Датчик освещенности](docs/light_sensor.md)
  - [Датчики температуры](docs/temp_sensors.md)
- [Описание файла `clockSetting.h`](docs/clock_setting.md)
- [Взаимодействие с внешним кодом (API библиотеки)](docs/api.md)
- [Руководство по настройкам часов](docs/setting.md)

<hr>

Если возникнут вопросы, пишите на valesh-soft@yandex.ru 
