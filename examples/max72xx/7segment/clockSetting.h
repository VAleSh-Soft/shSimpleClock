#pragma once

// ==== настройки часов ==============================



// ==== экран часов ==================================

/**
 * здесь укажите, какой экран или индикатор используется для вывода информации;
 * возможные варианты:
 *   TM1637_DISPLAY           - семисегментный индикатор на драйвере TM1637
 *   MAX72XX_7SEGMENT_DISPLAY - семисегментный экран на драйверах MAX7219 или 
 *                              MAX7221, четыре цифры
 *   MAX72XX_MATRIX_DISPLAY   - матричный светодиодный экран на драйверах 
 *                              MAX7219 или MAX7221, составленный из четырех 
 *                              матриц 8х8
 *   WS2812_MATRIX_DISPLAY    - матричный светодиодный экран 8х32 на базе 
 *                              адресных светодиодов
 */

#define MAX72XX_7SEGMENT_DISPLAY 


// ==== дополнительные опции ========================


// ---- календарь -------------------------------

// #define USE_CALENDAR // использовать или нет вывод даты по клику кнопкой Down


// ---- будильник -------------------------------

// #define USE_ALARM // использовать или нет будильник

#ifdef USE_ALARM

// #define USE_ONECLICK_TO_SET_ALARM // использовать одинарный клик кнопкой Set для входа в настройки будильника, иначе вход по двойному клику

// ---- будильник - пины ------------------------
#define BUZZER_PIN 5    // пин для подключения пищалки
#define ALARM_LED_PIN 7 // пин для подключения светодиода - индикатора будильника

#endif


// ---- режим настройки уровней яркости ---------

// #define USE_SET_BRIGHTNESS_MODE // использовать режим настройки яркости экрана



// ==== датчики ======================================


// ---- датчик освещенности ---------------------

// #define USE_LIGHT_SENSOR // использовать или нет датчик света на аналоговом входе для регулировки яркости экрана

#ifdef USE_LIGHT_SENSOR

// ---- датчик освещенности - пин ----------
#define LIGHT_SENSOR_PIN A3 

#endif


// ---- вывод температуры ------------------

// #define USE_TEMP_DATA // использовать или нет вывод на экран температуры по клику кнопкой Up

#ifdef USE_TEMP_DATA

// ---- DS18B20 ----------------------------
// #define USE_DS18B20 // использовать для вывода температуры датчик DS18b20

// ---- NTC термистор ----------------------
// #define USE_NTC     // использовать для вывода температуры NTC термистор

#ifdef USE_NTC

#if defined(USE_DS18B20)

// ---- DS18B20 - пин ----------------------
#define DS18B20_PIN 8 

#elif defined(USE_NTC)

// ---- NTC термистор - пин---------------
#define NTC_PIN A0 

// ---- параметры NTC термистора ----------------
uint16_t constexpr _resistor_std = 10000;    // сопротивление датчика при комнатной температуре (25 градусов Цельсия) в Омах;
uint16_t constexpr _balance_resistor = 9850; // точное сопротивление второго резистора делителя напряжения, в Омах;
uint16_t constexpr _beta = 3950;             // бета-коэффициент датчика, см. данные производителя; если данных производителя нет, коэффициент можно расчитать, исходя из бета-формулы расчета температуры, которую можно легко найти в интернете
uint8_t constexpr _bit_depth = 10;           // разрядность АЦП используемого микроконтроллера; для Ардуино UNO, Nano, Pro Mini _bit_depth = 10

#endif
#endif
#endif



// ===== экраны ======================================


// ---- матрица из адресных светодиодов ----

#if defined(WS2812_MATRIX_DISPLAY)

/*
 * здесь укажите используемый вами тип светодиодов;
 * возможные варианты:
 *   CHIPSET_LPD6803; CHIPSET_LPD8806; CHIPSET_WS2801; CHIPSET_WS2803;
 *   CHIPSET_SM16716; CHIPSET_P9813; CHIPSET_APA102; CHIPSET_SK9822;
 *   CHIPSET_DOTSTAR; CHIPSET_NEOPIXEL; CHIPSET_SM16703; CHIPSET_TM1829;
 *   CHIPSET_TM1812; CHIPSET_TM1809; CHIPSET_TM1804; CHIPSET_TM1803;
 *   CHIPSET_UCS1903; CHIPSET_UCS1903B; CHIPSET_UCS1904; CHIPSET_UCS2903;
 *   CHIPSET_WS2812; CHIPSET_WS2852; CHIPSET_WS2812B; CHIPSET_GS1903;
 *   CHIPSET_SK6812; CHIPSET_SK6822; CHIPSET_APA106; CHIPSET_PL9823;
 *   CHIPSET_WS2811; CHIPSET_WS2813; CHIPSET_APA104; CHIPSET_WS2811_400;
 *   CHIPSET_GE8822; CHIPSET_GW6205; CHIPSET_GW6205_400; CHIPSET_LPD1886;
 *   CHIPSET_LPD1886_8BIT
 */
