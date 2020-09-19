#pragma once

#include "PulseHandler.h"

class Pulse
{
public:
	Pulse(PulseHandler* handler)
		: handler_(handler)
		, last_(0)
		, step_(0)
		, count_(0)
		, timeOn_(0)
		, timeOff_(0)
	{
	}

	void Start(unsigned int count = 0, unsigned int timeOn = 0, unsigned int timeOff = 0)
	{
		step_ = 0;
		count_ = count * (timeOff ? 2 : 1);
		timeOn_ = timeOn;
		timeOff_ = timeOff;
		last_ = millis();
		handler_->Init();
	}

	void Step()
	{
		if (!timeOn_)
			return;

		const unsigned int timeout = timeOff_ && (step_ % 2) ? timeOff_ : timeOn_;
		const unsigned long current = millis();

		if (last_ + timeout > current)
			return;

		last_ = current;

		++step_;

		if (count_ && (step_ == count_))
		{
			Stop();
			return;
		}

		handler_->Action(step_);
	}

	void Stop()
	{
		timeOn_ = 0;
		handler_->Finish();
	}

private:
	PulseHandler* handler_;
	unsigned long last_;
	unsigned int step_;
	unsigned int count_;
	unsigned int timeOn_;
	unsigned int timeOff_;
};
