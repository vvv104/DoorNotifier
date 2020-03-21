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
  : r(new Led(Led::Red)), g(new Led(Led::Green)), b(new Led(Led::Blue))
  , hi(new Beeper(7040)), low(new Beeper(2000))
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
    modem.Start();
    //g.Start();
    hi.Start(1, 100);
  }

  void Step()
  {
    modem.Step();
    timer.Step();
    r.Step();
    g.Step();
    b.Step();
    hi.Step();
    low.Step();
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
    if (data.equalsIgnoreCase("open"))
      lock.Open();
    else if (data.equalsIgnoreCase("close"))
      lock.Close();
  }
#endif

  // DoorHandler interface

  virtual void OnOpen()
  {
    low.Start(0, 100, 900);
    timer.Start(1, 1000);
  }

  virtual void OnClose()
  {
    low.Stop();
    timer.Stop();
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
#ifdef DEBUG
    Serial.print("Modem: \'");
    Serial.print(data);
    Serial.println("\'");
#endif
  }

private:
  Pulse r, g, b;
  Pulse hi, low;
  Pulse timer;
  Modem modem;
  LockSensor lock;
#ifdef DEBUG
  bool sendingToModem;
  static char buf_[COM_BUF_LEN + 1];
  uint8_t pos_;
#endif
};

#ifdef DEBUG
static char Door::buf_[COM_BUF_LEN + 1];
#endif
