/*****************************************/
/*****************************************/
/****** By  : Abdelrahman Mohamed ********/
/****** Date:     7/DEC/2022      ********/
/****** SWC : FRYER_APPLICATION   ********/
/****** VER :        1.00         ********/
/*****************************************/
/*****************************************/
#include <LiquidCrystal_I2C.h>

#include "Encoder.h"
#include "Timer.h"
#include "Heater.h"

#define REFRESH_TIME 1000

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

// byte obracket[] = {
// 	B11100,
// 	B10000,
// 	B10000,
// 	B10000,
// 	B10000,
// 	B10000,
// 	B10000,
// 	B11100};

// byte cbracket[] = {
// 	B00111,
// 	B00001,
// 	B00001,
// 	B00001,
// 	B00001,
// 	B00001,
// 	B00001,
// 	B00111};

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
	210,
	10 * 60,
	15 * 60};

Encoder encoder(9, 10, 11);

Counter selector_counter(0, 1, 1);
Counter temp_counter(0, 200, 1, false);

Timer timer1(TIMER[0]);
Timer timer2(TIMER[0]);

PushButton timer_switch(3, 3000);

enum Timers
{
	TIMER_1,
	TIMER_2
};

Timer *Timers[] = {
	&timer1,
	&timer2};

// Counter timer1_counter(0, 5, 1);
// Counter timer2_counter(0, 5, 1);

// enum Counters
// {
// 	TIMER_1_COUNTER,
// 	TIMER_2_COUNTER,
// 	SELECTOR,
// 	COUNTER_SIZE
// };

// Counter *counters[COUNTER_SIZE] = {
// 	&timer1_counter,
// 	&timer2_counter,
// 	&selector_counter};

void setup()
{
	Serial.begin(9600);

	lcd.init();

	lcd.backlight();

	lcd.clear();
	lcd.createChar(0, degree);
	lcd.createChar(1, Cursor);
	lcd.createChar(2, NoCursor);
	// lcd.createChar(3, obracket);
	// lcd.createChar(4, cbracket);

	pinMode(TEMP_SENSOR, INPUT);
	pinMode(HEATER, OUTPUT);
	pinMode(ALARM, OUTPUT);

	lcd.clear();

	lcd.setCursor(0, 1 - selector_counter.GetValue());
	lcd.print(' ');
	lcd.setCursor(0, selector_counter.GetValue());
	lcd.print('>');

	lcd.setCursor(1, 0);
	lcd.print("T1");

	char string[14];
	lcd.setCursor(4, 0);
	sprintf(string, "%02lu:%02lu", timer1.GetTime() / 60, timer1.GetTime() % 60);
	lcd.printstr(string);

	lcd.setCursor(11, 0);
	lcd.print("Temp");

	lcd.setCursor(1, 1);
	lcd.print("T2");

	lcd.setCursor(4, 1);
	sprintf(string, "%02lu:%02lu", timer2.GetTime() / 60, timer2.GetTime() % 60);
	lcd.printstr(string);

	lcd.setCursor(11, 1);
	sprintf(string, "%3lu", temp_counter.GetValue());
	lcd.printstr(string);

	lcd.setCursor(14, 1);
	lcd.write(0);

	lcd.setCursor(15, 1);
	lcd.print("C");
}

void loop()
{
	if (timer_switch.IsReleased())
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
		// else
		// 	Timers[selector_counter.GetValue()]->Stop();
	}

	if (timer_switch.IsHeld())
	{
		encoder.GetButton().Reset();
		Timers[selector_counter.GetValue()]->Reset();
		timer_switch.Reset();
	}

	static u32 last_time = millis();
	if (millis() - last_time >= REFRESH_TIME)
	{
		static bool blinker = false;
		if (timer1.IsRunning() || timer2.IsRunning())
			blinker = !blinker;
		else
			blinker = true;
		char string[14];
		lcd.setCursor(4, 0);
		sprintf(string, "%02lu%c%02lu", timer1.GetTime() / 60, timer1.IsRunning() ? (blinker ? ':' : ' ') : ':', timer1.GetTime() % 60);
		lcd.printstr(string);

		lcd.setCursor(4, 1);
		sprintf(string, "%02lu%c%02lu", timer2.GetTime() / 60, timer2.IsRunning() ? (blinker ? ':' : ' ') : ':', timer2.GetTime() % 60);
		lcd.printstr(string);

		lcd.setCursor(11, 1);
		sprintf(string, "%3lu", temp_counter.GetValue());
		lcd.printstr(string);

		last_time = millis();
	}

	if (encoder.GetButton().IsPressed())
	{
		// selectedTimer = selector_counter.GetValue() + SELECTOR - selectedCounter;

		if (encoder.GetButton().IsToggled() && !Timers[selector_counter.GetValue()]->IsRunning())
		{
			lcd.setCursor(0, selector_counter.GetValue());
			// lcd.write(3);
			lcd.print('<');
			lcd.setCursor(9, selector_counter.GetValue());
			// lcd.write(4);
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
		if (encoder.GetButton().IsToggled())
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

			char string[14];
			lcd.setCursor(4, selector_counter.GetValue());
			sprintf(string, "%02lu:%02lu", Timers[selector_counter.GetValue()]->GetTime() / 60, Timers[selector_counter.GetValue()]->GetTime() % 60);
			lcd.printstr(string);
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
		// if (change == 1)
		// 	counters[selectedCounter]->Increment();
		// else
		// 	counters[selectedCounter]->Decrement();
		// char string[14];
		// if (selectedCounter == SELECTOR)
		// {
		// 	lcd.setCursor(0, 1 - selector_counter.GetValue());
		// 	lcd.print(' ');
		// 	lcd.setCursor(0, selector_counter.GetValue());
		// 	lcd.print('>');
		// }
		// else
		// {
		// 	counters[selectedCounter]->SetValue(TIMER[timer1_counter.GetValue()]);
		// 	sprintf(string, "%02d:%02d", counters[selectedCounter]->GetValue() / 60, counters[selectedCounter]->GetValue() % 60);
		// 	lcd.setCursor(4, selectedCounter);
		// 	lcd.printstr(string);
		// }

		// case TEMP:
		// 	lcd.setCursor(13, 1);
		// 	sprintf(temp_string, "%3d", temp_counter.GetValue());
		// 	lcd.printstr(temp_string);
		// 	break;
	}

	timer_switch.Update();

	encoder.Update();

	timer1.Update();

	timer2.Update();
}