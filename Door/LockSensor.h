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
  , last_(0)
  {
  }

  void Start()
  {
    last_ = millis();
  }

  void Step()
  {
    const unsigned long current = millis();

    if (last_ + timeout_ > current)
      return;

    last_ = current;

    // < 517 - open           513-515
    // < 544 - 1 turn         517-520
    //         2 turns        544-547
    const int sensorValue = analogRead(pin_);
    Serial.print("Hall Sensor = " );
    Serial.println(sensorValue);
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
  unsigned long last_;
  static const uint8_t pin_;
  static const unsigned int timeout_;
};

static const uint8_t LockSensor::pin_ = A3;
static const unsigned int LockSensor::timeout_ = 1000;
