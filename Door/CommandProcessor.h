#pragma once

#ifdef DEBUG
#define COM_BUF_LEN 31

class CommandHandler
{
public:
  virtual void OnCommand(const String& data) = 0;
};

class CommandProcessor
{
public:
  CommandProcessor(Modem* modem, CommandHandler* handler)
  : sendingToModem_(false)
  , pos_(0)
  , modem_(modem)
  , handler_(handler)
  {
    buf_[COM_BUF_LEN] = 0;
  }

  void Start()
  {
    Serial.begin(9600);
  }

  void Step()
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
        if (sendingToModem_)
        {
          modem_->Write('\r');
          modem_->Write('\n');
          sendingToModem_ = false;
          continue;
        }

        if (pos_)
        {
          buf_[pos_] = 0;
          handler_->OnCommand(buf_);
          pos_ = 0;
        }
        
        continue;
      }

      if (sendingToModem_)
      {
          modem_->Write(data);
          continue;
      }

      if (pos_ < COM_BUF_LEN)
        buf_[pos_++] = data;

      if (pos_ == 2 && (buf_[0] == 'a' || buf_[0] == 'A') && (buf_[1] == 't' || buf_[1] == 'T'))
      {
        pos_ = 0;
        modem_->Write(buf_[0]);
        modem_->Write(buf_[1]);
        sendingToModem_ = true;
      }
    }
  }

private:
  Modem* modem_;
  CommandHandler* handler_;
  bool sendingToModem_;
  char buf_[COM_BUF_LEN + 1];
  uint8_t pos_;
};
#endif
