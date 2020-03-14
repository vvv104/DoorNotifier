#pragma once

#include "Led.h"
#include "Beeper.h"
#include "Pulse.h"
#include "Modem.h"
#include "LockSensor.h"

#define COM_BUF_LEN 31

class Door : public DoorHandler, public PulseHandler, public ModemHandler
{
public:
  Door()
  : r(new Led(Led::Red)), g(new Led(Led::Green)), b(new Led(Led::Blue))
  , hi(new Beeper(7040, 100)), low(new Beeper(2000, 100))
  , lock(this)
  , timer(this)
  , modem(this)
  , sendingToModem(false)
  , pos_(0)
  {
  }

  void Start()
  {
    Serial.begin(9600);
    modem.Start();
    r.Start(4, 100, 1900);
    hi.Start(4, 2000);
    buf_[COM_BUF_LEN] = 0;
  }

  void Step()
  {
    modem.Step();
    timer.Step();
    r.Step();
    hi.Step();
    ProcessSerial();
  }

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

  void OnCommand(const char* data)
  {
    Serial.print("Command: \'");
    Serial.print(data);
    Serial.println("\'");
  }

  // DoorHandler interface

  virtual void OnOpen()
  {
    timer.Start(1, 1000 * 60 * 5);
  }

  virtual void OnClose()
  {
    timer.Stop();
  }

  // PulseHandler interface

  virtual void Init()
  {
    
  }
  
  virtual void Action()
  {
    
  }
  
  virtual void Finish()
  {
    
  }

  // ModemHandler interface

  virtual void OnReceive(const char* data)
  {
    Serial.print("Recieved data: \'");
    Serial.print(data);
    Serial.println("\'");
  }

private:
  Pulse r, g, b;
  Pulse hi, low;
  Pulse timer;
  Modem modem;
  LockSensor lock;
  bool sendingToModem;
  static char buf_[COM_BUF_LEN + 1];
  uint8_t pos_;
};

static char Door::buf_[COM_BUF_LEN + 1];