#define CHIPSET_WS2812B

/*
 * укажите порядок следования цветов в используемых вами светододах;
 * наиболее часто используются варианты:
 * RGB (красный, зеленый, синий)
 * GRB (зеленый, красный, синий)
 */
#define EORDER GRB

// использовать аппаратный SPI для управления светодиодами для чипов с четырехпроводным управлением
// #define USE_HARDWARE_SPI

/*
 * укажите порядок построения матрицы (стартовая точка - верхний левый угол):
 * BY_COLUMNS - по столбцам
 * BY_LINE - по строкам
 */
#define MX_TYPE BY_COLUMNS

// цвет цифр
#define COLOR_OF_NUMBER CRGB::Red

// ---- пины для подключения матрицы -------
#define DISPLAY_DIN_PIN 10 // пин для подключения экрана - DIN
#define DISPLAY_CLK_PIN -1 // пин для подключения экрана - CLK (для четырехпроводных схем)


// ---- TM1637 -----------------------------
#elif defined(TM1637_DISPLAY)

// ---- пины для подключения индикатора ----
#define DISPLAY_CLK_PIN 11 // пин для подключения экрана - CLK
#define DISPLAY_DAT_PIN 10 // пин для подключения экрана - DAT


// ---- MAX72XX ----------------------------
#elif defined(MAX72XX_7SEGMENT_DISPLAY) || defined(MAX72XX_MATRIX_DISPLAY)

// ---- пины для подключения драйвера ------
#define DISPLAY_CLK_PIN 13 // пин для подключения экрана - CLK (для Atmega168/328 не менять!!!)
#define DISPLAY_DIN_PIN 11 // пин для подключения экрана - DAT (для Atmega168/328 не менять!!!)
#define DISPLAY_CS_PIN 10  // пин для подключения экрана - CS

#endif



// ==== прочие настройки =============================

#define AUTO_EXIT_TIMEOUT 6 // время автоматического возврата в режим показа текущего времени из любых других режимов при отсутствии активности пользователя, секунд

#ifdef USE_ALARM

#define ALARM_DURATION 60        // продолжительность сигнала будильника, секунд
#define ALARM_SNOOZE_DELAY 120   // задержка повтора сигнала будильника, секунд
#define ALARM_REPETITION_COUNT 3 // количество повторов сигнала будильника

#endif



// ==== настройки EEPROM =============================

#ifdef USE_LIGHT_SENSOR
#define LIGHT_THRESHOLD_EEPROM_INDEX 97      // индекс в EEPROM для сохранения порога переключения яркости (uint8_t)
#define MIN_BRIGHTNESS_VALUE_EEPROM_INDEX 98 // индекс в EEPROM для сохранения  минимального значения яркости экрана (uint8_t)
#endif
#define MAX_BRIGHTNESS_VALUE_EEPROM_INDEX 99 // индекс в EEPROM для сохранения  максимального значение яркости экрана (uint8_t)
#ifdef USE_ALARM
#define ALARM_EEPROM_INDEX 100 // индекс в EEPROM для сохранения настроек будильника (uint8_t + uint16_t)
#endif



// ==== кнопки =======================================

/**
 * здесь укажите тип подключения используемых кнопок;
 * возможные варианты:
 *   BTN_PULL_UP   - с подтяжкой к VCC, при нажатии на кнопку на пин 
 *                   приходит логический 0
 *   BTN_PULL_DOWN - с подтяжкой к GND, при нажатии на кнопку на пин
 *                   приходит логическая 1
 */
#define BTN_PULL_UP

// ---- настройка параметров кнопок -------------

uint16_t constexpr TIMEOUT_OF_LONGCLICK = 1000; // интервал удержания кнопки нажатой
uint16_t constexpr INTERVAL_OF_SERIAL = 100;    // интервал выдачи событий BTN_LONGCLICK при удержании кнопки нажатой
uint16_t constexpr TIMEOUT_OF_DEBOUNCE = 50;    // интервал антидребезга

// ---- пины для подключения кнопок -------------
// если кнопка не используется, нужно указать -1, в этом случае ее объект не будет создаваться в памяти;
#define BTN_SET_PIN 4  // пин для подключения кнопки Set
#define BTN_DOWN_PIN 6 // пин для подключения кнопки Down
#define BTN_UP_PIN 9   // пин для подключения кнопки Up



// ==== модуль RTC ===================================
/**
 * здесь укажите используемый вами модуль часов реального времени;
 * возможные варианты:
 *   RTC_DS3231 - модуль DS3231
 *   RTC_DS1307 - модуль DS1307
 */
#define RTC_DS3231

// ---- модуль RTC - пины -----------------------
#define RTC_SDA_PIN A4 // пин для подключения вывода SDA RTC модуля (для Atmega168/328 не менять!!!)
#define RTC_SCL_PIN A5 // пин для подключения вывода SCL RTC модуля (для Atmega168/328 не менять!!!)

// ==== конец настроек часов =========================