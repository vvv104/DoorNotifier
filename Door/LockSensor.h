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
  , isOpen_(false)
  {
  }

  void Open()
  {
    isOpen_ = true;
    handler_->OnOpen();
  }

  void Close()
  {
    isOpen_ = false;
    handler_->OnClose();
  }

  bool IsOpen() const
  {
    return isOpen_;
  }

private:
  DoorHandler* handler_;
  bool isOpen_;
};
