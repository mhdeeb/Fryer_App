// Date: 10/FEB/2022
// Path: Fryer_App/Heater.cpp
// Link: https://github.com/mhdeeb/Fryer_App

#include "Encoder.h"
#include "Timer.h"

// FIXME: hold to close temp if timer is running
// FIXME: Timer not working

#define ENCODER_PIN_A 9
#define ENCODER_PIN_B 10
#define ENCODER_PIN_BTN 11
#define TIMER_SWITCH_PIN 3
#define TEMP_SWITCH_PIN 4
#define EDITOR_SWITCH_PIN 5
#define ALARM_PIN 6
#define MAX_TEMP 100
#define MIN_TEMP 0
#define DEFAULT_TEMP 37
#define DEGREE 0
#define OLD_LEFT_CURSOR 1
#define LEFT_CURSOR 0x7F
#define UP_CURSOR 2
#define OLD_RIGHT_CURSOR 3
#define RIGHT_CURSOR 0x7E
#define DOWN_CURSOR 4

// Custom characters for lcd
// byte degree[] = {
// 	B00110,
// 	B01001,
// 	B00110,
// 	B00000,
// 	B00000,
// 	B00000,
// 	B00000,
// 	B00000};

byte degree[] = {
	B01000,
	B10100,
	B01000,
	B00011,
	B00100,
	B00100,
	B00100,
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
	1 * 60,
	2 * 60,
	300,
	10 * 60,
	15 * 60};

LiquidCrystal_I2C lcd(0x27, 20, 4);

Encoder encoder(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_PIN_BTN);

Timer Timers[]{
	Timer(times[0], 1, 0, "T1", nullptr),
	Timer(times[0], 1, 1, "T2", nullptr),
};

Counter selector_counter(0, sizeof(Timers) / sizeof(Timer) - 1);
Counter editor_selector(0, sizeof(times) / sizeof(u32));
Counter temp_counter(MIN_TEMP, MAX_TEMP, DEFAULT_TEMP, 1, false);

PushButton timer_switch(TIMER_SWITCH_PIN, 3000);
PushButton temp_switch(TEMP_SWITCH_PIN, 5000);
PushButton editor_switch(EDITOR_SWITCH_PIN, 3000);
u8 edit_column = 0;
bool editorMode = false;

