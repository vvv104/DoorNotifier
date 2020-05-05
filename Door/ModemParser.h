#pragma once

class ModemHandler
{
public:
  virtual void CallSuccess() = 0;
  virtual void CallFailure() = 0;
};

class ModemCommand
{
public:
  virtual void Command(const char* cmd) = 0;
};

class ModemParser
{
public:
  ModemParser(ModemHandler* handler, ModemCommand* command)
  : handler_(handler)
  , command_(command)
  {
  }

  void OnReceive(const String& data)
  {
    LogVal("Modem: ", data);
  }

private:
  ModemHandler* handler_;
  ModemCommand* command_;
};
