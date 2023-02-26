// Date: 10/FEB/2022
// Path: Fryer_App/Heater.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#include "Encoder.h"
#include "Timer.h"
#include "Heater.h"

#define ENCODER_PIN_A 11
#define ENCODER_PIN_B 12
#define ENCODER_PIN_BTN 13
#define TIMER_SWITCH_PIN 3
#define TEMP_SWITCH_PIN 4
#define EDITOR_SWITCH_PIN 5
#define ALARM_PIN 6
#define BUZZER_PIN 7
#define TEMP_SENSOR_PIN A0
#define HEATER_PIN 9

#define MAX_TEMP 100
#define MIN_TEMP 0
#define DEFAULT_TEMP 37

#define DEGREE 0
#define OLD_CURSOR_LEFT 1
#define CURSOR_LEFT 0x7F
#define CURSOR_UP 2
#define OLD_CURSOR_RIGHT 3
#define CURSOR_RIGHT 0x7E
#define CURSOR_DOWN 4

// Custom characters for lcd
byte degree[] = {
	B01000,
	B10100,
	B01000,
	B00011,
	B00100,
	B00100,
	B00100,
	B00011};

byte old_cursor_left[] = {
	B00011,
	B00110,
	B01100,
	B11000,
	B11000,
	B01100,
	B00110,
	B00011};

byte cursor_left[] = {
	B00011,
	B00110,
	B01100,
	B11000,
	B11000,
	B01100,
	B00110,
	B00011};

byte cursor_up[] = {
	B00000,
	B00100,
	B01110,
	B11111,
	B01110,
	B01110,
	B01110,
	B00000};

byte old_cursor_right[] = {
	B11000,
	B01100,
	B00110,
	B00011,
	B00011,
	B00110,
	B01100,
	B11000};

byte cursor_right[] = {
	B11000,
	B01100,
	B00110,
	B00011,
	B00011,
	B00110,
	B01100,
	B11000};

byte cursor_down[] = {
	B00000,
	B01110,
	B01110,
	B01110,
	B11111,
	B01110,
	B00100,
	B00000};

// Standard Timers
u32 times[] = {
	30,
	60,
	300,
	900,
	1800,
	3600};

LiquidCrystal_I2C lcd(0x27, 20, 4);

Encoder encoder(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_PIN_BTN);

Timer Timers[]{
	Timer(times[0], 1, 0, "T1", nullptr),
	Timer(times[0], 1, 1, "T2", nullptr),
};

Counter timer_selector(0, sizeof(Timers) / sizeof(Timer) - 1);
Counter time1_selector(0, sizeof(times) / sizeof(u32) - 1);
Counter time2_selector(0, sizeof(times) / sizeof(u32) - 1);
Counter minute_selector(0, 99);
Counter second_selector(0, 59);
Counter editor_selector(0, sizeof(times) / sizeof(u32));

PushButton timer_switch(TIMER_SWITCH_PIN, 3000);
PushButton temp_switch(TEMP_SWITCH_PIN, 3000);
PushButton editor_switch(EDITOR_SWITCH_PIN, 3000);

Heater heater(HEATER_PIN, TEMP_SENSOR_PIN, DEFAULT_TEMP, MIN_TEMP, MAX_TEMP);

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

u8 edit_column = 0;

enum class InterfaceMode
{
	OFF_MODE = 0,
	EDIT_MODE = 1,
	RUN_MODE = 2,
};
InterfaceMode mode = InterfaceMode::OFF_MODE;

char string[14];
void setup()
{
	Serial.begin(9600);

	lcd.init();

	lcd.clear();
	lcd.createChar(DEGREE, degree);
	lcd.createChar(CURSOR_LEFT, cursor_left);
	lcd.createChar(OLD_CURSOR_LEFT, old_cursor_left);
	lcd.createChar(CURSOR_UP, cursor_up);
	lcd.createChar(CURSOR_RIGHT, cursor_right);
	lcd.createChar(OLD_CURSOR_RIGHT, old_cursor_right);
	lcd.createChar(CURSOR_DOWN, cursor_down);

	lcd.clear();

	lcd.setCursor(0, 0);
	lcd.write(CURSOR_RIGHT);

	lcd.setCursor(12, 0);
	lcd.print("Temp");

	lcd.setCursor(12, 1);
	sprintf(string, "%3d", heater.GetTemp());
	lcd.printstr(string);
	lcd.write(DEGREE);

	for (Timer &timer : Timers)
		timer.SetLCD(&lcd);
}

