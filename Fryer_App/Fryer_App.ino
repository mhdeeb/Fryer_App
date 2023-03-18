// Date: 10/FEB/2022
// Path: Fryer_App/Heater.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#include "Encoder.h"
#include "SevSeg.h"
#include "Multiplexer.h"
#include <EEPROM.h>

u8 MULTIPLEXER_PINS[] = {8, 10, 2};
u8 BUTTONS_PINS[] = {3, 4, 5, 6, 7, 9};
u8 SEV_SEG_PINS[][2] = {{0, 0}, {0, 0}};

#define ENCODER_PIN_A 13
#define ENCODER_PIN_B 11
#define ENCODER_PIN_BTN 12
#define ALARM_PIN 6
#define BUZZER_PIN 7

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
	bool isRunning = false;
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

		if (switcher.Get(PushButtonInterface::PRESSED))
		{
			if (!isRunning)
			{
				timer.Start();
				isRunning = true;
			}

			if (buzzer != nullptr)
				buzzer->Play(&select_beep, 1);

			if (timer.IsFinished())
			{
				timer.Reset();
				if (alarm != nullptr)
					alarm->Stop();
			}
		}
		else if (switcher.Get(PushButtonInterface::HELD))
		{
			timer.Reset();
			isRunning = false;
			switcher.Set(PushButtonInterface::RESETED);
			if (buzzer != nullptr)
				buzzer->Play(&select_beep, 1);
		}

		timer.Update();

		if (timer.IsFinished() && isRunning)
		{
			if (alarm != nullptr)
				alarm->Play(&alarm_beep, -1);
			isRunning = false;
		}
	}

	bool IsRunning() const { return isRunning; }

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

u8 showntimers[] = {0, 1};
u8 shownTimerIndex = 0;

u8 edit_column = 0;

enum class InterfaceMode
{
	RUN_MODE = 0,
	EDIT_MODE = 1
};
InterfaceMode mode = InterfaceMode::RUN_MODE;

void saveTimer(u8 index)
{
	for (u8 i = 0; i < 4; i++)
		EEPROM.write(index * 4 + i + 1, (Timers[index].GetTimer()->GetMax() >> (24 - 8 * i)) & 0xFF);
}

void loadTimer(u8 index)
{
	Timers[index].GetTimer()->Set(0);
	for (u8 i = 0; i < 4; i++)
		Timers[index].GetTimer()->Set((Timers[index].GetTimer()->GetTime() << 8) | EEPROM.read(index * 4 + i + 1));
}

void setup()
{
	Serial.begin(9600);

	if (EEPROM.read(0) == 0x55)
	{
		for (u8 i = 0; i < sizeof(Timers) / sizeof(Timer); i++)
			loadTimer(i);
	}
	else
	{
		EEPROM.write(0, 0x55);
		Timers[0].GetTimer()->Set(30);
		Timers[1].GetTimer()->Set(60);
		Timers[2].GetTimer()->Set(300);
		Timers[3].GetTimer()->Set(900);
		Timers[4].GetTimer()->Set(1800);
		for (u8 i = 0; i < sizeof(Timers) / sizeof(Timer); i++)
			saveTimer(i);
	}

	for (auto &timer : Timers)
	{
		timer.alarm = &alarm;
		timer.buzzer = &sound;
	}

	sevSeg1.Set(Timers[showntimers[0]].GetTimer());
	sevSeg2.Set(Timers[showntimers[1]].GetTimer());

	sevSeg1.GetDisplay()->setBrightness(7);
	sevSeg2.GetDisplay()->setBrightness(7);
}

