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

#define WS2812_MATRIX_DISPLAY 


// ---- настройки экранов -----------------------


// ---- TM1637 -----------------------------
#if defined(TM1637_DISPLAY)

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
 *   RGB (красный, зеленый, синий)
 *   GRB (зеленый, красный, синий)
 */
#define EORDER GRB

// ---- аппаратный SPI ---------------------
// #define USE_HARDWARE_SPI // использовать аппаратный SPI для управления светодиодами для чипов с четырехпроводным управлением

/*
 * укажите порядок построения матрицы (стартовая точка - верхний левый угол);
 * возможные варианты:
 *   BY_COLUMNS - по столбцам
 *   BY_LINE - по строкам
 */
#define MX_TYPE BY_COLUMNS

// ---- цвет символов ----------------------
#define COLOR_OF_NUMBER CRGB::Red

// ---- цвет фона --------------------------
#define COLOR_OF_BACKGROUND CRGB::Black

// ---- напряжение блока питания, В --------
#define POWER_SUPPLY_VOLTAGE 5

// ---- максимальный ток блока питания, мА --
#define POWER_SUPPLY_CURRENT 2000

// ---- пины для подключения матрицы -------
#define DISPLAY_DIN_PIN 10 // пин для подключения экрана - DIN
#define DISPLAY_CLK_PIN -1 // пин для подключения экрана - CLK (для четырехпроводных схем)

#endif


// ---- опции для матричных экранов -------------

#if defined(MAX72XX_MATRIX_DISPLAY) || defined(WS2812_MATRIX_DISPLAY)

// ---- Language ---------------------------
#define USE_RU_LANGUAGE // использовать русский язык и символы кириллицы при выводе данных

// ---- анимация ---------------------------
#define USE_TICKER_FOR_DATA // использовать вывод информации в виде бегущей строки

#if defined(USE_TICKER_FOR_DATA)

// ---- скорость анимации, fps -------------
#define TICKER_SPEED 100 // скорость бегущей строки в кадрах в секунду; 

#endif

// ---- секундный стобик -------------------
// #define SHOW_SECOND_COLUMN // показывать на правом краю экрана световой столбец, отображающий количество текущих секунд в минуте

#endif



// ==== кнопки =======================================

/**
 * здесь укажите тип подключения используемых кнопок;
 * возможные варианты:
 *   PULL_UP   - с подтяжкой к VCC, при нажатии на кнопку на пин 
 *               приходит логический 0
 *   PULL_DOWN - с подтяжкой к GND, при нажатии на кнопку на пин
 *               приходит логическая 1
 */
#define BTN_INPUT_TYPE PULL_UP

/**
 * здесь укажите тип используемых кнопок;
 * возможны варианты:
 *   BTN_NO - кнопка с нормально разомкнутыми контактами
 *   BTN_NC - кнопка с нормально замкнутыми контактами
 * 
 */
#define BTN_TYPE BTN_NO

// ---- настройка параметров кнопок -------------
uint16_t constexpr TIMEOUT_OF_LONGCLICK = 1000; // интервал удержания кнопки нажатой, мс
uint16_t constexpr INTERVAL_OF_SERIAL = 100;    // интервал выдачи событий BTN_LONGCLICK при удержании кнопки нажатой, мс; задавать значение, кратное 50
uint16_t constexpr TIMEOUT_OF_DEBOUNCE = 50;    // интервал антидребезга, мс

// ---- использовать пищалку для кнопки ---------
// #define USE_BUZZER_FOR_BUTTON

// ---- пины для подключения кнопок -------------
/*
 * если кнопка не будет использоваться, нужно указать -1;
 * в этом случае ее объект не будет создаваться в памяти;
*/
#define BTN_SET_PIN 4  // пин для подключения кнопки Set
#define BTN_DOWN_PIN 6 // пин для подключения кнопки Down
#define BTN_UP_PIN 9   // пин для подключения кнопки Up



// ==== дополнительные опции ========================


// ---- календарь -------------------------------
// #define USE_CALENDAR // использовать или нет вывод даты по клику кнопкой Down


// ---- будильник -------------------------------
// #define USE_ALARM // использовать или нет будильник

#if defined(USE_ALARM)

// ---- продолжительность сигнала, секунд --
uint8_t constexpr ALARM_DURATION = 60;

// ---- задержка повтора сигнала, секунд ---
uint8_t constexpr ALARM_SNOOZE_DELAY = 120;

// ---- количество повторов сигнала --------
uint8_t constexpr ALARM_REPETITION_COUNT = 3;

// ---- одиночный клик вместо двойного -----
// #define USE_ONECLICK_TO_SET_ALARM // использовать одиночный клик кнопкой Set для входа в настройки будильника, иначе вход по двойному клику

// ---- будильник - пин светодиода ---------
#define ALARM_LED_PIN 7 // пин для подключения светодиода - индикатора будильника

#endif

// ---- режим настройки уровней яркости ---------
// #define USE_SET_BRIGHTNESS_MODE // использовать режим настройки яркости экрана


