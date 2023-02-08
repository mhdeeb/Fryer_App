#include <Arduino.h>
#line 1 "c:\\Users\\eldeeb\\Desktop\\Fryer_APP\\Fryer_APP.ino"
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
#include "Heater_Interface.h"

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
u32 TIMER[6] = {
	1 * 5 * 60,
	2 * 5 * 60,
	3 * 5 * 60,
	4 * 5 * 60,
	5 * 5 * 60};

u8 current_timer[2] = {1, 2};
u8 ON_timer = 0, Max_time, Max_Min, Max_sec, On_pointer = 0;

#line 87 "c:\\Users\\eldeeb\\Desktop\\Fryer_APP\\Fryer_APP.ino"
void HEATER_CONTROL(void);
#line 114 "c:\\Users\\eldeeb\\Desktop\\Fryer_APP\\Fryer_APP.ino"
void setup();
#line 178 "c:\\Users\\eldeeb\\Desktop\\Fryer_APP\\Fryer_APP.ino"
void loop();
#line 87 "c:\\Users\\eldeeb\\Desktop\\Fryer_APP\\Fryer_APP.ino"
void HEATER_CONTROL(void)
{
	sensor_value = analogRead(PT1000_PIN);
	voltage = sensor_value * (5.0 / 1023.0);
	temp_c = (((voltage * 100) / vt_factor) + offset);
	delay(500);
}

Encoder encoder;

Counter selector_counter, timer1_counter, timer2_counter, temp_counter;

enum counter_type
{
	SELECTOR,
	TIMER_1,
	TIMER_2,
	COUNTER_SIZE
};

int selectedCounter = 0;

Counter *counters[COUNTER_SIZE] = {
	&timer1_counter,
	&timer2_counter,
	&selector_counter};

void setup()
{
	Encoder_VidInit(&encoder, 9, 10, 11);

	Counter_Init(&selector_counter, 0, 1, 1);

	Counter_Init(&timer1_counter, 0, 24 * 60, 1);

	Counter_Init(&timer2_counter, 0, 24 * 60, 1);

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

	lcd.setCursor(1, 0);
	lcd.print("T1");

	lcd.setCursor(4, 0);
	lcd.print("00:00");

	lcd.setCursor(11, 0);
	lcd.print("Temp");

	lcd.setCursor(1, 1);
	lcd.print("T2");

	lcd.setCursor(4, 1);
	lcd.print("00:00");

	lcd.setCursor(12, 1);
	lcd.print("25");

	lcd.setCursor(14, 1);
	lcd.write(0);

	lcd.setCursor(15, 1);
	lcd.print("C");
}

long lastDebounceTime = 0;
long debounceDelay = 50;

void loop()
{
	if ((millis() - lastDebounceTime) > debounceDelay)
	{
		char time_string[6];

		lcd.setCursor(0, 0);
		lcd.print(' ');
		lcd.setCursor(0, 1);
		lcd.print(' ');
		lcd.setCursor(4, 0);
		Serial.println(timer1_counter.value);
		sprintf(time_string, "%02d:%02d", timer1_counter.value / 60, timer1_counter.value % 60);
		lcd.printstr(time_string);
		lcd.setCursor(4, 1);
		sprintf(time_string, "%02d:%02d", timer2_counter.value / 60, timer2_counter.value % 60);
		lcd.printstr(time_string);

		if (digitalRead(encoder.buttonPin) == LOW && encoder.ButtonState == HIGH)
		{
			selectedCounter = selector_counter.value;
		}

		Encoder_VidControl(&encoder, counters[selectedCounter]);

		switch (selectedCounter)
		{
		case SELECTOR:
			lcd.setCursor(0, selector_counter.value);
			lcd.print('>');
			break;
		case TIMER_1:
			break;
		case TIMER_2:
			break;
		}

		lastDebounceTime = millis();
	}
}
#line 1 "c:\\Users\\eldeeb\\Desktop\\Fryer_APP\\FRYER_APP2.ino"
// /*****************************************/
// /*****************************************/
// /****** By  : Abdelrahman Mohamed ********/
// /****** Date:     7/DEC/2022      ********/
// /****** SWC : FRYER_APPLICATION   ********/
// /****** VER :        1.00         ********/
// /*****************************************/
// /*****************************************/
// #include "STD_TYPES.h"

// #include <Wire.h> /*This library help to communicate with I2C device*/
// #include <LiquidCrystal_I2C.h>

// #include "Encoder_Interface.h"
// #include "Heater_Interface.h"

