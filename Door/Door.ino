#include "Led.h"
#include "Beeper.h"
#include "Pulse.h"

#define DEBUG

#ifdef DEBUG
#define Log(msg) Serial.println(msg)
#else
#define Log(msg)
#endif

/*void testa(unsigned long timeout)
{
  static unsigned long last = millis();
  unsigned long current = millis();
  if (last + timeout > current)
    return;
  last = current;
  Log(current);
}

void testb(unsigned long timeout)
{
  static unsigned long last = millis();
  unsigned long current = millis();
  if (last + timeout > current)
    return;
  last = current;
  Log(current);
}*/

Led red(Led::Red);
Pulse pulseRed(&red);
Beeper beep(2000, 100);
Pulse pulseBeep(&beep);

void setup()
{
  Serial.begin(9600);
  pulseRed.Start(4, 100, 1900);
  pulseBeep.Start(0, 1000);
}

void loop()
{
  pulseRed.Step();
//  pulseBeep.Step();
}
