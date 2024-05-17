## shSimpleClock - библиотека, реализующая часы на модулях DS3231/DS1307/PCF8563/PCF8523

- [Описание библиотеки](#описание-библиотеки)
- [История версий](#история-версий)
- [Использованные сторонние библиотеки](#использованные-сторонние-библиотеки)
- [Смотри так же](#смотри-так-же)

### Описание библиотеки

Библиотека позволяет легко и быстро создавать часы с использованием различных модулей часов реального времени (RTC), нескольких типов экранов и различным набором опций (календарь, будильник, датчики освещенности и температуры и т.д) на выбор пользователя. 

Часы могут быть [построены](docs/rtc.md) на базе как модуля **DS3231**, так и модулей **DS1307**, **PCF8563** или **PCF8523**.

В часах могут быть использованы несколько типов [экранов](docs/displays.md) - как семисегментных индикаторов, так и светодиодных матриц.

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

Версия 1.6 - 15.05.2024
 + добавлена поддержка текстовых **LCD 1602** экранов, подключаемых с помощью адаптера **I2C** на базе чипа **PCF8574**; данные выводятся крупными цифрами в обе строки одновременно;
 + добавлен метод `void setBacklightState(bool _state)`, позволяющий включать и выключать подсветку LCD экрана;
 + добавлен пример использования библиотеки с экраном **LCD 1602**;
 + добавлена поддержка текстовых **LCD 2004** экранов, подключаемых с помощью адаптера **I2C** на базе чипа **PCF8574**; данные выводятся в выбранные две строки дисплея;
 + в файле **clockSetting.h** в блоке **настройки экранов** - **LCD_1602_I2C** добавлен рад параметров для настройки экрана - количество строк и символов, строки для вывода данных часов и т.д.;
 + добавлена метод `void printTextForScreen(uint8_t _offset, uint8_t _line, char *_str)`, позволяющий выводить произвольный текст на экран;
 * мелкие исправления и доработки;

Версия 1.5 - 02.05.2024
 + в библиотеку внедрен код библиотеки для работы с тактовыми кнопками; библиотека [**shButton.h**](https://github.com/VAleSh-Soft/shButton) в список зависимостей больше не входит;
 + в файле **clockSetting.h** в блоке **кнопки** - **настройка параметров кнопок** добавлена переменная `TIMEOUT_OF_DBLCLICK`, определяющая интервал двойного клика кнопкой в милисекундах;
 * обновлены примеры к библиотеке;
 * мелкие исправления и доработки;

Версия 1.4 - 29.04.2024
 + добавлена возможность не использовать индикаторный светодиод будильника; для этого достаточно задать `int8_t constexpr ALARM_LED_PIN = -1;`;
 + добавлена поддержка **ESP8266**; с оглядкой на доступные пины поддерживаются все опции (возможно, не в любых сочетаниях);
 + добавлена поддержка **ESP32**; здесь с пинами гораздо богаче, по крайней мере можно одновременно использовать и датчик освещенности, и NTC-термистор в качестве датчика температуры ));
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
 + для матричных экранов добавлена анимация смены информации на экране; анимация - бегущая строка )); добавлен интерфейс включения/отключения анимации смены информации; вход в настройку - удержание нажатой кнопки **Down**;
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
**LCD_I2C.h** - https://github.com/blackhack/LCD_I2C<br>

для работы с датчиком **DS18b20** используется библиотека<br>
**OneWire.h** - https://github.com/PaulStoffregen/OneWire

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
