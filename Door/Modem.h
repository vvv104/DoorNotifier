#pragma once

#include <SoftwareSerial.h>
#include "ModemParser.h"

#define MODEM_RX_PIN 8
#define MODEM_TX_PIN 9
#define MDM_BUF_LEN 127

class Modem : public ModemCommand
{
public:
	Modem(ModemHandler* handler)
		: parser_(handler, this)
		, modem_(MODEM_RX_PIN, MODEM_TX_PIN)
		, pos_(0)
	{
		buf_[MDM_BUF_LEN] = 0;
	}

	void Start()
	{
		modem_.begin(9600);
		Command("ATE0V0+CMEE=1;+CLCC=1;+CLTS=1;&W");
	}

	void Step()
	{
		int data;
		int len = modem_.available();

		if (len + pos_ > MDM_BUF_LEN)
			len = MDM_BUF_LEN - pos_;

		for (int i = 0; i < len; ++i)
		{
			data = modem_.read();

			if (data == -1)
				break;

			if (data == '\r' || data == '\n' || data == 0)
			{
				if (pos_)
				{
					buf_[pos_] = 0;
					parser_.OnReceive(buf_);
					pos_ = 0;
				}

				continue;
			}

			buf_[pos_++] = data;
		}

		if (pos_ == MDM_BUF_LEN)
		{
			parser_.OnReceive(buf_);
			pos_ = 0;
		}
	}

	void Call(const char* number)
	{
		if (parser_.IsCalling())
		{
			Log("Already calling");
			return;
		}

		LogVal("Calling: ", number);
		parser_.Calling(true);

		char buf[17]; // ATD+70123456789;\0
		strcpy(buf, "ATD+7");
		strncat(buf, number, 10);
		strcat(buf, ";");
		Command(buf);
	}

	void CancelCall()
	{
		if (!parser_.IsCalling())
		{
			Log("Not calling");
			return;
		}

		Command("ATH");
		parser_.Calling(false);
	}

	// ModemCommand interface
	virtual void Command(const char* cmd)
	{
		modem_.println(cmd);
	}

#ifdef DEBUG
	void Write(uint8_t b)
	{
		modem_.write(b);
	}
#endif

private:
	ModemParser parser_;
	SoftwareSerial modem_;
	char buf_[MDM_BUF_LEN + 1];
	uint8_t pos_;
};
