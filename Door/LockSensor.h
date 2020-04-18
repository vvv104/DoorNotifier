#pragma once

enum DoorState
{
  dsUnknown, dsOpen, dsLocked1, dsLocked2
};

class DoorHandler
{
public:
  virtual void OnStateChanged(DoorState state) = 0;
};

class LockSensor
{
public:
  LockSensor(DoorHandler* handler)
  : handler_(handler)
  , state_(dsUnknown)
  , last_(0)
  {
  }

  void Start()
  {
    last_ = millis();
  }

  void Step()
  {
#ifdef DEBUG
    return;
#endif
    const unsigned long current = millis();

    if (last_ + timeout_ > current)
      return;

    last_ = current;

    const int sensorValue = analogRead(pin_);
    LogVal("Hall Sensor = ", sensorValue);

    // < 517 - open           513-515
    // < 544 - 1 turn         517-520
    //         2 turns        544-547
    if (sensorValue < 512)
      SetState(dsUnknown);
    else if (sensorValue < 517)
      SetState(dsOpen);
    else if (sensorValue < 544)
      SetState(dsLocked1);
    else if (sensorValue < 548)
      SetState(dsLocked2);
    else
      SetState(dsUnknown);
  }

  void SetState(DoorState state)
  {
    if (state_ == state)
      return;

    state_ = state;
    handler_->OnStateChanged(state_);
  }

#ifdef DEBUG
  void Rotate(bool right)
  {
    if (right && state_ < dsLocked2)
      SetState((int)state_ + 1);
    else if (!right && state_ > dsUnknown)
      SetState((int)state_ - 1);
  }
#endif

private:
  DoorHandler* handler_;
  DoorState state_;
  unsigned long last_;
  static const uint8_t pin_;
  static const unsigned int timeout_;
};

static const uint8_t LockSensor::pin_ = A3;
static const unsigned int LockSensor::timeout_ = 1000;
