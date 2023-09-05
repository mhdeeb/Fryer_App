// Date: 10/FEB/2022
// Path: Fryer_App/Heater.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#include "SevSeg.h"
#include "Buzzer.h"
#include "SerialToParallel.h"

#include <EEPROM.h>
#include <Keypad.h>

const u32 COMMAND_TIME = 6000;

const String EDIT_COMMAND = "19119";

const byte rows = 4;
const byte cols = 4;

byte rowPins[rows] = {A2, A1, A0, 13};
byte colPins[cols] = {12, 11, 10, 9};

char keys[rows][cols] = {
	{'L', '0', '1', '2'},
	{'3', '4', '5', '6'},
	{'7', '8', '9', 'R'},
	{'E', '_', '_', '_'}};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

#define ALARM_PIN 8
#define BUZZER_PIN 7
#define SERIAL_TO_PARALLEL_LATCH_PIN A4
#define SERIAL_TO_PARALLEL_CLOCK_PIN A5
#define SERIAL_TO_PARALLEL_DATA_PIN A3
#define SEV_SEG_1_CLK_PIN 3
#define SEV_SEG_1_DIO_PIN 4
#define SEV_SEG_2_CLK_PIN 5
#define SEV_SEG_2_DIO_PIN 6

#define TIMER_COUNT 10

SerialToParallel serialToParallel(SERIAL_TO_PARALLEL_LATCH_PIN, SERIAL_TO_PARALLEL_CLOCK_PIN, SERIAL_TO_PARALLEL_DATA_PIN);

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

Timer Timers[TIMER_COUNT];

SevSeg sevSeg1(SEV_SEG_1_CLK_PIN, SEV_SEG_1_DIO_PIN);
SevSeg sevSeg2(SEV_SEG_2_CLK_PIN, SEV_SEG_2_DIO_PIN);

enum class InterfaceMode
{
	RUN_MODE = 0,
	EDIT_MODE = 1,
	COMMAND_MODE = 2,
};

InterfaceMode mode = InterfaceMode::RUN_MODE;

void timeToByte(u32 time, u8 *bytes)
{
	for (u8 i = 0; i < 4; i++)
		bytes[i] = (time >> (24 - 8 * i)) & 0xFF;
}

u32 byteToTime(u8 *bytes)
{
	return (u32)bytes[0] * 600 + (u32)bytes[1] * 60 + (u32)bytes[2] * 10 + (u32)bytes[3];
}

void saveTime(u8 index, u32 time)
{
	u8 bytes[4];

	timeToByte(time, bytes);

	saveTimeBytes(index, bytes);
}

void saveTimeBytes(u8 index, u8 *time)
{
	for (u8 i = 0; i < 4; i++)
		EEPROM.write(index * 4 + i, time[i]);
}

void loadTimeBytes(u8 index, u8 *time)
{
	for (u8 i = 0; i < 4; i++)
		time[i] = EEPROM.read(index * 4 + i);
}

u32 loadTime(u8 index)
{
	u8 bytes[4];

	loadTimeBytes(index, bytes);

	return byteToTime(bytes);
}

void setup()
{
	Serial.begin(9600);

	for (u8 i = 0; i < TIMER_COUNT; i++)
		Timers[i].Set(loadTime(i));

	sevSeg1.Set(&Timers[0]);
	sevSeg2.Set(&Timers[1]);

	keypad.setHoldTime(1000);
}

#define ISKEYPRESSED (isKeyEvent && keypad.key[0].stateChanged && keypad.key[0].kstate == PRESSED)

#define ISKEYHELD (isKeyEvent && keypad.key[0].stateChanged && keypad.key[0].kstate == HOLD)

void loop()
{
	static u32 command_time;
	bool isKeyEvent = keypad.getKeys();

	if (ISKEYPRESSED)
	{
		sound.Play(&select_beep, 1);
	}

	if (mode == InterfaceMode::RUN_MODE)
	{
		if (ISKEYPRESSED && keypad.key[0].kchar == 'E')
		{
			mode = InterfaceMode::COMMAND_MODE;
			command_time = millis();
			serialToParallel.TurnOn(10);
		}
		else if (ISKEYPRESSED && isDigit(keypad.key[0].kchar))
		{
			u8 index = keypad.key[0].kchar - '0';

			if (Timers[index].IsFinished())
			{
				Timers[index].Reset();
				serialToParallel.stopBlinking(index);
			}
			else
			{
				sevSeg2.Set(sevSeg1.Get());
				sevSeg1.Set(&Timers[index]);
			}
		}

		else if (ISKEYHELD && isDigit(keypad.key[0].kchar))
		{
			u8 index = keypad.key[0].kchar - '0';

			if (Timers[index].IsRunning())
			{
				Timers[index].Reset();
				serialToParallel.TurnOff(index);
			}
			else
			{
				Timers[index].Start();
				serialToParallel.TurnOn(index);
			}
		}

		bool isAnyFinsihsed = false;
		for (int index = 0; index < TIMER_COUNT; index++)
		{
			Timers[index].Update();

			if (Timers[index].IsFinished())
			{
				isAnyFinsihsed = true;
				if (!alarm.IsOn())
				{
					alarm.Play(&alarm_beep, -1);
				}

				if (!serialToParallel.isBlinking(index))
				{
					serialToParallel.startBlinking(index);
				}
			}
		}
		if (!isAnyFinsihsed)
		{
			alarm.Stop();
		}
	}
	else if (mode == InterfaceMode::EDIT_MODE)
	{
		if (ISKEYPRESSED && keypad.key[0].kchar == 'E')
		{
			mode = InterfaceMode::RUN_MODE;
			serialToParallel.Set(0);
		}
		else if (ISKEYPRESSED && isDigit(keypad.key[0].kchar))
		{
			static bool numberSelected = false;
			static u8 index{};
			static u8 numberIndex{};

			if (numberSelected)
			{
				u8 number = keypad.key[0].kchar - '0';

				u8 timeBytes[4];

				loadTimeBytes(index, timeBytes);

				timeBytes[numberIndex++] = number;

				Timers[index].Set(byteToTime(timeBytes));

				saveTimeBytes(index, timeBytes);

				if (numberIndex == 4)
				{
					numberSelected = false;
				}
			}
			else
			{
				numberSelected = true;
				index = keypad.key[0].kchar - '0';
				numberIndex = 0;

				sevSeg1.Set(&Timers[index]);

				serialToParallel.Set(0);
				serialToParallel.TurnOn(10);
				serialToParallel.TurnOn(index);
			}
		}
	}
	else if (mode == InterfaceMode::COMMAND_MODE)
	{
		static String command;

		if ((ISKEYPRESSED && keypad.key[0].kchar == 'E') || millis() - command_time >= COMMAND_TIME)
		{
			mode = InterfaceMode::RUN_MODE;
			serialToParallel.Set(0);
			sound.Play(&error_beep, 1);
			command = "";
		}
		else if (ISKEYPRESSED && isdigit(keypad.key[0].kchar))
		{
			command += keypad.key[0].kchar;

			if (command.length() == 5)
			{
				if (command.equals(EDIT_COMMAND))
				{
					mode = InterfaceMode::EDIT_MODE;
					for (auto &timer : Timers)
						timer.Reset();
				}
				else
				{
					mode = InterfaceMode::RUN_MODE;
					serialToParallel.Set(0);
					sound.Play(&error_beep, 1);
				}

				command = "";
			}
		}
	}

	alarm.Update();

	sound.Update();

	sevSeg1.Update();
	sevSeg2.Update();

	serialToParallel.Update();
}