// LiquidCrystal_I2C lcd(0x27, 20, 4);

// // Encoder parameters

// u8 A_State, A_LastState, SW, Real_counter;
// u8 Rpointer = 0, Cpointer = 0;
// u32 SW_counter = 0;
// u8 pointer_1 = 0;
// u8 temp;
// u8 timer_counter = 0;
// u8 timer_counter_1 = 0;

// u8 local_counter, local_counter_1;
// // Heater parameters
// const int PT1000_PIN = A0;
// const float vt_factor = 1.01;
// const float offset = -25.6;

// float temp_c;
// int sensor_value;
// float voltage;

// // Custom characters of celisios
// byte celisios[] = {
//   B00110,
//   B01001,
//   B00110,
//   B00000,
//   B00000,
//   B00000,
//   B00000,
//   B00000
// };

// // Custom characters to blink
// byte Cursor[] = {
//   B00000,
//   B00000,
//   B00000,
//   B00000,
//   B00000,
//   B00000,
//   B00000,
//   B11111
// };
// byte NoCursor[] = {
//   B00000,
//   B00000,
//   B00000,
//   B00000,
//   B00000,
//   B00000,
//   B00000,
//   B00000
// };

// u8 pointer = 0, encoder_value = 0;

// // Timer
// u32 T_Min[2], T_Sec[2];
// u8 start_flag = 0;
// u8 pre_temp = 50;
// u8 pre_temp_1 = 50;
// u8 Start_timer[2] = {
//   0,
//   0
// };
// u32 TIMER[6] = {
//   3200,
//   0,
//   0,
//   0,
//   0,
//   0

// };
// u8 current_timer[2] = { 1, 2 };
// u8 ON_timer = 0, Max_time, Max_Min, Max_sec, On_pointer = 0;

// void HEATER_CONTROL(void) {
//   sensor_value = analogRead(PT1000_PIN);
//   voltage = sensor_value * (5.0 / 1023.0);
//   temp_c = (((voltage * 100) / vt_factor) + offset);
//   delay(500);
// }

// u8 Encoder_VidControl(u8 Counter) {
//   A_State = digitalRead(OUTPUT_A);  // Reads the "current" state of the outputA
//   // If the previous and the current state of the outputA are different, that means a Pulse has occured
//   if (A_State != A_LastState) {
//     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
//     if (digitalRead(OUTPUT_B) != A_State) {
//       Counter++;
//     } else {
//       Counter--;
//     }
//   }
//   A_LastState = A_State;  // Updates the previous state of the outputA with the current state
//   return Counter;
// }

// void setup() {
//   Serial.begin(9600);
//   /* LCD INITIALIZATION*/

//   /*  The address is 0x27 (discovered using the I2C Scanner Code).
//       Number of columns is 16 and number of rows is 2                */

//   /* Initialize the display   */
//   lcd.init();

//   /* Turn on the backlight    */
//   lcd.backlight();

//   /* Clear the lcd            */
//   lcd.clear();
//   lcd.createChar(0, celisios);
//   lcd.createChar(1, Cursor);
//   lcd.createChar(2, NoCursor);

//   /* Encoder INITIALIZATION*/
//   pinMode(13, OUTPUT);

//   pinMode(OUTPUT_A, INPUT);
//   pinMode(OUTPUT_B, INPUT);
//   pinMode(BUTTON, INPUT_PULLUP);

//   // use of Pin Change Interrupt
//   //  Enable PCIE0 Bit0 = 1 (Port D)
//   PCICR |= B00000001;
//   // Select PCINT3 Bit4 = 1 (Pin D11)
//   PCMSK0 |= B00001110;

//   // Reads the initial state of the outputA
//   A_LastState = digitalRead(OUTPUT_A);

//   /* Heater INITIALIZATION*/

//   pinMode(TEMP_SENSOR, INPUT);
//   pinMode(HEATER, OUTPUT);
//   pinMode(ALARM, OUTPUT);

//   lcd.clear();
//   for (int x = 0; x < 2; x++) {
//     lcd.setCursor(4, 0);
//     lcd.print("WELCOME");
//     delay(750);
//     lcd.clear();
//     delay(400);
//   }
//   lcd.setCursor(1, 0);
//   lcd.print("T1");

//   lcd.setCursor(4, 0);
//   lcd.print("00:00");

//   lcd.setCursor(11, 0);
//   lcd.print("Temp");


//   lcd.setCursor(1, 1);
//   lcd.print("T2");