Note long_note[]{
	{128, 400},
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

Alarm alarm(ALARM_PIN);

char string[14];
void setup()
{
	Serial.begin(9600);

	lcd.init();

	lcd.clear();
	lcd.createChar(DEGREE, degree);
	lcd.createChar(LEFT_CURSOR, cursor_left);
	lcd.createChar(UP_CURSOR, cursor_up);
	lcd.createChar(RIGHT_CURSOR, cursor_right);
	lcd.createChar(DOWN_CURSOR, cursor_down);

	lcd.clear();

	lcd.setCursor(0, 0);
	lcd.write(RIGHT_CURSOR);

	lcd.setCursor(12, 0);
	lcd.print("Temp");

	lcd.setCursor(12, 1);
	sprintf(string, "%3lu", temp_counter.GetValue());
	lcd.printstr(string);
	lcd.write(DEGREE);
	// lcd.print("C");

	for (Timer &timer : Timers)
		timer.SetLCD(&lcd);
}

void loop()
{
	if (!editorMode)
	{
		if (editor_switch.IsHeld() && !temp_switch.IsToggled())
		{
			editorMode = true;
			alarm.Start(&select_beep);
			lcd.clear();
			lcd.backlight();
			lcd.setCursor(0, 0);
			lcd.write(RIGHT_CURSOR);
			lcd.setCursor(11, 0);
			lcd.print(" Edit");
			lcd.setCursor(12, 1);
			lcd.print("Mode");
			lcd.setCursor(10, 0);
			lcd.write(UP_CURSOR);
			lcd.setCursor(10, 1);
			lcd.write(DOWN_CURSOR);
			sprintf(string, "T%d %02lu:%02lu", 1, times[0] / 60, times[0] % 60);
			lcd.setCursor(1, 0);
			lcd.printstr(string);
			sprintf(string, "T%d %02lu:%02lu", 2, times[1] / 60, times[1] % 60);
			lcd.setCursor(1, 1);
			lcd.printstr(string);

			editor_switch.Reset();

			encoder.SetCounter(&editor_selector);
			encoder.Reset();
		}
		else if (temp_switch.IsPressed())
		{
			if (temp_switch.IsToggled())
			{
				timer_switch.Reset();
				encoder.SetCounter(&selector_counter);
				encoder.Reset();

				for (Timer &timer : Timers)
					timer.Reset();

				alarm.Start(&select_beep);

				lcd.setCursor(0, 0);
				lcd.write(RIGHT_CURSOR);
				lcd.backlight();
			}
			else
				lcd.noBacklight();
		}
	}
	else if (temp_switch.IsHeld())
	{
	}
	else if (editor_switch.IsPressed())
	{
		editorMode = false;
		lcd.setCursor(0, 0);
		lcd.print("                ");
		lcd.setCursor(0, 1);
		lcd.print("                ");
		lcd.setCursor(5, 1);
		lcd.print("Saved");
		delay(1000);
		alarm.Start(&start_beep);
		lcd.noBacklight();
		lcd.setCursor(9, 0);
		lcd.print(' ');
		lcd.setCursor(9, 1);
		lcd.print(' ');
		lcd.setCursor(12, 0);
		lcd.print("Temp");
		lcd.setCursor(12, 1);
		sprintf(string, "%3lu", temp_counter.GetValue());
		lcd.printstr(string);
		lcd.write(DEGREE);
		// lcd.print("C");
		for (Timer &timer : Timers)
		{
			timer.Set(times[0]);
			timer.Update();
		}
	}

	if (editorMode || temp_switch.IsToggled())
	{
		int8_t change = encoder.popRotationChange();

		if (editorMode)
		{
			if (encoder.GetButton().IsPressed())
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

				alarm.Start(&select_beep);
			}

			if (change)
			{
				if (edit_column == 0)
				{
					if (editor_selector.GetValue() == 5)
					{
						lcd.setCursor(1, 1);
						sprintf(string, "Temp %3lu", temp_counter.GetValue());
						lcd.printstr(string);
						lcd.write(DEGREE);
					}
					else if (editor_selector.GetValue() == 6)
					{
						lcd.setCursor(1, 1);
						lcd.print("         ");
					}
					else
					{
						sprintf(string, "T%lu %02lu:%02lu", editor_selector.GetValue() + 2, times[editor_selector.GetValue() + 1] / 60, times[editor_selector.GetValue() + 1] % 60);
						lcd.setCursor(1, 1);
						lcd.printstr(string);
					}
				}
				else if (edit_column == 1)
				{
					if (editor_selector.GetValue() == 6)
					{
						if (change > 0)
							temp_counter.Increment();
						else
							temp_counter.Decrement();
					}
					else
					{
						if (change > 0 && times[editor_selector.GetValue()] / 60 != 99)
							times[editor_selector.GetValue()] += 60;
						else if (change < 0 && times[editor_selector.GetValue()] / 60 != 0)
							times[editor_selector.GetValue()] -= 60;
					}
				}
				else if (edit_column == 2)
				{
					if (editor_selector.GetValue() != 6)
					{
						if (change > 0 && times[editor_selector.GetValue()] % 60 != 59)
							times[editor_selector.GetValue()]++;
						else if (change < 0 && times[editor_selector.GetValue()] % 60 != 0)
							times[editor_selector.GetValue()]--;
					}
				}

				lcd.setCursor(1, 0);

				if (editor_selector.GetValue() == 6)
				{
					sprintf(string, "Temp %3lu", temp_counter.GetValue());
					lcd.printstr(string);
					lcd.write(DEGREE);
				}
				else
				{
					lcd.setCursor(9, 0);
					lcd.print(' ');
					lcd.setCursor(9, 1);
					lcd.print(' ');
					sprintf(string, "T%lu %02lu:%02lu", editor_selector.GetValue() + 1, times[editor_selector.GetValue()] / 60, times[editor_selector.GetValue()] % 60);
					lcd.setCursor(1, 0);
					lcd.printstr(string);
				}

				if (edit_column)
				{
					if (editor_selector.GetValue() == 6)
						lcd.setCursor(8, 0);
					else
						lcd.setCursor(3 * edit_column + 2, 0);
				}

				alarm.Start(&select_beep);
			}
		}
		else
		{
			// if (timer_switch.IsPressed() && !Timers[selector_counter.GetValue()].IsRunning()) // FIXME: Timer needs to beep when pressed why running
			// 	alarm.Start(&error_beep, 2);
			if (timer_switch.IsPressed())
			{
				lcd.setCursor(0, selector_counter.GetValue());
				lcd.print(' ');
				lcd.setCursor(9, selector_counter.GetValue());
				lcd.print(' ');
				lcd.setCursor(0, selector_counter.GetValue());
				lcd.write(RIGHT_CURSOR);

				if (!Timers[selector_counter.GetValue()].IsRunning())
					alarm.Start(&start_beep);

				if (!(Timers[selector_counter.GetValue()].IsRunning() || Timers[selector_counter.GetValue()].IsFinished()))
					Timers[selector_counter.GetValue()].Start();

				for (Timer &timer : Timers)
					if (timer.IsFinished())
						timer.Reset();
			}
			else if (timer_switch.IsHeld())
			{
				Timers[selector_counter.GetValue()].Reset();
				timer_switch.Reset();
				alarm.Start(&start_beep);
			}

			if (encoder.GetButton().IsPressed())
			{
				if (!Timers[selector_counter.GetValue()].IsRunning())
				{
					if (encoder.GetButton().IsToggled())
					{
						lcd.setCursor(0, selector_counter.GetValue());
						lcd.write(OLD_LEFT_CURSOR);
						lcd.setCursor(9, selector_counter.GetValue());
						lcd.write(OLD_RIGHT_CURSOR);
					}
					else
					{
						lcd.setCursor(0, selector_counter.GetValue());
						lcd.print(' ');
						lcd.setCursor(9, selector_counter.GetValue());
						lcd.print(' ');
						lcd.setCursor(0, selector_counter.GetValue());
						lcd.write(RIGHT_CURSOR);
					}
					alarm.Start(&select_beep);
				}
				else
				{
					encoder.Reset();
					alarm.Start(&error_beep, 2);
				}
			}

			if (change)
			{
				if (encoder.GetButton().IsToggled() && !Timers[selector_counter.GetValue()].IsRunning())
				{
					if (change == 1) // FIXME: This is a hack
						selector_counter.Decrement();
					else
						selector_counter.Increment();
					static s8 selectedCounter = 0;
					if (change == 1)
						selectedCounter++;
					else
						selectedCounter--;

					if (selectedCounter < 0)
						selectedCounter = 5;
					else
						selectedCounter %= 6;

					Timers[selector_counter.GetValue()].Set(times[selectedCounter]);
				}
				else
				{
					lcd.setCursor(0, 1 - selector_counter.GetValue());
					lcd.print(' ');
					lcd.setCursor(0, selector_counter.GetValue());
					lcd.write(RIGHT_CURSOR);
				}
				alarm.Start(&select_beep);
			}

			for (Timer &timer : Timers)
			{
				if ((timer.IsFinished() && !timer.IsBlinking()))
				{
					timer.StartBlinking(&alarm_beep, 8);
					alarm.Start(&alarm_beep, 8);
				}
				timer.Update();
			}

			timer_switch.Update();
		}
	}

	temp_switch.Update();

	editor_switch.Update();

	alarm.Update();

	if (edit_column == 0)
		encoder.UnlockCounter();
	else
		encoder.LockCounter();

	encoder.Update();
}