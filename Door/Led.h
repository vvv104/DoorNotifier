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
  {
    pinMode(pin_, OUTPUT);
  }

  virtual void Init()
  {
    digitalWrite(pin_, HIGH);
  }
  
  virtual void Action(unsigned int step)
  {
    digitalWrite(pin_, step % 2 ? LOW : HIGH);
  }
  
  virtual void Finish()
  {
    digitalWrite(pin_, LOW);
  }

private:
  uint8_t pin_;
};

static Led red(RED_LED_PIN);
static Led green(GREEN_LED_PIN);
static Led blue(BLUE_LED_PIN);