//   lcd.setCursor(4, 1);
//   lcd.print("00:00");

//   lcd.setCursor(12, 1);
//   lcd.print("25");

//   lcd.setCursor(14, 1);
//   lcd.write(0);

//   lcd.setCursor(15, 1);
//   lcd.print("C");
// }

// void loop() {
//   if (SW_counter > 0) {
//     Real_counter++;
//     SW_counter = 0;
//   }
//   temp = (Real_counter / 2);

//   switch (temp) {
//     case 1:
//       // lcd.setCursor(0, pointer_1);
//       // lcd.print(">");
//       // // delay(500);
//       // lcd.setCursor(0, pointer_1);
//       // lcd.print(" ");
//       // delay(500);
//       do {
//         Serial.println(0);
//         encoder_value = Encoder_VidControl(encoder_value);
//         lcd.setCursor(0, pointer_1);
//         lcd.print(">");
//         // delay(250);
//         // lcd.setCursor(0, pointer_1);
//         // lcd.print(" ");
//         // delay(250);

//         if (encoder_value != pointer) {
//           pointer_1 = !pointer_1;
//           pointer = encoder_value;
//         }
//         if (SW_counter > 0) {

//           Real_counter++;
//           SW_counter = 0;
//         }
//         temp = (Real_counter / 2);

//       } while (temp == 1);

//       break;
//     case 2:
//       // Start timer

//       do {
//                 Serial.println(1);
//         if (SW_counter > 0) {
//           temp = 10;
//           Start_timer[pointer_1] = 1;
//           SW_counter = 0;
//         }
//         temp = (Real_counter / 2);
//       } while (temp == 2);

//       break;

//     case 3:

//       local_counter = pointer_1 + 1;
//       lcd.setCursor(0, pointer_1);
//       lcd.print(">");
//       pointer = local_counter;
//       do {
//         Serial.println(2);
//         lcd.setCursor(2, pointer_1);
//         lcd.cursor();
//         delay(250);
//         lcd.setCursor(2, pointer_1);
//         lcd.noCursor();
//         delay(250);

//         if (local_counter != pointer) {
//           pointer = encoder_value;
//           if (local_counter > 6) {
//             local_counter_1 = 3;
//             local_counter = 1;
//           } else if (local_counter < 1) {
//             local_counter_1 = 18;
//             local_counter = 6;
//           }
//           lcd.setCursor(2, pointer_1);
//           lcd.print(local_counter);
//           T_Min[pointer_1] = TIMER[(local_counter - 1)] / 60;
//           T_Sec[pointer_1] = TIMER[(local_counter - 1)] % 60;

//           if (T_Min[pointer_1] < 10) {
//             lcd.setCursor(4, pointer_1);
//             lcd.print("0");
//             lcd.setCursor(5, pointer_1);
//             lcd.print(T_Min[pointer_1]);
//           } else {
//             lcd.setCursor(4, pointer_1);
//             lcd.print(T_Min[pointer_1]);
//           }
//           if (T_Sec[pointer_1] < 10) {
//             lcd.setCursor(7, pointer_1);
//             lcd.print("0");
//             lcd.setCursor(8, pointer_1);
//             lcd.print(T_Sec[pointer_1]);
//           } else {
//             lcd.setCursor(7, pointer_1);
//             lcd.print(T_Sec[pointer_1]);
//           }
//         }

//         if (SW_counter > 0) {

//           Real_counter++;
//           SW_counter = 0;
//         }
//         temp = (Real_counter / 2);
//       } while (temp == 3);
//       current_timer[pointer_1] = local_counter;

//       break;
//     case 4:

//       timer_counter = TIMER[(local_counter - 1)] / 60;
//       do {
//         Serial.println(3);
//         if (timer_counter <= 9) {
//           lcd.setCursor(4, pointer_1);
//           lcd.write(1);
//           lcd.setCursor(5, pointer_1);
//           lcd.write(1);
//           delay(250);
//           lcd.setCursor(4, pointer_1);
//           lcd.print("0");
//           lcd.setCursor(5, pointer_1);
//           lcd.print(timer_counter);

//           lcd.setCursor(6, pointer_1);
//           lcd.print(":");
//           delay(250);
//         } else if (timer_counter <= 60) {
//           lcd.setCursor(4, pointer_1);
//           lcd.write(1);
//           lcd.setCursor(5, pointer_1);
//           lcd.write(1);
//           delay(250);
//           lcd.setCursor(4, pointer_1);
//           lcd.print(timer_counter);

