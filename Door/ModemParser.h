#pragma once

class ModemHandler
{
public:
  virtual void CallSuccess() = 0;
  virtual void CallFailure() = 0;
};

class ModemParser
{
public:
  ModemParser(ModemHandler* handler)
  : handler_(handler)
  {
  }

  void OnReceive(const String& data)
  {
    LogVal("Modem: ", data);
  }

private:
  ModemHandler* handler_;
};
