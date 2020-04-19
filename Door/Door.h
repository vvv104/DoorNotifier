#pragma once

#include "Led.h"
#include "Beeper.h"
#include "Pulse.h"
#include "Modem.h"
#include "LockSensor.h"

#ifdef DEBUG
#define COM_BUF_LEN 31
#endif

class Door : public DoorHandler, public PulseHandler, public ModemHandler
{
public:
  Door()
  : r(&red), g(&green), b(&blue)
  , beep(&beeper)
  , lock(this)
  , timer(this)
  , modem(this)
#ifdef DEBUG
  , sendingToModem(false)
  , pos_(0)
#endif
  {
#ifdef DEBUG
    buf_[COM_BUF_LEN] = 0;
#endif
  }

  void Start()
  {
#ifdef DEBUG
    Serial.begin(9600);
#endif
    lock.Start();
    modem.Start();
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
    ProcessSerial();
#endif
  }

#ifdef DEBUG
  void ProcessSerial()
  {
    int data;
    int len = Serial.available();

    for (int i = 0; i < len; ++i)
    {
      data = Serial.read();

      if (data == -1)
        break;

      if (data == '\r' || data == '\n' || data == 0)
      {
        if (sendingToModem)
        {
          modem.Write('\r');
          modem.Write('\n');
          sendingToModem = false;
          continue;
        }

        if (pos_)
        {
          buf_[pos_] = 0;
          OnCommand(buf_);
          pos_ = 0;
        }
        
        continue;
      }

      if (sendingToModem)
      {
          modem.Write(data);
          continue;
      }

      if (pos_ < COM_BUF_LEN)
        buf_[pos_++] = data;

      if (pos_ == 2 && (buf_[0] == 'a' || buf_[0] == 'A') && (buf_[1] == 't' || buf_[1] == 'T'))
      {
        pos_ = 0;
        modem.Write(buf_[0]);
        modem.Write(buf_[1]);
        sendingToModem = true;
      }
    }
  }

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

  virtual void OnReceive(const String& data)
  {
    LogVal("Modem: ", data);
  }

private:
  Pulse r, g, b;
  Pulse beep;
  Pulse timer;
  Modem modem;
  LockSensor lock;
#ifdef DEBUG
  bool sendingToModem;
  char buf_[COM_BUF_LEN + 1];
  uint8_t pos_;
#endif
};
