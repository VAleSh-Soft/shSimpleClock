/**
 * @file clkButtons.h
 * @author Vladimir Shatalov (valesh-soft@yandex.ru)
 * @brief библиотека для работы с тактовыми кнопками;
 *        полная версия здесь - https://github.com/VAleSh-Soft/shButton
 * @version 1.0
 * @date 01.05.2024
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#if defined(USE_ALARM)
#include "clkAlarmClass.h"
#endif
#include "clkTaskManager.h"

// ==== clkButton ====================================

// флаги свойств и состояния кнопки - биты поля _flags
#define FLAG_BIT 0          // сохраненное состояние кнопки - нажата/не нажата
#define INPUTTYPE_BIT 1     // тип подключения - PULL_UP/ PULL_DOWN
#define BTNTYPE_BIT 2       // тип кнопки - нормально разомкнутая или нормально замкнутая
#define DEBOUNCE_BIT 3      // флаг включения подавления дребезга
#define VIRTUALCLICK_BIT 4  // режим виртуального клика
#define VIRTUALBUTTON_BIT 5 // флаг режима виртуальной кнопки; здесь не используется
#define ONECLICK_BIT 6      // флаг одиночного клика
#define LONGCLICK_BIT 7     // флаг длинного клика

// значения по умолчанию
#define NO_PIN 255

// типы кнопки
#define BTN_NO 0 // кнопка с нормально разомкнутыми контактами
#define BTN_NC 1 // кнопка с нормально замкнутыми контактами

// типы подключения кнопки
#define PULL_UP 0   // кнопка подтянута к VCC
#define PULL_DOWN 1 // кнопка подтянута к GND

// состояние кнопки
#define BTN_RELEASED 0 // кнопка отпущена
#define BTN_PRESSED 1  // кнопка нажата, но время удержания не вышло

// события кнопки
#define BTN_UP 2       // кнопка только что отпущена
#define BTN_DOWN 3     // кнопка только что нажата
#define BTN_DBLCLICK 4 // двойной клик - второе нажатие до истечения интервала двойного клика; если непрерывно щелкать кнопкой, будут поочередно выдаваться события BTN_DOWN и BTN_DBLCLICK

// виртуальные события кнопки
#define BTN_ONECLICK 5  // одиночный клик, следует через некоторый интервал после нажатия кнопки, если за это время не последовал двойной клик или длительное удержание кнопки нажатой; по умолчанию событие отключено
#define BTN_LONGCLICK 6 // событие, соответствующее удержанию кнопки дольше времени удержания; может быть быть непрерывным (по умолчанию), однократным или следовать через определенные интервалы

// режимы отработки удержания кнопки
#define LCM_CONTINUED 0   // непрерывное событие BTN_LONGCLICK, генерируется постоянно, пока кнопка удерживается нажатой, если интервал удержания превышен; значение по умолчанию
#define LCM_ONLYONCE 1    // однократное событие BTN_LONGCLICK, генерируется только один раз по истечении интервала удержания кнопки, после чего будет постоянно выдаваться BTN_PRESSED до тех пор, пока кнопка будет удерживаться нажатой
#define LCM_CLICKSERIES 2 // по истечении интервала удержания кнопки событие BTN_LONGCLICK генерируется постоянно через равные интервалы времени, в промежутках выдается BTN_PRESSED

enum clkButtonFlag : uint8_t
{
  CLK_BTN_FLAG_NONE, // флаг кнопки - ничего не делать
  CLK_BTN_FLAG_NEXT, // флаг кнопки - изменить значение
  CLK_BTN_FLAG_EXIT  // флаг кнопки - возврат в режим показа текущего времени
};

class clkButton
{
private:
  uint8_t _PIN = NO_PIN;                              // пин, на который посажена кнопка
  uint16_t _debounce_timeout = TIMEOUT_OF_DEBOUNCE;   // интервал подавления дребезга контактов, мс
  uint16_t _longclick_timeout = TIMEOUT_OF_LONGCLICK; // интервал удержания кнопки нажатой, мс
  uint16_t _dblclck_timeout = TIMEOUT_OF_DBLCLICK;    // интервал двойного клика, мс
  uint8_t _longclick_mode = LCM_ONLYONCE;             // режим удержания кнопки;
  uint16_t _interval_of_serial = INTERVAL_OF_SERIAL;  // интервал следования события BTN_LONGCLICK, если установлен режим LCM_CLICKSERIES, мс
  uint8_t _btn_state = BTN_RELEASED;                  // текущее состояние кнопки

  uint8_t _flags = 0x10; // набор флагов свойств и состояния кнопки;
                         // изначально включен виртуальный клик - 0b00010000
  /*
   * 0 бит - сохраненное состояние кнопки - нажата(1)/не нажата(0)
   * 1 бит - тип подключения - PULL_UP(0)/PULL_DOWN(1)
   * 2 бит - тип кнопки - нормально разомкнутая (BTN_NO(0)) или нормально замкнутая (BTN_NC(1))
   * 3 бит - флаг включения подавления дребезга - пока флаг поднят (1), изменения состояния не принимаются
   * 4 бит - режим виртуального клика, 0 - выключен, 1 - включен
   * 5 бит - флаг виртуальной кнопки, 0 - обычная кнопка, 1 - виртуальная кнопка; здесь не используется
   * 6 бит - флаг одиночного клика, 0 - не было, 1 - был одиночный клик
   * 7 бит - флаг длинного клика, 0 - не было, 1 - был длинный клик
   */

  uint32_t btn_timer = 0; // таймер отработки подавления дребезга контактов и длинного клика
  uint32_t dbl_timer = 0; // таймер двойного клика

  // получение состояния бита
  bool getFlag(uint8_t _bit);
  // установка состояния бита
  void setFlag(uint8_t _bit, bool x);

  // получение мгновенного состояния кнопки - нажата/не нажата с учетом типа подключения и без учета дребезга контактов
  bool getContactsState();

  // установка кнопке состояния "только что нажата" или "только что отпущена"
  void setBtnUpDown(bool flag, uint32_t thisMls);

  // дополнительная обработка клика
  void setAdditionalOptions();

  clkButtonFlag btn_flag = CLK_BTN_FLAG_NONE;

