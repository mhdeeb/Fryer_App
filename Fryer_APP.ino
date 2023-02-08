/*****************************************/
/*****************************************/
/****** By  : Abdelrahman Mohamed ********/
/****** Date:     7/DEC/2022      ********/
/****** SWC : FRYER_APPLICATION   ********/
/****** VER :        1.00         ********/
/*****************************************/
/*****************************************/
#include <Wire.h> /*This library help to communicate with I2C device*/
#include <LiquidCrystal_I2C.h>

#include "Encoder_Interface.h"
#include "PushButton_Interface.h"
#include "Heater_Interface.h"

#define DEBOUNCE_DELAY 50

LiquidCrystal_I2C lcd(0x27, 20, 4);

// Encoder parameters

u8 A_State, A_LastState, SW, Real_counter;
u8 Rpointer = 0, Cpointer = 0;
u32 SW_counter = 0;
u8 pointer_1 = 0;
u8 temp;
u8 timer_counter = 0;
u8 timer_counter_1 = 0;

u8 local_counter, local_counter_1;
// Heater parameters
const int PT1000_PIN = A0;
const float vt_factor = 1.01;
const float offset = -25.6;

float temp_c;
int sensor_value;
float voltage;

// Custom characters for degree
byte degree[] = {
	B00110,
	B01001,
	B00110,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000};

// Custom characters to blink
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

u8 pointer = 0, encoder_value = 0;

// Timer
u32 T_Min[2], T_Sec[2];
u8 start_flag = 0;
u8 pre_temp = 50;
u8 pre_temp_1 = 50;
u8 Start_timer[2] = {0, 0};
int TIMER[] = {
	0.5 * 60,
	1 * 60,
	3.5 * 60,
	6 * 60,
	12 * 60,
	16 * 60};

u8 current_timer[2] = {1, 2};
u8 ON_timer = 0, Max_time, Max_Min, Max_sec, On_pointer = 0;

void HEATER_CONTROL(void)
{
	sensor_value = analogRead(PT1000_PIN);
	voltage = sensor_value * (5.0 / 1023.0);
	temp_c = (((voltage * 100) / vt_factor) + offset);
	delay(500);
}

Encoder encoder;

Counter selector_counter, timer1_counter, timer2_counter, temp_counter;

PushButton timer_switch;

enum counter_type
{
	TIMER_1,
	TIMER_2,
	SELECTOR,
	COUNTER_SIZE
};

int selectedCounter = SELECTOR;

Counter *counters[COUNTER_SIZE] = {
	&timer1_counter,
	&timer2_counter,
	&selector_counter};

void setup()
{
	Encoder_VidInit(&encoder, 9, 10, 11);

	Counter_Init(&selector_counter, 0, 1, 1);
	// Counter_Init(&timer1_counter, 0, 24 * 60 - 1, 1);
	Counter_Init(&timer1_counter, 0, 5, 1);
	Counter_Init(&timer2_counter, 0, 5, 1);

	PushButton_Init(&timer_switch, 12, 2000);

	Serial.begin(9600);
	/* LCD INITIALIZATION*/

	/*  The address is 0x27 (discovered using the I2C Scanner Code).
		Number of columns is 16 and number of rows is 2                */

	/* Initialize the display   */
	lcd.init();

	/* Turn on the backlight    */
	lcd.backlight();

	/* Clear the lcd            */
	lcd.clear();
	lcd.createChar(0, degree);
	lcd.createChar(1, Cursor);
	lcd.createChar(2, NoCursor);

	/* Heater INITIALIZATION*/

	pinMode(TEMP_SENSOR, INPUT);
	pinMode(HEATER, OUTPUT);
	pinMode(ALARM, OUTPUT);

	lcd.clear();

	lcd.setCursor(0, 1 - selector_counter.value);
	lcd.print(' ');
	lcd.setCursor(0, selector_counter.value);
	lcd.print('>');

	lcd.setCursor(1, 0);
	lcd.print("T1");

	char time_string[6];
	lcd.setCursor(4, 0);
	sprintf(time_string, "%02d:%02d", TIMER[timer1_counter.value] / 60, TIMER[timer1_counter.value] % 60);
	lcd.printstr(time_string);

	lcd.setCursor(11, 0);
	lcd.print("Temp");

	lcd.setCursor(1, 1);
	lcd.print("T2");

	lcd.setCursor(4, 1);
	sprintf(time_string, "%02d:%02d", TIMER[timer2_counter.value] / 60, TIMER[timer2_counter.value] % 60);
	lcd.printstr(time_string);

	lcd.setCursor(12, 1);
	lcd.print("25");

	lcd.setCursor(14, 1);
	lcd.write(0);

	lcd.setCursor(15, 1);
	lcd.print("C");
}

void loop()
{
	if (timer_switch.isToggled)
	{
		
	}

	if (encoder.button.isPressed)
	{
		selectedCounter = selector_counter.value + SELECTOR - selectedCounter;
		if (selectedCounter == SELECTOR)
		{
			lcd.setCursor(0, 1 - selector_counter.value);
			lcd.print(' ');
			lcd.setCursor(0, selector_counter.value);
			lcd.print('>');
		}
		else
		{
			lcd.setCursor(0, 0);
			lcd.print(' ');
			lcd.setCursor(0, 1);
			lcd.print(' ');
		}
	}

	if (Encoder_popRotationChange(&encoder))
	{
		char time_string[6];
		switch (selectedCounter)
		{
		case SELECTOR:
			lcd.setCursor(0, 1 - selector_counter.value);
			lcd.print(' ');
			lcd.setCursor(0, selector_counter.value);
			lcd.print('>');
			break;
		case TIMER_1:
			lcd.setCursor(4, 0);
			sprintf(time_string, "%02d:%02d", TIMER[timer1_counter.value] / 60, TIMER[timer1_counter.value] % 60);
			lcd.printstr(time_string);
			break;
		case TIMER_2:
			lcd.setCursor(4, 1);
			sprintf(time_string, "%02d:%02d", TIMER[timer2_counter.value] / 60, TIMER[timer2_counter.value] % 60);
			lcd.printstr(time_string);
			break;
		}
	}

	PushButton_Update(&encoder.button);

	PushButton_Update(&timer_switch);

	Encoder_VidControl(&encoder, counters[selectedCounter]);
}