//           lcd.setCursor(6, pointer_1);
//           lcd.print(":");
//           delay(250);
//         } else if (timer_counter > 60) {
//           timer_counter_1 = 0;
//           timer_counter = 0;
//         }

//         T_Min[pointer_1] = timer_counter;
//         if (SW_counter > 0) {
//           Real_counter++;
//           SW_counter = 0;
//         }
//         temp = (Real_counter / 2);
//       } while (temp == 4);
//       break;
//     case 5:

//       timer_counter = TIMER[(local_counter - 1)] % 60;
//       do {
//         Serial.println(4);
//         if (timer_counter <= 9) {
//           lcd.setCursor(7, pointer_1);
//           lcd.write(1);
//           lcd.setCursor(8, pointer_1);
//           lcd.write(1);
//           delay(250);
//           lcd.setCursor(7, pointer_1);
//           lcd.print("0");
//           lcd.setCursor(8, pointer_1);
//           lcd.print(timer_counter);

//           lcd.setCursor(9, pointer_1);
//           lcd.print(" ");
//           delay(250);
//         } else if (timer_counter <= 60) {
//           lcd.setCursor(7, pointer_1);
//           lcd.write(1);
//           lcd.setCursor(8, pointer_1);
//           lcd.write(1);
//           delay(250);
//           lcd.setCursor(7, pointer_1);
//           lcd.print(timer_counter);

//           lcd.setCursor(9, pointer_1);
//           lcd.print(" ");
//           delay(250);
//         } else if (timer_counter > 60) {
//           timer_counter_1 = 0;
//           timer_counter = 0;
//         }

//         if (SW_counter > 0) {
//           Real_counter++;
//           SW_counter = 0;
//         }
//         temp = (Real_counter / 2);
//       } while (temp == 5);

//       T_Sec[pointer_1] = timer_counter;
//       T_Min[pointer_1] = T_Min[pointer_1] * 60;
//       T_Sec[pointer_1] += T_Min[pointer_1];
//       TIMER[(local_counter - 1)] = T_Sec[pointer_1];

//       // Real_counter=2;
//       T_Min[pointer_1] = 0;
//       T_Sec[pointer_1] = 0;
//       delay(500);

//       break;
//     case 6:
//       do {
//         Serial.println(5);
//         if (pre_temp < 100) {
//           lcd.setCursor(11, 1);
//           lcd.print(" ");
//           lcd.setCursor(12, 1);
//           lcd.write(1);
//           lcd.setCursor(13, 1);
//           lcd.write(1);
//           delay(250);

//           lcd.setCursor(12, 1);
//           lcd.print(pre_temp);

//           delay(250);
//         } else if (pre_temp >= 100) {
//           lcd.setCursor(11, 1);
//           lcd.write(1);
//           lcd.setCursor(12, 1);
//           lcd.write(1);
//           lcd.setCursor(13, 1);
//           lcd.write(1);

//           delay(250);
//           lcd.setCursor(11, 1);
//           lcd.print(pre_temp);

//           delay(250);
//         }
//         if (pre_temp > 210) {
//           pre_temp = 100;
//         }
//         if (SW_counter > 0) {
//           Real_counter++;
//           SW_counter = 0;
//         }
//         temp = (Real_counter / 2);
//       } while (temp == 6);
//       Real_counter = 2;
//       break;
//   }
//   /* Start Timer */
//   while (temp == 10) {
//     u8 T1, T2;
//     ON_timer = 0;

//     T_Min[0] = TIMER[current_timer[0]] / 60;
//     T_Sec[0] = TIMER[current_timer[0]] % 60;
//     T_Min[1] = TIMER[current_timer[1]] / 60;
//     T_Sec[1] = TIMER[current_timer[1]] % 60;
//     if (pointer_1 == 0) {
//       T1 = 0;
//       T2 = 1;
//     } else {
//       T1 = 1;
//       T2 = 0;
//     }
//     if (TIMER[current_timer[0]] > TIMER[current_timer[1]]) {
//       Max_time = TIMER[current_timer[0]];
//       Max_Min = T_Min[0];
//       Max_sec = T_Sec[0];
//     } else {
//       Max_time = TIMER[current_timer[1]];
//       Max_Min = T_Min[1];
//       Max_sec = T_Sec[1];
//     }