public:
  clkButton(uint8_t pin, bool serial_mode = false);

  // получение текущего состояния кнопки
  uint8_t getButtonState();

  // получение состояния кнопки по результату последнего опроса
  uint8_t getLastState();

  // возвращает true, если по результатам последнего вызова метода getButtonState() кнопка нажата;
  bool isButtonClosed();

  // определение одновременного нажатия двух кнопок; возвращает true, если в момент возникновения события btn_state текущей кнопки кнопка _but уже нажата; если метод вернул true, состояние обеих кнопок сбрасывается;
  bool isSecondButtonPressed(clkButton &_but, uint8_t btn_state = BTN_DOWN);

  // принудительный сброс состояния кнопки; может понадобиться, если по каким-то причинам нужно, например, исключить возникновение событий BTN_ONECLICK и BTN_DBLCLICK;
  void resetButtonState();

  clkButtonFlag getButtonFlag(bool _clear = false);

  void setButtonFlag(clkButtonFlag flag);
};

// ---- clkButton private -----------------------

bool clkButton::getFlag(uint8_t _bit)
{
  bool result = (_bit < 8) ? (((_flags) >> (_bit)) & 0x01) : false;
  return (result);
}

void clkButton::setFlag(uint8_t _bit, bool x)
{
  if (_bit < 8)
  {
    (x) ? (_flags) |= (1UL << (_bit)) : (_flags) &= ~(1UL << (_bit));
  }
}

bool clkButton::getContactsState()
{
  bool val = digitalRead(_PIN);
  if (getFlag(INPUTTYPE_BIT) == PULL_UP)
  {
    val = !val;
  }
  if (getFlag(BTNTYPE_BIT) == BTN_NC)
  {
    return (!val);
  }

  return (val);
}

void clkButton::setBtnUpDown(bool flag, uint32_t thisMls)
{
  setFlag(DEBOUNCE_BIT, false);
  setFlag(FLAG_BIT, flag);

  if (flag)
  {
    if (!getFlag(ONECLICK_BIT))
    { // если это первый клик, запустить таймер двойного клика и поднять флаг одиночного клика
      _btn_state = BTN_DOWN;
      setFlag(ONECLICK_BIT, true);
      dbl_timer = thisMls;
    }
    else if (thisMls - dbl_timer <= _dblclck_timeout)
    {
      _btn_state = BTN_DBLCLICK;
      setFlag(ONECLICK_BIT, false);
    }
  }
  else
  {
    _btn_state = BTN_UP;
  }
}

void clkButton::setAdditionalOptions()
{
  switch (_btn_state)
  {
  case BTN_DOWN:
  case BTN_DBLCLICK:
  case BTN_LONGCLICK:
    // в любом режиме, кроме стандартного, каждый клик любой кнопки перезапускает таймер автовыхода в стандартный режим
    if (clkTasks.getTaskState(clkTasks.return_to_default_mode))
    {
      clkTasks.startTask(clkTasks.return_to_default_mode);
    }
#if defined(USE_BUZZER_FOR_BUTTON) && BUZZER_PIN >= 0
    if (_btn_state != BTN_LONGCLICK)
    {
      tone(BUZZER_PIN, 2000, 25); // на каждый клик срабатывает пищалка
    }
#endif
#if defined(USE_ALARM)
    // если сработал будильник, отключить его
    if (clkAlarm.getAlarmState() == ALARM_YES)
    {
      clkAlarm.setAlarmState(ALARM_ON);
      resetButtonState();
    }
#endif
    break;
  }
}

