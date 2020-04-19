#pragma once

#include "PulseHandler.h"

#define RED_LED_PIN 3
#define GREEN_LED_PIN 5
#define BLUE_LED_PIN 6

class Led : public PulseHandler
{
public:
  Led(uint8_t pin)
  : pin_(pin)
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
};

static Led red(RED_LED_PIN);
static Led green(GREEN_LED_PIN);
static Led blue(BLUE_LED_PIN);