void loop()
{
	// if (mode == InterfaceMode::OFF_MODE)
	// {
	// 	if (editor_switch.Get(PushButtonInterface::HELD))
	// 	{
	// 		mode = InterfaceMode::EDIT_MODE;
	// 		sound.Play(&select_beep);
	// 		lcd.clear();
	// 		lcd.backlight();
	// 		lcd.setCursor(0, 0);
	// 		lcd.write(CURSOR_RIGHT);
	// 		lcd.setCursor(11, 0);
	// 		lcd.print(" Edit");
	// 		lcd.setCursor(12, 1);
	// 		lcd.print("Mode");
	// 		lcd.setCursor(10, 0);
	// 		lcd.write(CURSOR_UP);
	// 		lcd.setCursor(10, 1);
	// 		lcd.write(CURSOR_DOWN);
	// 		sprintf(string, "T%d %02lu:%02lu", 1, times[0] / 60, times[0] % 60);
	// 		lcd.setCursor(1, 0);
	// 		lcd.printstr(string);
	// 		sprintf(string, "T%d %02lu:%02lu", 2, times[1] / 60, times[1] % 60);
	// 		lcd.setCursor(1, 1);
	// 		lcd.printstr(string);

	// 		editor_switch.Set(PushButtonInterface::RESETED);

	// 		encoder.SetCounter(&editor_selector);
	// 		encoder.Reset();
	// 	}
	// 	else if (temp_switch.Get(PushButtonInterface::PRESSED))
	// 	{
	// 		mode = InterfaceMode::RUN_MODE;
	// 		timer_switch.Reset();
	// 		encoder.SetCounter(&timer_selector);
	// 		encoder.Reset();

	// 		for (Timer &timer : Timers)
	// 			timer.Reset();

	// 		sound.Play(&select_beep);

	// 		lcd.setCursor(0, 0);
	// 		lcd.write(CURSOR_RIGHT);
	// 		lcd.backlight();
	// 	}
	// }
	// else
	// {
	// 	s8 change = encoder.popRotationChange();

	// 	if (mode == InterfaceMode::EDIT_MODE)
	// 	{
	// 		if (editor_switch.Get(PushButtonInterface::PRESSED))
	// 		{
	// 			mode = InterfaceMode::OFF_MODE;
	// 			lcd.setCursor(0, 0);
	// 			lcd.print("                ");
	// 			lcd.setCursor(0, 1);
	// 			lcd.print("                ");
	// 			lcd.setCursor(5, 1);
	// 			lcd.print("Saved");
	// 			delay(1000);
	// 			sound.Play(&start_beep);
	// 			lcd.noBacklight();
	// 			lcd.setCursor(9, 0);
	// 			lcd.print(' ');
	// 			lcd.setCursor(9, 1);
	// 			lcd.print(' ');
	// 			lcd.setCursor(12, 0);
	// 			lcd.print("Temp");
	// 			lcd.setCursor(12, 1);
	// 			sprintf(string, "%3d", heater.GetTemp());
	// 			lcd.printstr(string);
	// 			lcd.write(DEGREE);
	// 			for (Timer &timer : Timers)
	// 			{
	// 				timer.Set(times[0]);
	// 				timer.Update();
	// 			}
	// 		}

	// 		if (encoder.GetButton().Get(PushButtonInterface::PRESSED))
	// 		{
	// 			edit_column++;

	// 			if (editor_selector.GetValue() == 6)
	// 			{
	// 				edit_column %= 2;

	// 				if (edit_column)
	// 				{
	// 					lcd.cursor();
	// 					lcd.setCursor(8, 0);
	// 				}
	// 				else
	// 					lcd.noCursor();
	// 			}
	// 			else
	// 			{
	// 				edit_column %= 3;

	// 				if (edit_column)
	// 				{
	// 					lcd.cursor();
	// 					lcd.setCursor(3 * edit_column + 2, 0);
	// 				}
	// 				else
	// 					lcd.noCursor();
	// 			}

	// 			minute_selector.SetValue(times[editor_selector.GetValue()] / 60);
	// 			second_selector.SetValue(times[editor_selector.GetValue()] % 60);

	// 			if (edit_column == 0)
	// 				encoder.SetCounter(&editor_selector);
	// 			else if (edit_column == 1)
	// 			{
	// 				if (editor_selector.GetValue() == 6)
	// 					encoder.SetCounter(&heater.GetTargetTempCounter());
	// 				else
	// 					encoder.SetCounter(&minute_selector);
	// 			}
	// 			else if (edit_column == 2)
	// 				encoder.SetCounter(&second_selector);

	// 			sound.Play(&select_beep);
	// 		}

	// 		if (change)
	// 		{
	// 			lcd.setCursor(1, 0);

	// 			if (editor_selector.GetValue() == 6)
	// 			{
	// 				sprintf(string, "Temp %3lu", heater.GetTargetTempCounter().GetValue());
	// 				lcd.printstr(string);
	// 				lcd.write(DEGREE);
	// 				lcd.setCursor(1, 1);
	// 				lcd.print("         ");
	// 			}
	// 			else
	// 			{
	// 				if (edit_column)
	// 				{
	// 					times[editor_selector.GetValue()] = minute_selector.GetValue() * 60 + second_selector.GetValue();
	// 					saveTimer(editor_selector.GetValue());
	// 				}

	// 				lcd.setCursor(9, 0);
	// 				lcd.print(' ');
	// 				lcd.setCursor(9, 1);
	// 				lcd.print(' ');
	// 				sprintf(string, "T%lu %02lu:%02lu", editor_selector.GetValue() + 1, times[editor_selector.GetValue()] / 60, times[editor_selector.GetValue()] % 60);
	// 				lcd.setCursor(1, 0);
	// 				lcd.printstr(string);
	// 				if (editor_selector.GetValue() == 5)
	// 				{
	// 					sprintf(string, "Temp %3lu", heater.GetTargetTempCounter().GetValue());
	// 					lcd.setCursor(1, 1);
	// 					lcd.printstr(string);
	// 					lcd.write(DEGREE);
	// 				}
	// 				else
	// 				{
	// 					sprintf(string, "T%lu %02lu:%02lu", editor_selector.GetValue() + 2, times[editor_selector.GetValue() + 1] / 60, times[editor_selector.GetValue() + 1] % 60);
	// 					lcd.setCursor(1, 1);
	// 					lcd.printstr(string);
	// 				}
	// 			}

	// 			if (edit_column)
	// 			{
	// 				if (editor_selector.GetValue() == 6)
	// 					lcd.setCursor(8, 0);
	// 				else
	// 					lcd.setCursor(3 * edit_column + 2, 0);
	// 			}

	// 			sound.Play(&select_beep);
	// 		}
	// 	}
	// 	else if (mode == InterfaceMode::RUN_MODE)
	// 	{
	// 		bool isAnyTimerRunning = false;
	// 		for (Timer &timer : Timers)
	// 		{
	// 			if ((timer.IsFinished() && !timer.IsBlinking()))
	// 			{
	// 				timer.StartBlinking(&alarm_beep, -1);
	// 				alarm.Play(&alarm_beep, -1);
	// 			}
	// 			else if (timer.IsRunning())
	// 				isAnyTimerRunning = true;
	// 			timer.Update();
	// 		}

	// 		if (temp_switch.Get(PushButtonInterface::PRESSED) && !isAnyTimerRunning)
	// 		{
	// 			mode = InterfaceMode::OFF_MODE;
	// 			lcd.noBacklight();
	// 		}
	// 		else if (temp_switch.Get(PushButtonInterface::RELEASED) && isAnyTimerRunning)
	// 		{
	// 			sound.Play(&error_beep, 2);
	// 		}
	// 		else if (temp_switch.Get(PushButtonInterface::HELD) && isAnyTimerRunning)
	// 		{
	// 			mode = InterfaceMode::OFF_MODE;
	// 			lcd.noBacklight();
	// 		}
	// 		else if (timer_switch.Get(PushButtonInterface::PRESSED))
	// 		{
	// 			lcd.setCursor(0, timer_selector.GetValue());
	// 			lcd.print(' ');
	// 			lcd.setCursor(9, timer_selector.GetValue());
	// 			lcd.print(' ');
	// 			lcd.setCursor(0, timer_selector.GetValue());
	// 			lcd.write(CURSOR_RIGHT);

	// 			bool isAnyFinished = false;
	// 			for (Timer &timer : Timers)
	// 				if (timer.IsFinished())
	// 				{
	// 					isAnyFinished = true;
	// 					timer.Reset();
	// 				}

	// 			if (!Timers[timer_selector.GetValue()].IsRunning())
	// 			{
	// 				sound.Play(&start_beep);
	// 				if (!isAnyFinished)
	// 					Timers[timer_selector.GetValue()].Start();
	// 				timer_switch.Set(PushButtonInterface::RESETED);
	// 			}
	// 		}
	// 		else if (timer_switch.Get(PushButtonInterface::HELD))
	// 		{
	// 			Timers[timer_selector.GetValue()].Reset();
	// 			timer_switch.Set(PushButtonInterface::RESETED);
	// 			sound.Play(&start_beep);
	// 		}
	// 		else if (timer_switch.Get(PushButtonInterface::RELEASED) && Timers[timer_selector.GetValue()].IsRunning())
	// 		{
	// 			sound.Play(&error_beep, 2);
	// 		}
	// 		else if (encoder.GetButton().Get(PushButtonInterface::PRESSED))
	// 		{
	// 			if (!Timers[timer_selector.GetValue()].IsRunning())
	// 			{
	// 				if (encoder.GetButton().Get(PushButtonInterface::TOGGLED))
	// 				{
	// 					lcd.setCursor(0, timer_selector.GetValue());
	// 					lcd.write(OLD_CURSOR_LEFT);
	// 					lcd.setCursor(9, timer_selector.GetValue());
	// 					lcd.write(OLD_CURSOR_RIGHT);
	// 					if (timer_selector.GetValue())
	// 						encoder.SetCounter(&time2_selector);
	// 					else
	// 						encoder.SetCounter(&time1_selector);
	// 				}
	// 				else
	// 				{
	// 					lcd.setCursor(0, timer_selector.GetValue());
	// 					lcd.print(' ');
	// 					lcd.setCursor(9, timer_selector.GetValue());
	// 					lcd.print(' ');
	// 					lcd.setCursor(0, timer_selector.GetValue());
	// 					lcd.write(CURSOR_RIGHT);
	// 					encoder.SetCounter(&timer_selector);
	// 				}
	// 				sound.Play(&select_beep);
	// 				alarm.Stop();
	// 			}
	// 			else
	// 			{
	// 				encoder.GetButton().Reset(PushButtonInterface::TOGGLED);
	// 				sound.Play(&error_beep, 2);
	// 			}
	// 		}

	// 		if (change)
	// 		{
	// 			if (encoder.GetButton().Get(PushButtonInterface::TOGGLED))
	// 			{
	// 				lcd.setCursor(0, timer_selector.GetValue());
	// 				lcd.write(OLD_CURSOR_LEFT);
	// 				lcd.setCursor(9, timer_selector.GetValue());
	// 				lcd.write(OLD_CURSOR_RIGHT);
	// 				if (timer_selector.GetValue())
	// 					Timers[1].Set(times[time2_selector.GetValue()]);
	// 				else
	// 					Timers[0].Set(times[time1_selector.GetValue()]);
	// 			}
	// 			else
	// 			{
	// 				lcd.setCursor(0, 1 - timer_selector.GetValue());
	// 				lcd.print(' ');
	// 				lcd.setCursor(0, timer_selector.GetValue());
	// 				lcd.write(CURSOR_RIGHT);
	// 			}

	// 			for (Timer &timer : Timers)
	// 				if (timer.IsFinished())
	// 					timer.Reset();

	// 			sound.Play(&select_beep);
	// 			alarm.Stop();
	// 		}

	// 		lcd.setCursor(12, 1);
	// 		sprintf(string, "%3d", heater.GetTemp());
	// 		lcd.printstr(string);

	// 		timer_switch.Update();

	// 		heater.Update();
	// 	}
	// }

	if (mode == InterfaceMode::RUN_MODE)
	{
		for (auto &timer : Timers)
		{
			timer.Update();
			if (timer.switcher.Get(PushButtonInterface::PRESSED))
			{
				if (shownTimerIndex)
				{
					sevSeg1.Set(timer.GetTimer());
				}
				else
				{
					sevSeg2.Set(timer.GetTimer());
				}

				shownTimerIndex = !shownTimerIndex;
			}
		}
	}
	else if (mode == InterfaceMode::EDIT_MODE)
	{
		encoder.Update();
		for (auto &timer : Timers)
		{
			timer.GetTimer()->Update();
			timer.switcher.Update();
			if (timer.switcher.Get(PushButtonInterface::PRESSED))
			{
			}
		}
	}

	if (edit_switch.Get(PushButtonInterface::PRESSED))
	{
		if (edit_switch.Get(PushButtonInterface::TOGGLED))
		{
			mode = InterfaceMode::EDIT_MODE;
			sevSeg2.GetDisplay()->setBrightness(0, false);
		}
		else
		{
			mode = InterfaceMode::RUN_MODE;
			sevSeg2.GetDisplay()->setBrightness(7, false);
		}
	}

	alarm.Update();

	sound.Update();

	multiplexer.Update();
}