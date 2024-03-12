## shSimpleClock - библиотека, реализующая часы на модуле DS3231

- [Описание библиотеки](#описание-библиотеки)
- [История версий](#история-версий)
- [Использованные сторонние библиотеки](#использованные-сторонние-библиотеки)
- [Смотри так же](#смотри-так-же)

### Описание библиотеки

Библиотека позволяет легко и быстро создавать часы с использованием модуля часов реального времени (RTC) **DS3231** (или **DS1307**), нескольких типов экранов и различным набором опций (календарь, будильник, датчики освещенности и температуры и т.д) на выбор пользователя. 

В базовом варианте (на семисегментном экране **TM1637**, без календаря, будильника и датчиков освещения и температуры) часы помещаются в **ATmega8**/**ATmega88** (возможно, без загрузчика), но проверить работу на этих контроллерах возможности не было.

В часах могут быть использованы несколько типов экранов - как семисегментных индикаторов, так и светодиодных матриц.

Так же есть возможность использовать датчик освещенности в виде фоторезистора для автоматического снижения яркости экрана в темное время суток.

Часы могут выводить данные о температуре, при этом есть возможность использовать как внутренний датчик температуры микросхемы **DS3231**, так и внешние датчики.

Часы полностью автономны и не нуждаются в программном управлении, однако взаимодействие с библиотекой сторонним кодом тоже предусмотрено.

Все базовые настройки часов, как то: используемые модули, используемый набор опций, пины для подключения периферии - задаются в файле [**clockSetting.h**](docs/clock_setting.md), который нужно поместить в папку со скетчем.

**ВАЖНО:** объявление файла **clockSetting.h** в скетче должно быть **ДО** объявления библиотеки **shSimpleClock.h** (пример см. ниже):

```
#include "clockSetting.h"
#include <shSimpleClock.h>
```

Образец файла **clockSetting.h** идет вместе с библиотекой.

### История версий

Версия 1.2 - 
 - удалена опция показа даты бегущей строкой, т.к. по большому счету воспринимать информацию в таком виде не слишком удобно;
 + для матричных экранов добавлена анимация смены информации на экране; анимация - бегущая строка ));
 + добавлен интерфейс включения/отключения анимации смены информации; вход в настройку - удержание нажатой кнопки **Down**;
 + для матричных экранов добавлен автовывод даты и/или температуры через равные промежутки времени; 
 + добавлен интерфейс настройки интервала автовывода даты и/или температуры на экран; настраиваемые интервалы: 0, 1, 5, 10, 15, 30 и 60 минут; при интервале 0 автовывод отключен;
 + добавлен метод `void setAnimationState(bool _state)`, позволяющий включать/отключать анимацию вывода информации на экран;
 + добавлен метода `void setIntervalForAutoShowData(uint8_t _index)`, позволяющий установить интервал автовывода информации на экран;

Версия 1.1 - 11.03.2024
 + добавлено ежесекундное событие; событие происходит при каждой смене секунд;
 + добавлена возможность подключения к ежесекундному событию callback-функции, которая будет вызываться через заданное количество секунд;
 + добавлено событие будильника; событие происходит при срабатывании будильника;
 + добавлена возможность подключения к событию будильника callback-функции, которая будет вызываться при его срабатывании; 

Версия 1.0 - 11.03.2024
 + первая версия; библиотека выросла из проекта [simple_clock](https://github.com/VAleSh-Soft/simple_clock);

<hr>

### Использованные сторонние библиотеки

**shButton.h** - https://github.com/VAleSh-Soft/shButton<br>

Для работы с экранами используются библиотеки<br>
**TM1637Display.h** - https://github.com/avishorp/TM1637<br>
или<br>
**shMAX72xxMini.h** - https://github.com/VAleSh-Soft/shMAX72xxMini<br>
или <br>
**FastLED.h** - https://github.com/FastLED/FastLED<br>

для работы с датчиком **DS18b20** используется библиотека<br>
**OneWire.h** - https://github.com/PaulStoffregen/OneWire

<hr>

Если возникнут вопросы, пишите на valesh-soft@yandex.ru 

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
