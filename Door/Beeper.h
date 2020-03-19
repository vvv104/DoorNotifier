#pragma once

#include "PulseHandler.h"

class Beeper : public PulseHandler
{
public:
  Beeper(unsigned int freq)
  : freq_(freq)
  , status_(false)
  {
  }

  virtual void Init()
  {
    status_ = true;
    tone(pin_, freq_);
  }
  
  virtual void Action()
  {
    status_ = !status_;
    if (status_)
      tone(pin_, freq_);
    else
      noTone(pin_);
  }
  
  virtual void Finish()
  {
    noTone(pin_);
  }

private:
  unsigned int freq_;
  bool status_;
  static const uint8_t pin_;
};

static const uint8_t Beeper::pin_ = 2;
