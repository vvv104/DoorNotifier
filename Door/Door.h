#pragma once

#include "Led.h"
#include "Beeper.h"
#include "Pulse.h"
#include "Modem.h"
#include "LockSensor.h"
#include "CommandProcessor.h"

class Door : public DoorHandler, public PulseHandler, public ModemHandler
#ifdef DEBUG
    , public CommandHandler
#endif
{
public:
  Door()
  : r(&red), g(&green), b(&blue)
  , beep(&beeper)
  , lock(this)
  , timer(this)
  , modem(this)
#ifdef DEBUG
  , command(&modem, this)
#endif
  {
  }

  void Start()
  {
#ifdef DEBUG
    command.Start();
#endif
    modem.Start();
    lock.Start();
    b.Start();
    beeper(7040).Beep(100);
  }

  void Step()
  {
    lock.Step();
    modem.Step();
    timer.Step();
    r.Step();
    g.Step();
    b.Step();
    beep.Step();
#ifdef DEBUG
    command.Step();
#endif
  }

#ifdef DEBUG
  // CommandHandler interface
  void OnCommand(const String& data)
  {
    if (data == "+")
      lock.Rotate(true);
    else if (data == "-")
      lock.Rotate(false);
  }
#endif

  // DoorHandler interface
  void OnStateChanged(DoorState state)
  {
    LogVal("Door state: ", state)

    switch (state)
    {
      case dsOpen:
        g.Stop();
        b.Stop();
        r.Start(0, 500, 500);
        beeper(2637).Beep(250);
        timer.Start(1, 1000, 2000);
        break;
      case dsLocked1:
        r.Stop();
        b.Stop();
        g.Start(0, 500, 500);
        beeper(2093).Beep(250);
        timer.Stop();
        break;
      case dsLocked2:
        r.Stop();
        b.Stop();
        g.Start(0, 100, 4900);
        beeper(1760).Beep(250);
        timer.Stop();
        break;
      case dsUnknown:
      default:
        g.Stop();
        b.Stop();
        r.Start();
        beeper(3520).Beep(250);
        timer.Start(1, 1000, 2000);
        break;
    }
  }

  // PulseHandler interface

  virtual void Init()
  {
    Log("door->init");
  }
  
  virtual void Action()
  {
    Log("door->action");
  }
  
  virtual void Finish()
  {
    Log("door->finish");
  }

  // ModemHandler interface
  
  virtual void CallSuccess()
  {
    
  }
  
  virtual void CallFailure()
  {
    
  }

private:
  Pulse r, g, b;
  Pulse beep;
  Pulse timer;
  Modem modem;
  LockSensor lock;
#ifdef DEBUG
  CommandProcessor command;
#endif
};
