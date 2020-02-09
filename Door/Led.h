#pragma once

#include "PulseHandler.h"

class Led : public PulseHandler
{
public:
  enum Color
  {
    Red, Green, Blue
  };

  Led(Color color)
  : pin_(ColorToPin(color))
  , status_(false)
  {
    pinMode(pin_, OUTPUT);
  }

  virtual void Init()
  {
    status_ = true;
    digitalWrite(pin_, status_);
  }
  
  virtual void Action()
  {
    status_ = !status_;
    digitalWrite(pin_, status_);
  }
  
  virtual void Finish()
  {
    digitalWrite(pin_, false);
  }

private:
  uint8_t pin_;
  bool status_;
  static const uint8_t pins_[3];

  static uint8_t ColorToPin(Color color)
  {
    switch (color)
    {
      case Red:
        return pins_[0];
      case Green:
        return pins_[1];
      case Blue:
        return pins_[2];
    }
    return 0;
  }
};

static const uint8_t Led::pins_[3] = {3, 5, 6};
