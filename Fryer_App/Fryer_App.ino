// Date: 10/FEB/2022
// Path: Fryer_App/Heater.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#include "lib/SevSeg.h"
#include "lib/Buzzer.h"

#include <EEPROM.h>
#include <Keypad.h>

const u32 COMMAND_TIME = 5000;

const String EDIT_COMMAND = "19119";

const byte rows = 4;
const byte cols = 4;

char keys[rows][cols] = {
	{'0', '1', '2', '3'},
	{'4', '5', '6', '7'},
	{'8', '9', 'L', 'R'},
	{'E', '_', '_', '_'}};

byte rowPins[rows] = {2, 3, 4, 5};
byte colPins[cols] = {6, 7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

u8 SEV_SEG_PINS[][2] = {{12, A0}, {A1, A2}};

#define ALARM_PIN 8
#define BUZZER_PIN A3

#define TIMER_NUMBER 10

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

Timer Timers[TIMER_NUMBER];

SevSeg sevSeg1(SEV_SEG_PINS[0][0], SEV_SEG_PINS[0][1]);
SevSeg sevSeg2(SEV_SEG_PINS[1][0], SEV_SEG_PINS[1][1]);

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

	for (u8 i = 0; i < TIMER_NUMBER; i++)
		Timers[i].Set(loadTime(i));

	sevSeg1.Set(&Timers[0]);
	sevSeg2.Set(&Timers[1]);
}

void loop()
{
	u32 command_time;
	char key = keypad.getKey();

	if (key != NO_KEY)
	{
		sound.Play(&select_beep, 1);
	}

	if (mode == InterfaceMode::RUN_MODE)
	{
		if (key == 'E')
		{
			mode = InterfaceMode::COMMAND_MODE;
			command_time = millis();
		}
		else if (key == 'L')
			;
		else if (key == 'R')
			;
		else
		{
		}
	}
	else if (mode == InterfaceMode::EDIT_MODE)
	{
		if (key == 'E')
		{
			mode = InterfaceMode::RUN_MODE;
		}
		else if (isdigit(key))
		{
			static bool numberSelected = false;
			static u8 index{};
			static u8 numberIndex{};

			if (numberSelected)
			{
				u8 number = key - '0';

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
				index = key - '0';
				numberIndex = 0;

				sevSeg1.Set(&Timers[index]);
			}
		}
	}
	else if (mode == InterfaceMode::COMMAND_MODE)
	{
		static String command;

		if (key == 'E' || millis() - command_time >= COMMAND_TIME)
		{
			mode = InterfaceMode::RUN_MODE;
			sound.Play(&error_beep, 1);
			command = "";
		}
		else if (isdigit(key))
		{
			command += key;

			if (command.length() == 5)
			{
				if (command.equals(EDIT_COMMAND))
				{
					mode = InterfaceMode::EDIT_MODE;
				}
				else
				{
					mode = InterfaceMode::RUN_MODE;
					sound.Play(&error_beep, 1);
				}

				command = "";
			}
		}
	}

	alarm.Update();

	sound.Update();

	for (auto &timer : Timers)
		timer.Update();

	sevSeg1.Update();
	sevSeg2.Update();
}