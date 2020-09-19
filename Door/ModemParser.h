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
		, isCalling(false)
	{
	}

	void OnReceive(const char* data)
	{
		LogVal("Modem: ", data);
		if (data[0] == '+')
		{
			UnsolicRC(data + 1);
			return;
		}
	}

	bool IsCalling() const
	{
		return isCalling;
	}

	void Calling(bool val)
	{
		isCalling = val;
	}

private:
	void UnsolicRC(const char* rc)
	{

	}

	ModemHandler* handler_;
	ModemCommand* command_;
	bool isCalling;
};
