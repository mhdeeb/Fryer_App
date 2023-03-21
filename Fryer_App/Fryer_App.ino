// Date: 10/FEB/2022
// Path: Fryer_App/Heater.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#include "Encoder.h"
#include "SevSeg.h"
#include "Multiplexer.h"
#include <EEPROM.h>

u8 MULTIPLEXER_PINS[] = {7, 6, 5};
u8 BUTTONS_PINS[] = {0, 1, 2, A5, A4, 3};
u8 SEV_SEG_PINS[][2] = {{A1, A2}, {12, A0}};

#define ENCODER_PIN_A 9
#define ENCODER_PIN_B 10
#define ENCODER_PIN_BTN 11
#define ALARM_PIN 8
#define BUZZER_PIN A3

bool MULTIPLEXER_OUTPUT[]{LOW, LOW, LOW, LOW, LOW, LOW, LOW};

u32 Times[]{300, 300, 300, 300, 300};

Encoder encoder(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_PIN_BTN);

Note long_note[]{
	{255, 400},
	{0, 400}};

Note short_note[]{
	{200, 100},
	{0, 100}};

Note error_note[]{
	{200, 75},
	{0, 75}};

Melody alarm_beep{
	long_note,
	sizeof(long_note) / sizeof(Note)};

Melody select_beep{
	short_note,
	sizeof(short_note) / sizeof(Note)};

Melody start_beep{
	short_note,
	sizeof(short_note) / sizeof(Note)};

Melody error_beep{
	error_note,
	sizeof(error_note) / sizeof(Note)};

Buzzer sound(BUZZER_PIN);
Buzzer alarm(ALARM_PIN);

struct Timer_Module
{
private:
	Timer timer;

public:
	PushButton switcher;
	Buzzer *alarm = nullptr;
	Buzzer *buzzer = nullptr;

	Timer_Module(u32 initialValue, u8 switcherPin, u32 holdTime = 3000) : timer(initialValue), switcher(switcherPin, holdTime)
	{
	}

	void Update()
	{
		switcher.Update();

		timer.Update();

		if (switcher.Get(PushButtonInterface::PRESSED))
		{
			if (timer.IsFinished())
			{
				timer.Reset();

				if (alarm)
					alarm->Stop();

				switcher.Set(PushButtonInterface::RESETED);
			}
			else if (!timer.IsRunning())
				timer.Start();

			if (buzzer)
				buzzer->Play(&select_beep, 1);
		}
		else if (switcher.Get(PushButtonInterface::HELD))
		{
			timer.Reset();
			switcher.Set(PushButtonInterface::RESETED);
			if (buzzer)
				buzzer->Play(&select_beep, 1);
		}

		if (timer.IsFinished() && alarm)
		{
			alarm->Continue(&alarm_beep, -1);
		}
	}

	bool IsRunning() { return timer.IsRunning(); }

	Timer *GetTimer()
	{
		return &timer;
	}
};

Timer_Module Timers[]{
	Timer_Module(0, BUTTONS_PINS[0]),
	Timer_Module(0, BUTTONS_PINS[1]),
	Timer_Module(0, BUTTONS_PINS[2]),
	Timer_Module(0, BUTTONS_PINS[3]),
	Timer_Module(0, BUTTONS_PINS[4])};

PushButton edit_switch(BUTTONS_PINS[5], 3000);

Multiplexer multiplexer{MULTIPLEXER_PINS, sizeof(MULTIPLEXER_PINS) / sizeof(u8)};

SevSeg sevSeg1(SEV_SEG_PINS[0][0], SEV_SEG_PINS[0][1]);
SevSeg sevSeg2(SEV_SEG_PINS[1][0], SEV_SEG_PINS[1][1]);

u8 shownTimerIndex = 0;

enum class InterfaceMode
{
	RUN_MODE = 0,
	EDIT_MODE = 1
};
InterfaceMode mode = InterfaceMode::RUN_MODE;

void saveTimer(u8 index)
{
	for (u8 i = 0; i < 4; i++)
		EEPROM.write(index * 4 + i + 1, (Times[index] >> (24 - 8 * i)) & 0xFF);
}

void loadTimer(u8 index)
{
	Times[index] = 0;
	for (u8 i = 0; i < 4; i++)
		Times[index] = (Times[index] << 8) | EEPROM.read(index * 4 + i + 1);
}

