#pragma once

#include "PulseHandler.h"

#define BEEPER_PIN 2

class Beeper : public PulseHandler
{
public:
  Beeper()
  : status_(false)
  , frequency_(0)
  {
  }

  virtual void Init()
  {
    status_ = true;
    tone(BEEPER_PIN, frequency_);
  }
  
  virtual void Action()
  {
    status_ = !status_;
    if (status_)
      tone(BEEPER_PIN, frequency_);
    else
      noTone(BEEPER_PIN);
  }
  
  virtual void Finish()
  {
    noTone(BEEPER_PIN);
  }

  void Beep(unsigned long duration = 0) const
  {
    tone(BEEPER_PIN, frequency_, duration);
  }

  const Beeper& operator()(unsigned int frequency)
  {
    frequency_ = frequency;
    return *this;
  }

private:
  bool status_;
  unsigned int frequency_;
};

static Beeper beeper;
