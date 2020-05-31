#pragma once

#include "PulseHandler.h"

#define BEEPER_PIN 2

class Beeper : public PulseHandler
{
public:
  Beeper()
  : frequency_(0)
  {
  }

  virtual void Init()
  {
    tone(BEEPER_PIN, frequency_);
  }
  
  virtual void Action(unsigned int step)
  {
    if (step % 2)
      noTone(BEEPER_PIN);
    else
      tone(BEEPER_PIN, frequency_);
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
  unsigned int frequency_;
};

static Beeper beeper;
