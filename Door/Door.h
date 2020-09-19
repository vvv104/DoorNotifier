#pragma once

#include "Led.h"
#include "Beeper.h"
#include "Pulse.h"
#include "Modem.h"
#include "LockSensor.h"
#include "CommandProcessor.h"
#include "Numbers.h"
#include "Time.h"

class Door : public DoorHandler, public PulseHandler, public ModemHandler
#ifdef DEBUG
	, public CommandHandler
#endif
{
public:
	Door()
		: r(&red), g(&green), b(&blue)
		, beep(&beeper)
		, lock(this)
		, timer(this)
		, modem(this)
#ifdef DEBUG
		, command(&modem, this)
#endif
	{
	}

	void Start()
	{
#ifdef DEBUG
		command.Start();
#endif
		modem.Start();
		lock.Start();
		b.Start();
		beeper(7040).Beep(100);
	}

	void Step()
	{
		lock.Step();
		modem.Step();
		timer.Step();
		r.Step();
		g.Step();
		b.Step();
		beep.Step();
#ifdef DEBUG
		command.Step();
#endif
	}

#ifdef DEBUG
	// CommandHandler interface
	void OnCommand(const char* data)
	{
		if (strcmp(data, "+") == 0)
			lock.Rotate(true);
		else if (strcmp(data, "-") == 0)
			lock.Rotate(false);
	}
#endif

	// DoorHandler interface
	void OnStateChanged(DoorState state)
	{
		LogVal("Door state: ", state)

			timer.Stop();
		r.Stop();
		g.Stop();
		b.Stop();
		beep.Stop();
		modem.CancelCall();

		switch (state)
		{
		case dsOpen:
			r.Start(0, 500, 500);
			beeper(2637).Beep(250);
			timer.Start(5, 1 * secs);
			break;
		case dsLocked1:
			g.Start(0, 500, 500);
			beeper(2093).Beep(250);
			timer.Start(2, 10 * secs);
			break;
		case dsLocked2:
			g.Start(0, 100, 4900);
			beeper(1760).Beep(250);
			break;
		case dsUnknown:
		default:
			r.Start();
			beeper(3520).Beep(250);
			timer.Start(5, 1 * secs);
			break;
		}
	}

	// PulseHandler interface

	virtual void Init()
	{
		Log("door->init");
	}

	virtual void Action(unsigned int step)
	{
		LogVal("door->action", step);

		if (step == 1)
			beep.Start(0, 500, 500);

		//    if (step >= 2)
		//      modem.Call(numbers[0]);
	}

	virtual void Finish()
	{
		Log("door->finish");
	}

	// ModemHandler interface

	virtual void CallSuccess()
	{
		timer.Stop();
	}

	virtual void CallFailure()
	{

	}

private:
	Pulse r, g, b;
	Pulse beep;
	Pulse timer;
	Modem modem;
	LockSensor lock;
#ifdef DEBUG
	CommandProcessor command;
#endif
};
