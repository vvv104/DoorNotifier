#pragma once

class DoorHandler
{
public:
  virtual void OnOpen() = 0;
  virtual void OnClose() = 0;
};

class LockSensor
{
public:
  LockSensor(DoorHandler* handler)
  : handler_(handler)
  {
  }

private:
  DoorHandler* handler_;
};