//     for (u8 k = Max_Min; k >= 0; k--) {
//       for (u8 j = Max_sec; j >= 0; j--) {
//         switch (ON_timer) {
//           case 0:
//             break;
//           case 1:
//             if (T_Sec[T1] <= 9) {
//               lcd.setCursor(7, T1);
//               lcd.print("0");
//               lcd.setCursor(8, T1);
//               lcd.print(T_Sec[T1]);

//               lcd.setCursor(9, T1);
//               lcd.print(" ");
//             } else if (T_Sec[T1] < 60) {

//               lcd.setCursor(7, T1);
//               lcd.print(T_Sec[T1]);
//               lcd.setCursor(9, T1);
//               lcd.print(" ");
//             }
//             T_Sec[T1]--;
//             break;
//           case 2:
//             if (T_Sec[T1] <= 9) {
//               lcd.setCursor(7, T1);
//               lcd.print("0");
//               lcd.setCursor(8, T1);
//               lcd.print(T_Sec[T1]);

//               lcd.setCursor(9, T1);
//               lcd.print(" ");
//             } else if (T_Sec[T1] < 60) {

//               lcd.setCursor(7, T1);
//               lcd.print(T_Sec[T1]);
//               lcd.setCursor(9, T1);
//               lcd.print(" ");
//             }
//             T_Sec[T1]--;
//             if (T_Sec[T2] <= 9) {
//               lcd.setCursor(7, T2);
//               lcd.print("0");
//               lcd.setCursor(8, T2);
//               lcd.print(T_Sec[T2]);

//               lcd.setCursor(9, T2);
//               lcd.print(" ");
//             } else if (T_Sec[T2] < 60) {

//               lcd.setCursor(7, T2);
//               lcd.print(T_Sec[T2]);
//               lcd.setCursor(9, T2);
//               lcd.print(" ");
//             }
//             T_Sec[T1]--;
//             break;
//         }

//         delay(999);
//       }
//     }
//   }
// }

// ISR(PCINT0_vect) {
//   if ((SW = digitalRead(BUTTON)) == LOW) {
//     while (SW == LOW) {
//       Serial.println(6);
//       SW = digitalRead(BUTTON);
//     }

//     SW_counter++;

//     if (temp == 10) {
//       if (SW_counter > 2) {
//         Real_counter = SW_counter / 2;
//         SW_counter = 0;
//       }
//     }
//   }

//   if (((digitalRead(OUTPUT_A) == HIGH || digitalRead(OUTPUT_B)) & temp == 1) == HIGH) {
//     encoder_value = Encoder_VidControl(encoder_value);
//   }

//   if (((digitalRead(OUTPUT_A) == HIGH || digitalRead(OUTPUT_B)) & temp == 2) == HIGH) {
//     Real_counter = 6;
//   }

//   if (((digitalRead(OUTPUT_A) == HIGH || digitalRead(OUTPUT_B)) & temp == 3) == HIGH) {
//     local_counter_1 = Encoder_VidControl(local_counter_1);
//     local_counter = (local_counter_1) / 3;
//   }

//   if (((digitalRead(OUTPUT_A) == HIGH || digitalRead(OUTPUT_B)) & ((temp == 4) || (temp == 5))) == HIGH) {
//     timer_counter_1 = Encoder_VidControl(timer_counter_1);
//     timer_counter = (timer_counter_1) / 3;
//   }

//   if (((digitalRead(OUTPUT_A) == HIGH || digitalRead(OUTPUT_B)) & temp == 6) == HIGH) {
//     pre_temp_1 = Encoder_VidControl(pre_temp_1);
//     if (pre_temp_1 != pre_temp) {
//       if (pre_temp_1 > pre_temp) {
//         pre_temp_1 = pre_temp + 5;
//         pre_temp = pre_temp_1;
//       } else {
//         pre_temp_1 = pre_temp - 5;
//         pre_temp = pre_temp_1;
//       }
//     }
//   }

//   if (((digitalRead(OUTPUT_A) == HIGH || digitalRead(OUTPUT_B)) & temp == 10) == HIGH) {
//     On_pointer = Encoder_VidControl(On_pointer);
//     ON_timer = Start_timer[0] + Start_timer[1];
//   }
// }

#line 1 "c:\\Users\\eldeeb\\Desktop\\Fryer_APP\\Heater_Program.ino"
/*****************************************/
/*****************************************/
/****** By  : Abdelrahman Mohamed ********/
/****** Date:     7/DEC/2022      ********/
/****** SWC :       Heater        ********/
/****** VER :        1.00         ********/
/*****************************************/
/*****************************************/

