#pragma once

typedef void (*clkEventCallback)(void); // тип - указатель для Callback-функции

class clkClockEvent
{
private:
  clkEventCallback callback;
  uint16_t count = 0;
  uint16_t index = 0;
  bool state = false;

public:
  clkClockEvent();

  void init(uint16_t _interval, clkEventCallback _callback, bool _active = true);

  void init(clkEventCallback _callback, bool _active = true);

  // ~clkClockEvent();

  void run();

  void setState(bool _state);

  bool getState();
};

clkClockEvent::clkClockEvent() {}

void clkClockEvent::init(uint16_t _interval, clkEventCallback _callback, bool _active)
{
  index = 0;
  count = _interval;
  state = _active;
  callback = _callback;
}

void clkClockEvent::init(clkEventCallback _callback, bool _active)
{
  index = 0;
  count = 1;
  state = _active;
  callback = _callback;
}

// clkClockEvent::~clkClockEvent() { callback = NULL; }

void clkClockEvent::run()
{
  if (state && callback != NULL)
  {
    if (++index >= count)
    {
      index = 0;
      callback();
    }
  }
}

void clkClockEvent::setState(bool _state) { state = _state; }

bool clkClockEvent::getState() { return state; }

// ===================================================

clkClockEvent sscClockEvent;

#if defined(USE_ALARM)
clkClockEvent sscAlarmEvent;
#endif
