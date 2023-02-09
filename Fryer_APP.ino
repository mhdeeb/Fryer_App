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

byte obracket[] = {
	B11100,
	B10000,
	B10000,
	B10000,
	B10000,
	B10000,
	B10000,
	B11100};

byte cbracket[] = {
	B00111,
	B00001,
	B00001,
	B00001,
	B00001,
	B00001,
	B00001,
	B00111};

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
s32 TIMER[] = {
	0.5 * 60,
	1 * 60,
	3.5 * 60,
	6 * 60,
	12 * 60,
	16 * 60};

Encoder encoder(9, 10, 11);

Counter selector_counter(0, 2, 1);
Timer timer1(TIMER[0]);
Timer timer2(TIMER[0]);
// Counter timer1_counter(0, 5, 1);
// Counter timer2_counter(0, 5, 1);
// Counter temp_counter(0, 100, 1, false);

PushButton timer_switch(13, 2000);
// Counter_Init(&timer1_counter, 0, 24 * 60 - 1, 1);

enum counter_type
{
	TIMER_1,
	TIMER_2,
	TEMP,
	SELECTOR,
	COUNTER_SIZE
};

// int selectedCounter = SELECTOR;

// Counter *counters[COUNTER_SIZE] = {
// 	&timer1_counter,
// 	&timer2_counter,
// 	&temp_counter,
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
	lcd.createChar(3, obracket);
	lcd.createChar(4, cbracket);

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

	char time_string[6];
	lcd.setCursor(4, 0);
	// sprintf(time_string, "%02d:%02d", TIMER[timer1_counter.GetValue()] / 60, TIMER[timer1_counter.GetValue()] % 60);
	lcd.printstr(time_string);

	lcd.setCursor(11, 0);
	lcd.print("Temp");

	lcd.setCursor(1, 1);
	lcd.print("T2");

	lcd.setCursor(4, 1);
	// sprintf(time_string, "%02d:%02d", TIMER[timer2_counter.GetValue()] / 60, TIMER[timer2_counter.GetValue()] % 60);
	lcd.printstr(time_string);

	char temp_string[4];
	lcd.setCursor(12, 1);
	// sprintf(temp_string, "%3d", temp_counter.GetValue());
	lcd.printstr(temp_string);

	lcd.setCursor(14, 1);
	lcd.write(0);

	lcd.setCursor(15, 1);
	lcd.print("C");
}

void loop()
{
	if (timer_switch.IsPressed())
	{
		if (!timer1.IsRunning())
			timer1.Start();
		else
			timer1.Stop();
	}

	if (timer_switch.IsToggled())
	{
		Serial.println(timer1.GetTime());
	}

	if (timer_switch.IsHeld())
	{
		Serial.println(timer1.IsRunning());
		timer1.Reset();
		timer2.Reset();
		timer_switch.Reset();
	}

	if (encoder.GetButton().IsPressed())
	{
		// selectedCounter = selector_counter.GetValue() + SELECTOR - selectedCounter;

		// if (selectedCounter == SELECTOR)
		// {
		// 	lcd.setCursor(0, selector_counter.GetValue());
		// 	lcd.print(' ');
		// 	lcd.setCursor(9, selector_counter.GetValue());
		// 	lcd.print(' ');
		// 	lcd.setCursor(0, selector_counter.GetValue());
		// 	lcd.print('>');
		// }
		// else
		// {
		// 	lcd.setCursor(0, selector_counter.GetValue());
		// 	lcd.write(3);
		// 	lcd.setCursor(9, selector_counter.GetValue());
		// 	lcd.write(4);
		// }
	}

	// if (int8_t change = encoder.popRotationChange())
	// {
	// 	if (change == 1)
	// 		counters[selectedCounter]->Increment();
	// 	else
	// 		counters[selectedCounter]->Decrement();
	// 	char time_string[6];
	// 	char temp_string[4];
	// 	switch (selectedCounter)
	// 	{
	// 	case SELECTOR:
	// 		lcd.setCursor(0, 1 - selector_counter.GetValue());
	// 		lcd.print(' ');
	// 		lcd.setCursor(0, selector_counter.GetValue());
	// 		lcd.print('>');
	// 		break;
	// 	case TIMER_1:
	// 		lcd.setCursor(4, 0);
	// 		sprintf(time_string, "%02d:%02d", TIMER[timer1_counter.GetValue()] / 60, TIMER[timer1_counter.GetValue()] % 60);
	// 		lcd.printstr(time_string);
	// 		break;
	// 	case TIMER_2:
	// 		lcd.setCursor(4, 1);
	// 		sprintf(time_string, "%02d:%02d", TIMER[timer2_counter.GetValue()] / 60, TIMER[timer2_counter.GetValue()] % 60);
	// 		lcd.printstr(time_string);
	// 		break;
	// 	case TEMP:
	// 		lcd.setCursor(13, 1);
	// 		sprintf(temp_string, "%3d", temp_counter.GetValue());
	// 		lcd.printstr(temp_string);
	// 		break;
	// 	}
	// }

	timer_switch.Update();

	encoder.Update();

	timer1.Update();

	timer2.Update();
}