// ---- clkButton public ------------------------

clkButton::clkButton(uint8_t pin, bool serial_mode)
{
  _PIN = pin;
  setFlag(INPUTTYPE_BIT, BTN_INPUT_TYPE);
  (BTN_INPUT_TYPE == PULL_UP) ? pinMode(_PIN, INPUT_PULLUP) : pinMode(_PIN, INPUT);
  setFlag(BTNTYPE_BIT, BTN_TYPE);
  if (serial_mode)
  {
    _longclick_mode = LCM_CLICKSERIES;
  }
}

uint8_t clkButton::getButtonState()
{
  uint32_t thisMls = millis();

  // если поднят флаг подавления дребезга и интервал еще не вышел, больше ничего не делать
  if (_debounce_timeout > 0 &&
      getFlag(DEBOUNCE_BIT) &&
      thisMls - btn_timer < _debounce_timeout)
  {
    return (_btn_state);
  }

  bool isClosed = getContactsState();
  // состояние кнопки не изменилось с прошлого опроса
  if (isClosed == getFlag(FLAG_BIT))
  { // и не поднят флаг подавления дребезга
    if (!getFlag(DEBOUNCE_BIT))
    {
      if (!isClosed)
      { // кнопка находится в отжатом состоянии
        _btn_state = BTN_RELEASED;
        if (thisMls - dbl_timer > _dblclck_timeout)
        { // если период двойного клика закончился, проверить на виртуальный клик и сбросить флаг одиночного клика
          if (getFlag(VIRTUALCLICK_BIT) && getFlag(ONECLICK_BIT))
          {
            _btn_state = BTN_ONECLICK;
          }
          setFlag(ONECLICK_BIT, false);
          setFlag(LONGCLICK_BIT, false);
        }
      }
      else if (thisMls - btn_timer < _longclick_timeout && !getFlag(LONGCLICK_BIT))
      { // кнопка находится в нажатом состоянии, но время удержания еще не вышло, и события удержания еще не было
        _btn_state = BTN_PRESSED;
      }
      else // если кнопка удерживается нажатой дольше времени удержания, то дальше возможны варианты
      {    // если события удержания еще не было, то выдать его
        if (!getFlag(LONGCLICK_BIT))
        {
          if (_longclick_mode == LCM_CLICKSERIES)
          {
            btn_timer = thisMls;
          }
          setFlag(LONGCLICK_BIT, true);
          _btn_state = BTN_LONGCLICK;
        }
        else // если это уже не первое событие удержания, то дальше согласно выбранного режима
        {
          switch (_longclick_mode)
          {
          case LCM_ONLYONCE:
            _btn_state = BTN_PRESSED;
            break;
          case LCM_CLICKSERIES:
            if (thisMls - btn_timer >= _interval_of_serial)
            {
              btn_timer = thisMls;
              _btn_state = BTN_LONGCLICK;
            }
            else
            {
              _btn_state = BTN_PRESSED;
            }
            break;
          default:
            _btn_state = BTN_LONGCLICK;
            break;
          }
        }
        setFlag(ONECLICK_BIT, false);
      }
    }
  }
  // состояние кнопки изменилось с прошлого опроса
  else
  { // если задано подавление дребезга контактов
    if (_debounce_timeout > 0)
    { // если флаг подавления еще не поднят - поднять и больше ничего не делать
      if (!getFlag(DEBOUNCE_BIT))
      {
        btn_timer = thisMls;
        setFlag(DEBOUNCE_BIT, true);
        // и заодно сбросить переменную _btn_state, чтобы не выскакивали множественные события типа BTN_DOWN пока не истечет интервал антидребезга; исключение - состояние удержания кнопки в режиме непрерывного события
        if (!(_btn_state == BTN_LONGCLICK && _longclick_mode == LCM_CONTINUED))
        {
          _btn_state = isButtonClosed();
        }
      } // иначе, если поднят, и интервал вышел - установить состояние кнопки
      else if (thisMls - btn_timer >= _debounce_timeout)
      {
        btn_timer = thisMls;
        setBtnUpDown(isClosed, thisMls);
      }
    }
    else // если подавление вообще не задано, то сразу установить состояние кнопки
    {
      btn_timer = thisMls;
      setBtnUpDown(isClosed, thisMls);
    }
  }

  setAdditionalOptions();

  return (_btn_state);
}

uint8_t clkButton::getLastState() { return (_btn_state); }