// ---- вывод температуры ------------------
// #define USE_TEMP_DATA // использовать или нет вывод на экран температуры по клику кнопкой Up



#if defined(USE_ALARM) || defined(USE_BUZZER_FOR_BUTTON)

// ==== пищалка ======================================
#define BUZZER_PIN 5    // пин для подключения пищалки

#endif



// ==== датчики ======================================


// ---- датчик освещенности ---------------------
// #define USE_LIGHT_SENSOR // использовать или нет датчик света на аналоговом входе для регулировки яркости экрана

#if defined(USE_LIGHT_SENSOR)

// ---- датчик освещенности - пин ----------
#define LIGHT_SENSOR_PIN A3 

#endif

#if defined(USE_TEMP_DATA)
// ---- датчики температуры ---------------------

// ---- DS18B20 ----------------------------
// #define USE_DS18B20 // использовать для вывода температуры датчик DS18b20

// ---- DS18B20 - пин ----------------------
#define DS18B20_PIN 8 

// ---- NTC термистор ----------------------
// #define USE_NTC     // использовать для вывода температуры NTC термистор

#if defined(USE_NTC)

#if defined(USE_NTC)

// ---- параметры NTC термистора ----------------
uint16_t constexpr RESISTOR_STD = 10000;    // сопротивление датчика при комнатной температуре (25 градусов Цельсия) в Омах;
uint16_t constexpr BALANCE_RESISTOR = 9850; // точное сопротивление второго резистора делителя напряжения, в Омах;
uint16_t constexpr BETA_COEFFICIENT = 3950;             // бета-коэффициент датчика, см. данные производителя; если данных производителя нет, коэффициент можно расчитать, исходя из бета-формулы расчета температуры, которую можно легко найти в интернете
uint8_t constexpr BIT_DEPTH = 10;           // разрядность АЦП используемого микроконтроллера; для Ардуино UNO, Nano, Pro Mini BIT_DEPTH = 10

// ---- NTC термистор - пин---------------
#define NTC_PIN A0 

#endif
#endif
#endif



// ==== прочие настройки =============================

#define AUTO_EXIT_TIMEOUT 6 // время автоматического возврата в режим показа текущего времени из любых других режимов при отсутствии активности пользователя, секунд



// ==== настройки EEPROM =============================

#if defined(USE_CALENDAR) || defined(USE_TEMP_DATA)
#define INTERVAL_FOR_AUTOSHOWDATA_EEPROM_INDEX 96 //  индекс ячейки в EEPROM для сохранения периодичности автопоказа даты и температуры
#endif
#if defined(USE_TICKER_FOR_DATA)
#define TICKER_STATE_VALUE_EEPROM_INDEX 97 // индекс ячейки в EEPROM для сохранения статуса анимации
#endif
#if defined(USE_LIGHT_SENSOR)
#define LIGHT_THRESHOLD_EEPROM_INDEX 95      // индекс ячейки в EEPROM для сохранения порога переключения яркости (uint8_t)
#define MIN_BRIGHTNESS_VALUE_EEPROM_INDEX 98 // индекс ячейки в EEPROM для сохранения  минимального значения яркости экрана (uint8_t)
#endif
#define MAX_BRIGHTNESS_VALUE_EEPROM_INDEX 99 // индекс ячейки в EEPROM для сохранения  максимального значение яркости экрана (uint8_t)
#if defined(USE_ALARM)
#define ALARM_DATA_EEPROM_INDEX 100 // индекс ячейки в EEPROM для сохранения настроек будильника (uint8_t + uint16_t)
#endif
#if defined(WS2812_MATRIX_DISPLAY)
#define COLOR_OF_NUMBER_VALUE_EEPROM_INDEX 103 // индекс ячейки в EEPROM для сохранения цвета цифр для экранов на адресных светодиодах (uint8_t x 4)
#define COLOR_OF_BACKGROUND_VALUE_EEPROM_INDEX 107 // индекс ячейки в EEPROM для сохранения цвета фона для экранов на адресных светодиодах (uint8_t x 4)
#endif



// ==== модуль RTC ===================================
/*
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




// ==== служебная информация, НЕ МЕНЯТЬ!!!!! =========

// используется или нет периодический автовывод даты и/или температуры
#if defined(USE_CALENDAR) || defined(USE_TEMP_DATA)
#define USE_AUTO_SHOW_DATA 1
#else
#define USE_AUTO_SHOW_DATA 0
#endif

// дополнительные настройки; настраиваются: уровни яркости, порог переключения яркости, период автовывода и включение/отключение анимации
#if defined(USE_SET_BRIGHTNESS_MODE) || defined(USE_LIGHT_SENSOR) || defined(USE_TICKER_FOR_DATA) || USE_AUTO_SHOW_DATA
#define USE_OTHER_SETTING 1
#else
#define USE_OTHER_SETTING 0
#endif

// используются матричные экраны
#if defined(MAX72XX_MATRIX_DISPLAY) || defined(WS2812_MATRIX_DISPLAY)
#define USE_MATRIX_DISPLAY 1
#else
#define USE_MATRIX_DISPLAY 0
#endif

