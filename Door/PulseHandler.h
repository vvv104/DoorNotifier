#pragma once

class PulseHandler
{
public:
  virtual void Init() = 0;
  virtual void Action() = 0;
  virtual void Finish() = 0;
};
