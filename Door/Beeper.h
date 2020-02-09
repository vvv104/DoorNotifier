#pragma once

#include "PulseHandler.h"

class Beeper : public PulseHandler
{
public:
  Beeper(unsigned int freq, unsigned long dur)
  : freq_(freq)
  , dur_(dur)
  {
  }

  virtual void Init()
  {
    tone(pin_, freq_, dur_);
  }
  
  virtual void Action()
  {
    tone(pin_, freq_, dur_);
  }
  
  virtual void Finish()
  {
    noTone(pin_);
  }

private:
  unsigned int freq_;
  unsigned long dur_;
  static const uint8_t pin_;
};

static const uint8_t Beeper::pin_ = 2;