void loop()
{
	if (mode == InterfaceMode::OFF_MODE)
	{
		if (editor_switch.Get(PushButtonInterface::HELD))
		{
			mode = InterfaceMode::EDIT_MODE;
			sound.Play(&select_beep);
			lcd.clear();
			lcd.backlight();
			lcd.setCursor(0, 0);
			lcd.write(CURSOR_RIGHT);
			lcd.setCursor(11, 0);
			lcd.print(" Edit");
			lcd.setCursor(12, 1);
			lcd.print("Mode");
			lcd.setCursor(10, 0);
			lcd.write(CURSOR_UP);
			lcd.setCursor(10, 1);
			lcd.write(CURSOR_DOWN);
			sprintf(string, "T%d %02lu:%02lu", 1, times[0] / 60, times[0] % 60);
			lcd.setCursor(1, 0);
			lcd.printstr(string);
			sprintf(string, "T%d %02lu:%02lu", 2, times[1] / 60, times[1] % 60);
			lcd.setCursor(1, 1);
			lcd.printstr(string);

			editor_switch.Set(PushButtonInterface::RESETED);

			encoder.SetCounter(&editor_selector);
			encoder.Reset();
		}
		else if (temp_switch.Get(PushButtonInterface::PRESSED))
		{
			mode = InterfaceMode::RUN_MODE;
			timer_switch.Reset();
			encoder.SetCounter(&timer_selector);
			encoder.Reset();

			for (Timer &timer : Timers)
				timer.Reset();

			sound.Play(&select_beep);

			lcd.setCursor(0, 0);
			lcd.write(CURSOR_RIGHT);
			lcd.backlight();
		}
	}
	else
	{
		s8 change = encoder.popRotationChange();

		if (mode == InterfaceMode::EDIT_MODE)
		{
			if (editor_switch.Get(PushButtonInterface::PRESSED))
			{
				mode = InterfaceMode::OFF_MODE;
				lcd.setCursor(0, 0);
				lcd.print("                ");
				lcd.setCursor(0, 1);
				lcd.print("                ");
				lcd.setCursor(5, 1);
				lcd.print("Saved");
				delay(1000);
				sound.Play(&start_beep);
				lcd.noBacklight();
				lcd.setCursor(9, 0);
				lcd.print(' ');
				lcd.setCursor(9, 1);
				lcd.print(' ');
				lcd.setCursor(12, 0);
				lcd.print("Temp");
				lcd.setCursor(12, 1);
				sprintf(string, "%3d", heater.GetTemp());
				lcd.printstr(string);
				lcd.write(DEGREE);
				for (Timer &timer : Timers)
				{
					timer.Set(times[0]);
					timer.Update();
				}
			}

			if (encoder.GetButton().Get(PushButtonInterface::PRESSED))
			{
				edit_column++;

				if (editor_selector.GetValue() == 6)
				{
					edit_column %= 2;

					if (edit_column)
					{
						lcd.cursor();
						lcd.setCursor(8, 0);
					}
					else
						lcd.noCursor();
				}
				else
				{
					edit_column %= 3;

					if (edit_column)
					{
						lcd.cursor();
						lcd.setCursor(3 * edit_column + 2, 0);
					}
					else
						lcd.noCursor();
				}

				minute_selector.SetValue(times[editor_selector.GetValue()] / 60);
				second_selector.SetValue(times[editor_selector.GetValue()] % 60);

				if (edit_column == 0)
					encoder.SetCounter(&editor_selector);
				else if (edit_column == 1)
				{
					if (editor_selector.GetValue() == 6)
						encoder.SetCounter(&heater.GetTargetTempCounter());
					else
						encoder.SetCounter(&minute_selector);
				}
				else if (edit_column == 2)
					encoder.SetCounter(&second_selector);

				sound.Play(&select_beep);
			}

			if (change)
			{
				lcd.setCursor(1, 0);

				if (editor_selector.GetValue() == 6)
				{
					sprintf(string, "Temp %3lu", heater.GetTargetTempCounter().GetValue());
					lcd.printstr(string);
					lcd.write(DEGREE);
					lcd.setCursor(1, 1);
					lcd.print("         ");
				}
				else
				{
					if (edit_column)
						times[editor_selector.GetValue()] = minute_selector.GetValue() * 60 + second_selector.GetValue();

					lcd.setCursor(9, 0);
					lcd.print(' ');
					lcd.setCursor(9, 1);
					lcd.print(' ');
					sprintf(string, "T%lu %02lu:%02lu", editor_selector.GetValue() + 1, times[editor_selector.GetValue()] / 60, times[editor_selector.GetValue()] % 60);
					lcd.setCursor(1, 0);
					lcd.printstr(string);
					if (editor_selector.GetValue() == 5)
					{
						sprintf(string, "Temp %3lu", heater.GetTargetTempCounter().GetValue());
						lcd.setCursor(1, 1);
						lcd.printstr(string);
						lcd.write(DEGREE);
					}
					else
					{
						sprintf(string, "T%lu %02lu:%02lu", editor_selector.GetValue() + 2, times[editor_selector.GetValue() + 1] / 60, times[editor_selector.GetValue() + 1] % 60);
						lcd.setCursor(1, 1);
						lcd.printstr(string);
					}
				}

				if (edit_column)
				{
					if (editor_selector.GetValue() == 6)
						lcd.setCursor(8, 0);
					else
						lcd.setCursor(3 * edit_column + 2, 0);
				}

				sound.Play(&select_beep);
			}
		}
		else if (mode == InterfaceMode::RUN_MODE)
		{
			bool isAnyTimerRunning = false;
			for (Timer &timer : Timers)
			{
				if ((timer.IsFinished() && !timer.IsBlinking()))
				{
					timer.StartBlinking(&alarm_beep, -1);
					alarm.Play(&alarm_beep, -1);
				}
				else if (timer.IsRunning())
					isAnyTimerRunning = true;
				timer.Update();
			}

			if (temp_switch.Get(PushButtonInterface::PRESSED) && !isAnyTimerRunning)
			{
				mode = InterfaceMode::OFF_MODE;
				lcd.noBacklight();
			}
			else if (temp_switch.Get(PushButtonInterface::RELEASED) && isAnyTimerRunning)
			{
				sound.Play(&error_beep, 2);
			}
			else if (temp_switch.Get(PushButtonInterface::HELD) && isAnyTimerRunning)
			{
				mode = InterfaceMode::OFF_MODE;
				lcd.noBacklight();
			}
			else if (timer_switch.Get(PushButtonInterface::PRESSED))
			{
				lcd.setCursor(0, timer_selector.GetValue());
				lcd.print(' ');
				lcd.setCursor(9, timer_selector.GetValue());
				lcd.print(' ');
				lcd.setCursor(0, timer_selector.GetValue());
				lcd.write(CURSOR_RIGHT);

				bool isAnyFinished = false;
				for (Timer &timer : Timers)
					if (timer.IsFinished())
					{
						isAnyFinished = true;
						timer.Reset();
					}

				if (!Timers[timer_selector.GetValue()].IsRunning())
				{
					sound.Play(&start_beep);
					if (!isAnyFinished)
						Timers[timer_selector.GetValue()].Start();
					timer_switch.Set(PushButtonInterface::RESETED);
				}
			}
			else if (timer_switch.Get(PushButtonInterface::HELD))
			{
				Timers[timer_selector.GetValue()].Reset();
				timer_switch.Set(PushButtonInterface::RESETED);
				sound.Play(&start_beep);
			}
			else if (timer_switch.Get(PushButtonInterface::RELEASED) && Timers[timer_selector.GetValue()].IsRunning())
			{
				sound.Play(&error_beep, 2);
			}
			else if (encoder.GetButton().Get(PushButtonInterface::PRESSED))
			{
				if (!Timers[timer_selector.GetValue()].IsRunning())
				{
					if (encoder.GetButton().Get(PushButtonInterface::TOGGLED))
					{
						lcd.setCursor(0, timer_selector.GetValue());
						lcd.write(OLD_CURSOR_LEFT);
						lcd.setCursor(9, timer_selector.GetValue());
						lcd.write(OLD_CURSOR_RIGHT);
						if (timer_selector.GetValue())
							encoder.SetCounter(&time2_selector);
						else
							encoder.SetCounter(&time1_selector);
					}
					else
					{
						lcd.setCursor(0, timer_selector.GetValue());
						lcd.print(' ');
						lcd.setCursor(9, timer_selector.GetValue());
						lcd.print(' ');
						lcd.setCursor(0, timer_selector.GetValue());
						lcd.write(CURSOR_RIGHT);
						encoder.SetCounter(&timer_selector);
					}
					sound.Play(&select_beep);
					alarm.Stop();
				}
				else
				{
					encoder.GetButton().Reset(PushButtonInterface::TOGGLED);
					sound.Play(&error_beep, 2);
				}
			}

			if (change)
			{
				if (encoder.GetButton().Get(PushButtonInterface::TOGGLED))
				{
					lcd.setCursor(0, timer_selector.GetValue());
					lcd.write(OLD_CURSOR_LEFT);
					lcd.setCursor(9, timer_selector.GetValue());
					lcd.write(OLD_CURSOR_RIGHT);
					if (timer_selector.GetValue())
						Timers[1].Set(times[time2_selector.GetValue()]);
					else
						Timers[0].Set(times[time1_selector.GetValue()]);
				}
				else
				{
					lcd.setCursor(0, 1 - timer_selector.GetValue());
					lcd.print(' ');
					lcd.setCursor(0, timer_selector.GetValue());
					lcd.write(CURSOR_RIGHT);
				}

				for (Timer &timer : Timers)
					if (timer.IsFinished())
						timer.Reset();

				sound.Play(&select_beep);
				alarm.Stop();
			}

			lcd.setCursor(12, 1);
			sprintf(string, "%3d", heater.GetTemp());
			lcd.printstr(string);

			timer_switch.Update();

			heater.Update();
		}
	}

	temp_switch.Update();

	editor_switch.Update();

	sound.Update();

	encoder.Update();
}