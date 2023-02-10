// Date: 10/FEB/2022
// Path: Fryer_App/Heater.cpp
// Link: https://github.com/mhdeeb/Fryer_App


#include "Encoder.h"
#include "Timer.h"
#include "Heater.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

// Custom characters for lcd
byte degree[] = {
	B00110,
	B01001,
	B00110,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000};

byte Cursor[] = {
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B11111};

byte NoCursor[] = {
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000};

// Standard Timers
u32 TIMER[] = {
	30,
	1 * 60,
	2 * 60,
	300,
	10 * 60,
	15 * 60};

Encoder encoder(9, 10, 11);

Counter selector_counter(0, 1, 1);
Counter editor_selector(0, 6, 1);
Counter temp_counter(0, 200, 1, false);

Timer timer1(TIMER[0], 1, 0, "T1", nullptr);
Timer timer2(TIMER[0], 1, 1, "T2", nullptr);

PushButton timer_switch(3, 3000);
PushButton temp_switch(4, 5000);
PushButton editor_switch(5, 5000);

note notes[]{
	{128, 500},
	{0, 500}};

Melody melody = {
	notes,
	2};

Alarm alarm(6);

enum Timers
{
	TIMER_1,
	TIMER_2
};

Timer *Timers[] = {
	&timer1,
	&timer2};

void setup()
{
	Serial.begin(9600);

	lcd.init();

	lcd.clear();
	lcd.createChar(0, degree);
	lcd.createChar(1, Cursor);
	lcd.createChar(2, NoCursor);

	pinMode(TEMP_SENSOR, INPUT);
	pinMode(HEATER, OUTPUT);
	pinMode(ALARM, OUTPUT);
	pinMode(6, OUTPUT);

	lcd.clear();

	lcd.setCursor(0, 1 - selector_counter.GetValue());
	lcd.print(' ');
	lcd.setCursor(0, selector_counter.GetValue());
	lcd.print('>');

	lcd.setCursor(11, 0);
	lcd.print("Temp");

	lcd.setCursor(11, 1);
	char string[14];
	sprintf(string, "%3lu", temp_counter.GetValue());
	lcd.printstr(string);

	lcd.setCursor(14, 1);
	lcd.write(0);

	lcd.setCursor(15, 1);
	lcd.print("C");

	timer1.SetLCD(&lcd);
	timer2.SetLCD(&lcd);
}

void loop()
{
	static bool editorMode = false;
	static bool onEditorMode = false;
	if (temp_switch.IsPressed() || onEditorMode)
	{
		if (temp_switch.IsToggled() || onEditorMode)
		{
			alarm.Start(&melody);
			lcd.backlight();
		}
		else
			lcd.noBacklight();
		onEditorMode = false;
	}

	if (editor_switch.IsHeld() && !editorMode)
	{
		editorMode = true;
		onEditorMode = true;
		editor_switch.Reset();
	}

	if (editor_switch.IsReleased() && editorMode)
	{
		editorMode = false;
	}

	if (temp_switch.IsToggled() || editor_switch.IsToggled())
	{
		static bool wasHeld = false;
		if (timer_switch.IsReleased())
		{
			if (!wasHeld)
			{
				encoder.GetButton().Reset();
				lcd.setCursor(0, selector_counter.GetValue());
				lcd.print(' ');
				lcd.setCursor(9, selector_counter.GetValue());
				lcd.print(' ');
				lcd.setCursor(0, selector_counter.GetValue());
				lcd.print('>');
				if (!Timers[selector_counter.GetValue()]->IsRunning())
					Timers[selector_counter.GetValue()]->Start();
			}
			else
				wasHeld = false;
		}

		if (timer_switch.IsHeld())
		{
			encoder.GetButton().Reset();
			Timers[selector_counter.GetValue()]->Reset();
			timer_switch.Reset();
			wasHeld = true;
		}

		if ((timer1.IsFinished() && !timer1.IsBlinking()) || (timer2.IsFinished() && !timer2.IsBlinking()))
		{
			alarm.Start(&melody, 8);
			if (timer1.IsFinished())
				timer1.StartBlinking(&melody, 8);
			if (timer2.IsFinished())
				timer2.StartBlinking(&melody, 8);
		}

		if (timer_switch.IsPressed() && Timers[selector_counter.GetValue()]->IsFinished())
		{
			alarm.Stop();
			Timers[selector_counter.GetValue()]->Reset();
			if (Timers[1 - selector_counter.GetValue()]->IsFinished())
				Timers[selector_counter.GetValue()]->Reset();
			wasHeld = true;
		}

		if (encoder.GetButton().IsPressed())
		{
			if (encoder.GetButton().IsToggled() && !Timers[selector_counter.GetValue()]->IsRunning())
			{
				lcd.setCursor(0, selector_counter.GetValue());
				lcd.print('<');
				lcd.setCursor(9, selector_counter.GetValue());
				lcd.print('>');
			}
			else
			{
				lcd.setCursor(0, selector_counter.GetValue());
				lcd.print(' ');
				lcd.setCursor(9, selector_counter.GetValue());
				lcd.print(' ');
				lcd.setCursor(0, selector_counter.GetValue());
				lcd.print('>');
			}
		}

		if (int8_t change = encoder.popRotationChange())
		{
			if (encoder.GetButton().IsToggled() && !Timers[selector_counter.GetValue()]->IsRunning())
			{
				static s8 selectedCounter = 0;
				if (change == 1)
					selectedCounter++;
				else
					selectedCounter--;

				if (selectedCounter < 0)
					selectedCounter = 5;
				else
					selectedCounter %= 6;

				Timers[selector_counter.GetValue()]->Set(TIMER[selectedCounter]);
			}
			else
			{
				if (change == 1)
					selector_counter.Increment();
				else
					selector_counter.Decrement();
				lcd.setCursor(0, 1 - selector_counter.GetValue());
				lcd.print(' ');
				lcd.setCursor(0, selector_counter.GetValue());
				lcd.print('>');
			}
		}

		timer_switch.Update();

		encoder.Update();

		timer1.Update();

		timer2.Update();
	}

	temp_switch.Update();

	editor_switch.Update();

	alarm.Update();
}