bool clkButton::isButtonClosed() { return (getFlag(FLAG_BIT)); }

bool clkButton::isSecondButtonPressed(clkButton &_but, uint8_t btn_state)
{
  bool result = false;
  if (getLastState() == btn_state && _but.isButtonClosed())
  {
    result = true;
    resetButtonState();
    _but.resetButtonState();
  }
  return (result);
}

void clkButton::resetButtonState()
{
  setFlag(ONECLICK_BIT, false);
  setFlag(LONGCLICK_BIT, false);
  setButtonFlag(CLK_BTN_FLAG_NONE);
  // сброс _btn_state в зависимости от последнего состояния - либо нажата, либо отпущена
  _btn_state = isButtonClosed();
}

clkButtonFlag clkButton::getButtonFlag(bool _clear)
{
  clkButtonFlag result = btn_flag;
  if (_clear)
  {
    btn_flag = CLK_BTN_FLAG_NONE;
  }
  return (result);
}

void clkButton::setButtonFlag(clkButtonFlag flag) { btn_flag = flag; }

// ==== end clkButton=================================

// ==== clkButtonGroup ===============================
enum clkButtonType : uint8_t
{
  CLK_BTN_SET,
  CLK_BTN_UP,
  CLK_BTN_DOWN
};

class clkButtonGroup
{
private:
  clkButton *buttons[3] = {NULL, NULL, NULL};

  bool isValidButton(clkButtonType _btn);

public:
  clkButtonGroup();

  void init();

  void setButtonFlag(clkButtonType _btn, clkButtonFlag _flag);

  clkButtonFlag getButtonFlag(clkButtonType _btn, bool _clear = false);

  uint8_t getButtonState(clkButtonType _btn);

  uint8_t getLastState(clkButtonType _btn);

  void resetButtonState(clkButtonType _btn);

  bool isButtonClosed(clkButtonType _btn);

  bool isSecondButtonPressed(clkButtonType _btn1,
                             clkButtonType _btn2,
                             uint8_t _state);
};

// ---- clkButtonGroup private ------------------

bool clkButtonGroup::isValidButton(clkButtonType _btn)
{
  if (buttons != NULL)
  {
    return (buttons[(uint8_t)_btn] != NULL);
  }

  return false;
}

// ---- clkButtonGroup public -------------------

clkButtonGroup::clkButtonGroup() {}

void clkButtonGroup::init()
{
#if (BTN_SET_PIN >= 0)
  buttons[0] = new clkButton(BTN_SET_PIN);
#endif
#if (BTN_UP_PIN >= 0)
  buttons[1] = new clkButton(BTN_UP_PIN, true);
#endif
#if (BTN_DOWN_PIN >= 0)
  buttons[2] = new clkButton(BTN_DOWN_PIN, true);
#endif
}

void clkButtonGroup::setButtonFlag(clkButtonType _btn, clkButtonFlag _flag)
{
  if (isValidButton(_btn))
  {
    buttons[(uint8_t)_btn]->setButtonFlag(_flag);
  }
}

clkButtonFlag clkButtonGroup::getButtonFlag(clkButtonType _btn, bool _clear)
{
  if (isValidButton(_btn))
  {
    return (buttons[(uint8_t)_btn]->getButtonFlag(_clear));
  }

  return CLK_BTN_FLAG_NONE;
}

uint8_t clkButtonGroup::getButtonState(clkButtonType _btn)
{
  if (isValidButton(_btn))
  {
    return (buttons[(uint8_t)_btn]->getButtonState());
  }

  return BTN_RELEASED;
}

uint8_t clkButtonGroup::getLastState(clkButtonType _btn)
{
  if (isValidButton(_btn))
  {
    return (buttons[(uint8_t)_btn]->getLastState());
  }

  return BTN_RELEASED;
}

void clkButtonGroup::resetButtonState(clkButtonType _btn)
{
  if (isValidButton(_btn))
  {
    buttons[(uint8_t)_btn]->resetButtonState();
  }
}

bool clkButtonGroup::isButtonClosed(clkButtonType _btn)
{
  if (isValidButton(_btn))
  {
    return (buttons[(uint8_t)_btn]->isButtonClosed());
  }

  return false;
}

bool clkButtonGroup::isSecondButtonPressed(clkButtonType _btn1,
                                           clkButtonType _btn2,
                                           uint8_t _state)
{
  if (isValidButton(_btn1) && isValidButton(_btn2))
  {
    return (buttons[(uint8_t)_btn1]->isSecondButtonPressed(*buttons[(uint8_t)_btn2],
                                                           _state));
  }

  return false;
}

// ==== end clkButtonGroup ===========================

clkButtonGroup clkButtons;