void setup()
{
	Serial.begin(9600);

	if (EEPROM.read(0) == 0x55)
	{
		for (u8 i = 0; i < sizeof(Times) / sizeof(*Times); i++)
			loadTimer(i);
	}
	else
	{
		EEPROM.write(0, 0x55);
		for (u8 i = 0; i < sizeof(Times) / sizeof(*Times); i++)
			saveTimer(i);
	}

	int i = 0;
	for (auto &timer : Timers)
	{
		timer.GetTimer()->Set(Times[i]);
		timer.alarm = &alarm;
		timer.buzzer = &sound;
		i++;
	}

	sevSeg1.Set(Timers[0].GetTimer());
	sevSeg2.Set(Timers[1].GetTimer());

	multiplexer.Set(MULTIPLEXER_OUTPUT);
}

bool blinker = false;
u32 lastBlink = millis();
u8 selectTimeIndex = 0;
bool editMinutes = true;
Timer editTimer(0);
Counter timeCounter[]{{0, 99}, {0, 59}};

void loop()
{
	if (millis() - lastBlink > 500)
	{
		blinker = !blinker;
		lastBlink = millis();
	}

	if (mode == InterfaceMode::RUN_MODE)
	{
		for (int i = 0; i < sizeof(Timers) / sizeof(Timer_Module); i++)
		{
			Timers[i].Update();

			if (Timers[i].switcher.Get(PushButtonInterface::PRESSED))
			{
				if (shownTimerIndex)
				{
					sevSeg2.Set(Timers[i].GetTimer());
				}
				else
				{
					sevSeg1.Set(Timers[i].GetTimer());
				}

				shownTimerIndex = !shownTimerIndex;
			}

			if (Timers[i].IsRunning())
				multiplexer.Set(i, true);
			else if (Timers[i].GetTimer()->IsFinished())
				multiplexer.Set(i, blinker);
			else
				multiplexer.Set(i, false);
		}
	}
	else if (mode == InterfaceMode::EDIT_MODE)
	{
		encoder.Update();

		for (int i = 0; i < sizeof(Timers) / sizeof(Timer_Module); i++)
		{
			PushButton &button = Timers[i].switcher;

			button.Update();

			if (button.Get(PushButtonInterface::PRESSED))
			{
				for (int i = 0; i < sizeof(Timers) / sizeof(Timer_Module); i++)
					multiplexer.Set(i, false);

				multiplexer.Set(i, true);

				selectTimeIndex = i;

				editTimer.Set(Times[selectTimeIndex]);

				timeCounter[0].SetValue(Times[selectTimeIndex] / 60);
				timeCounter[1].SetValue(Times[selectTimeIndex] % 60);

				sound.Play(&select_beep, 1);
			}
		}

		if (encoder.GetButton().Get(PushButtonInterface::PRESSED))
		{
			encoder.SetCounter(&timeCounter[editMinutes = !editMinutes]);

			sound.Play(&select_beep, 1);
		}

		if (s8 change = encoder.popRotationChange())
		{
			Times[selectTimeIndex] = timeCounter[0].GetValue() * 60 + timeCounter[1].GetValue();

			editTimer.Set(Times[selectTimeIndex]);

			sound.Play(&select_beep, 1);
		}
	}

	if (edit_switch.Get(PushButtonInterface::PRESSED))
	{

		sound.Play(&select_beep, 1);

		if (edit_switch.Get(PushButtonInterface::TOGGLED))
		{
			mode = InterfaceMode::EDIT_MODE;
			sevSeg1.Set(&editTimer);
			sevSeg2.GetDisplay()->setBrightness(0x05, false);
			multiplexer.Set(5, true);
			encoder.SetCounter(&timeCounter[editMinutes = false]);
			for (auto &timer : Timers)
				timer.GetTimer()->Reset();
		}
		else
		{
			mode = InterfaceMode::RUN_MODE;
			sevSeg1.Set(Timers[0].GetTimer());
			sevSeg2.Set(Timers[0].GetTimer());
			sevSeg2.GetDisplay()->setBrightness(0x05, true);
			multiplexer.Set(5, false);

			int i = 0;
			for (auto &timer : Timers)
			{
				timer.GetTimer()->Set(Times[i]);
				i++;
			}

			for (u8 i = 0; i < sizeof(Times) / sizeof(*Times); i++)
				saveTimer(i);
		}
	}

	alarm.Update();

	sound.Update();

	multiplexer.Update();

	sevSeg1.Update();
	sevSeg2.Update();

	edit_switch.Update();
}