#pragma once

#include <SoftwareSerial.h>
#include "Debug.h"

#define MODEM_RX_PIN 8
#define MODEM_TX_PIN 9
#define MDM_BUF_LEN 127

class ModemHandler
{
public:
  virtual void OnReceive(const String& data) = 0;
};

class Modem
{
public:
  Modem(ModemHandler* handler)
  : handler_(handler)
  , modem_(MODEM_RX_PIN, MODEM_TX_PIN)
  , pos_(0)
  {
    buf_[MDM_BUF_LEN] = 0;
  }

  void Start()
  {
    modem_.begin(9600);
    Command("ATE0V0+CMEE=1;&W");
  }

  void Step()
  {
    int data;
    int len = modem_.available();

    if (len + pos_ > MDM_BUF_LEN)
      len = MDM_BUF_LEN - pos_;

    for (int i = 0; i < len; ++i)
    {
      data = modem_.read();

      if (data == -1)
        break;

      if (data == '\r' || data == '\n' || data == 0)
      {
        if (pos_)
        {
          buf_[pos_] = 0;
          handler_->OnReceive(buf_);
          pos_ = 0;
        }

        continue;
      }

      buf_[pos_++] = data;
    }

    if (pos_ == MDM_BUF_LEN)
    {
      handler_->OnReceive(buf_);
      pos_ = 0;
    }
  }

  void Command(const char* cmd)
  {
    modem_.println(cmd);
  }

  void Write(uint8_t b)
  {
    modem_.write(b);
  }

private:
  ModemHandler* handler_;
  SoftwareSerial modem_;
  char buf_[MDM_BUF_LEN + 1];
  uint8_t pos_;
};
