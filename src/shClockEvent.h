#pragma once

typedef void (*sceCallback)(void); // тип - указатель для Callback-функции

class shClockEvent
{
private:
  sceCallback callback;
  uint16_t count = 0;
  uint16_t index = 0;
  bool state = false;

public:
  shClockEvent();

  void init(uint16_t _interval, sceCallback _callback, bool _active = true);

  void init(sceCallback _callback, bool _active = true);

  // ~shClockEvent();

  void run();

  void setState(bool _state);

  bool getState();
};

shClockEvent::shClockEvent() {}

void shClockEvent::init(uint16_t _interval, sceCallback _callback, bool _active)
{
  index = 0;
  count = _interval;
  state = _active;
  callback = _callback;
}

void shClockEvent::init(sceCallback _callback, bool _active)
{
  index = 0;
  count = 1;
  state = _active;
  callback = _callback;
}

// shClockEvent::~shClockEvent() { callback = NULL; }

void shClockEvent::run()
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

void shClockEvent::setState(bool _state) { state = _state; }

bool shClockEvent::getState() { return state; }
