#include <Arduino.h>
#include "PinDefinitions.h"
#include "Config.h"
#include "Defaults.h"
#include "GlobalVariables.h"
#include "MotorControl.h"
#include "Sensor.h"
#include "Wire.h"
#include "FastLED.h"

// Include ArduinoJson.h only when bluetooth logging / tuning is enabled.
#if BLUETOOTH_LOGGING_ENABLED == 1 || BLUETOOTH_TUNING_ENABLED == 1
#include <ArduinoJson.h>

#define TX_DOC_MAX_DATA_LEN 256
#define RX_DOC_MAX_DATA_LEN 192

// Inlcude BluetoothSerial.h only when the config macro for using the inbuilt bluetooth is set to 1
#if USE_INBUILT_BLUETOOTH == 1
#include <BluetoothSerial.h>

#endif
#endif

// Macro definitions for commonly occurring conditions.
// These macro definitions make the code more readable.
#define MID_8_SENSORS_HIGH (s3 == 1 && s4 == 1 && s5 == 1 && s6 == 1 && s7 == 1 && s8 == 1 && s9 == 1 && s10 == 1)
#define MID_8_SENSORS_LOW (s3 == 0 && s4 == 0 && s5 == 0 && s6 == 0 && s7 == 0 && s8 == 0 && s9 == 0 && s10 == 0)

#define MID_6_SENSORS_HIGH (s4 == 1 && s5 == 1 && s6 == 1 && s7 == 1 && s8 == 1 && s9 == 1)
#define MID_6_SENSORS_LOW (s4 == 0 && s5 == 0 && s6 == 0 && s7 == 0 && s8 == 0 && s9 == 0)

// #########################################################################################################################
///////////////////////////////////////////////     VARIABLE DEFINITIONS     ///////////////////////////////////////////////
// #########################################################################################################################

#if BLUETOOTH_LOGGING_ENABLED == 1
StaticJsonDocument<TX_DOC_MAX_DATA_LEN> txDoc;
#endif
#if BLUETOOTH_TUNING_ENABLED == 1
StaticJsonDocument<RX_DOC_MAX_DATA_LEN> rxDoc;
#endif

int Kp = DEFAULT_KP;
int Ki = DEFAULT_KI;
int Kd = DEFAULT_KD;
int baseMotorSpeed = DEFAULT_MOTOR_SPEED;
int loopDelay = DEFAULT_LOOP_DELAY;
int error = 0;
int leftMotorOffset = 0;
int rightMotorOffset = 0;
int P = 0;
int I = 0;
int D = 0;
int error_dir = 0;
int previousError = 0;
int PID_value = 0;

// Make sure to update this variable according to the type of track the LFR is about to run on.
// When the macro for enabling inversion is set to 1, this variable updates itself automatically.
uint8_t trackType = BLACK_LINE_WHITE_TRACK;

CRGB leds[NUM_LEDS];

// Define the Inbuilt bluetooth object only when bluetooth tuning / logging config macro is set to 1
// and the default bluetooth is to be used.
#if ((BLUETOOTH_LOGGING_ENABLED == 1 || BLUETOOTH_TUNING_ENABLED == 1) && USE_INBUILT_BLUETOOTH == 1)
BluetoothSerial SerialBT;
#endif

// #########################################################################################################################
///////////////////////////////////////////////     FUNCTION DEFINITIONS     ///////////////////////////////////////////////
// #########################################################################################################################

/**
 * @brief 	This function is called when inversion is enabled and the trackType variable changes to
 * 			the type different from the default value.
 * 			In this case, 2 out of 6 LEDs of the Neopixel array indicate red for inversion and the
 * 			rest are for indicating checkpoints.
 */
void indicateInversionOn()
{
	leds[0] = CRGB(255, 0, 0);
	leds[1] = CRGB(255, 0, 0);
	FastLED.show();
}

/**
 * @brief 	Clear all indications for inversion only.
 */
void indicateInversionOff()
{
	leds[0] = CRGB(0, 0, 0);
	leds[1] = CRGB(0, 0, 0);
	FastLED.show();
}

/**
 * @brief 	This function is called when an event occurs. In this case, the event is checkpoint detection
 * 			4 out of 6 LEDs of the neopixel array indicate a color diffrent from the color
 * 			used for indicating inversion.
 */
void indicateOn()
{
	digitalWrite(BUZZER, HIGH);
	digitalWrite(BLUE_LED, HIGH);
	leds[3] = CRGB(80, 0, 255);
	leds[4] = CRGB(80, 0, 255);
	leds[2] = CRGB(80, 0, 255);
	leds[5] = CRGB(80, 0, 255);
	FastLED.show();
}

/**
 * @brief	Clear all indications for event only
 *
 */
void indicateOff()
{
	digitalWrite(BUZZER, LOW);
	digitalWrite(BLUE_LED, LOW);
	leds[2] = CRGB(0, 0, 0);
	leds[3] = CRGB(0, 0, 0);
	leds[4] = CRGB(0, 0, 0);
	leds[5] = CRGB(0, 0, 0);
	FastLED.show();
}

/**
 * @brief 	The first step of an LFR's algorithm.
 * 			This function does the following :
 * 				1. Updates the error variable,
 * 				2. Keep a track of; on which side of the sensor was the line was detected ( error_dir )
 *				3. Indicate On / Off for checkpoint & inversion
 *				4. based on the sensor readings check if the robot went out of the line or did it just come across a stop patch
 *			Logic for traversing through maze like intersections can be implemented in this function
 */
void readSensors()
{

	uint16_t sensorData = getSensorReadings();
	error = getCalculatedError(0);

	// left most sensor value
	int s1 = (sensorData & (1 << 13)) >> 13;

	int s2 = (sensorData & (1 << 12)) >> 12;
	int s3 = (sensorData & (1 << 11)) >> 11;
	int s4 = (sensorData & (1 << 10)) >> 10;
	int s5 = (sensorData & (1 << 9)) >> 9;
	int s6 = (sensorData & (1 << 8)) >> 8;
	int s7 = (sensorData & (1 << 7)) >> 7;
	int s8 = (sensorData & (1 << 6)) >> 6;
	int s9 = (sensorData & (1 << 5)) >> 5;
	int s10 = (sensorData & (1 << 4)) >> 4;
	int s11 = (sensorData & (1 << 3)) >> 3;

	// right most sensor value
	int s12 = (sensorData & (1 << 2)) >> 2;

	if (s1 != s12)
		error_dir = s1 - s12;

	if (MID_6_SENSORS_HIGH)
		indicateOn();
	else
		indicateOff();

	if (trackType == WHITE_LINE_BLACK_TRACK)
		indicateInversionOn();
	else
		indicateInversionOff();

	// When all the sensors indicate LFR is out of line
	if (sensorData == 0b0000000000000000)
	{
		// Moved out of the line
		if (error_dir < 0)
			error = OUT_OF_LINE_ERROR_VALUE;
		else if (error_dir > 0)
			error = -1 * OUT_OF_LINE_ERROR_VALUE;
	}
	// When all the sensors indicate LFR is above a patch.
	else if (sensorData == 0b0011111111111100)
	{
		// This is a stop
		moveStraight(baseMotorSpeed, baseMotorSpeed);
		delay(STOP_CHECK_DELAY);
		uint16_t sensorDataAgain = getSensorReadings();
		if (sensorDataAgain == 0b0011111111111100)
		{
			indicateOff();
			shortBrake(1000);
			stop();
			delay(10000);
		}
	}

#if BLUETOOTH_LOGGING_ENABLED == 1

#if USE_SERIAL_BLUETOOTH == 1
	char ss[16];
	// char output[TX_DOC_MAX_DATA_LEN];

	sprintf(ss, "" TWELVE_BIT_SENSOR_PATTERN, TO_TWELVE_BIT_SENSOR_PATTERN(sensorData));

	// txDoc["di"] = String(ss);
	// serializeJson(txDoc, output);
	// Serial.println(output);
	// txDoc.clear();

	Serial.print(F("I|Readings : "));
	Serial.print(String(ss));
	Serial.print(F(" | Error : "));
	Serial.print(error);
	Serial.print(F(" | Inv : "));
	Serial.println(isInverted == WHITE_LINE_BLACK_TRACK ? F("WHITE_LINE_BLACK_TRACK") : F("BLACK_LINE_WHITE_TRACK"));
#endif

#if USE_INBUILT_BLUETOOTH == 1
	SerialBT.printf("I|Readings : " TWELVE_BIT_SENSOR_PATTERN "| Error : %d | Inv :  %s\n",
					TO_TWELVE_BIT_SENSOR_PATTERN(sensorData),
					error, trackType == WHITE_LINE_BLACK_TRACK ? "WHITE_LINE_BLACK_TRACK" : "BLACK_LINE_WHITE_TRACK");
#endif

#endif
}

/**
 * @brief	Calculated the PID_Value based of the error calculated in the previous step.
 * 			The PID valus acts as a speed difference to be added between the left and right motors.
 */
void calculatePID()
{
	P = error;

	if (error == 0)
		I = 0;
	else
		I = I + error;

	I = constrain(I, -200, 200);

	D = error - previousError;
	PID_value = (Kp * P) + (Ki * I) + (Kd * D);
	PID_value = constrain(PID_value, -255, 255);
	previousError = error;
}

/**
 * @brief 	Calculates the individual speeds of left and right motor (between 0 - 255) after introducing
 * 			the PID_Value as an addition / subtraction for either of the left & right motor speeds
 * 			depending on the sign of the PID_Value.
 * 			Besides, this function also makes the robot turn clockwise / counter clockwise if a special
 * 			value is assigned to the error variable, as a routine to make the LFR get back to the line.
 */
void controlMotors()
{
	if (error == OUT_OF_LINE_ERROR_VALUE)
	{
#if BLUETOOTH_LOGGING_ENABLED == 1 && USE_SERIAL_BLUETOOTH == 1
		Serial.println(F("E|Turning Clockwise  : "));
#endif
#if BLUETOOTH_LOGGING_ENABLED == 1 && USE_INBUILT_BLUETOOTH == 1
		SerialBT.println(F("E|Turning Clockwise  : "));
#endif
#if BRAKING_ENABLED == 1
		shortBrake(BRAKE_DURATION_MILLIS);
#endif
		// rotate clockwise until the sensors read a line somewhere in the center.
		uint8_t sensorReadings = getSensorReadings();
		while (isOutOfLine(sensorReadings))
		{
			turnCW(baseMotorSpeed - leftMotorOffset, baseMotorSpeed - rightMotorOffset);
			sensorReadings = getSensorReadings();
		}
// #if BRAKING_ENABLED == 1
// 		shortBrake(BRAKE_DURATION_MILLIS);
// #endif
#if GAPS_ENABLED == 1
		// Check line 300
		error_dir = 0;
#endif
	}
	else if (error == (-1 * OUT_OF_LINE_ERROR_VALUE))
	{
#if BLUETOOTH_LOGGING_ENABLED == 1 && USE_SERIAL_BLUETOOTH == 1
		Serial.println(F("E|Turning Counter Clockwise  : "));
#endif
#if BLUETOOTH_LOGGING_ENABLED == 1 && USE_INBUILT_BLUETOOTH == 1
		SerialBT.println(F("E|Turning Counter Clockwise  : "));
#endif

#if BRAKING_ENABLED == 1
		shortBrake(BRAKE_DURATION_MILLIS);
#endif
		// rotate clockwise until the sensors read a line somewhere in the center.
		uint8_t sensorReadings = getSensorReadings();
		while (isOutOfLine(sensorReadings))
		{
			turnCCW(baseMotorSpeed - leftMotorOffset, baseMotorSpeed - rightMotorOffset);
			sensorReadings = getSensorReadings();
		}
// #if BRAKING_ENABLED == 1
// 		shortBrake(BRAKE_DURATION_MILLIS);
// #endif

// If gaps are enabled in this project, then the error_dir is set to 0 as soon as
// the lfr get back to the line. This way a robot can traverse through line break
// but may get out of dead ends.
#if GAPS_ENABLED == 1
		error_dir = 0;
#endif
	}
	else
	{
		int leftMotorSpeed = baseMotorSpeed + PID_value - leftMotorOffset;
		int rightMotorSpeed = baseMotorSpeed - PID_value - rightMotorOffset;

		moveStraight(leftMotorSpeed, rightMotorSpeed);

		// Additional delay imposed to make the Derivative component's impact more significant.
		// This variable can be tuned dynamically
		if (D != 0)
			delay(loopDelay);
	}
}

// #########################################################################################################################
//////////////////////////////////////////////////////     SETUP     //////////////////////////////////////////////////////
// #########################################################################################################################

void setup()
{
	pinMode(LEFT_MOTOR_PIN_1, OUTPUT);
	pinMode(LEFT_MOTOR_PIN_2, OUTPUT);
	pinMode(RIGHT_MOTOR_PIN_1, OUTPUT);
	pinMode(RIGHT_MOTOR_PIN_2, OUTPUT);
	pinMode(LEFT_MOTOR_PWM_PIN, OUTPUT);
	pinMode(RIGHT_MOTOR_PWM_PIN, OUTPUT);

	pinMode(BLUE_LED, OUTPUT);
	pinMode(BUZZER, OUTPUT);

#if BLUETOOTH_LOGGING_ENABLED == 1 || BLUETOOTH_TUNING_ENABLED == 1

#if USE_SERIAL_BLUETOOTH == 1
	Serial.begin(SERIAL_BAUD_RATE);
#endif

#if USE_INBUILT_BLUETOOTH == 1
	SerialBT.setPin("9637");
	SerialBT.begin("Glider v1.0");
#endif

#endif

	FastLED.addLeds<WS2812, NEOPIXEL_LED_PIN, GRB>(leds, NUM_LEDS);
}

// #########################################################################################################################
//////////////////////////////////////////////////////     LOOP     ///////////////////////////////////////////////////////
// #########################################################################################################################

void loop()
{

#if BLUETOOTH_TUNING_ENABLED == 1

	// Sample Message format
	// {"P":36,"I":60,"D":38,"ms":100,"de":9}

#if USE_SERIAL_BLUETOOTH == 1
	if (Serial.available())
	{
		Serial.flush();
		String data = Serial.readStringUntil('\n');

		Serial.print(F("I|Data received : "));
		Serial.println(data);

		DeserializationError error = deserializeJson(rxDoc, data);

		if (error)
		{
			Serial.print(F("E|deseriaize json failed : "));
			Serial.println(error.f_str());
			return;
			rxDoc.clear();
		}

		Kp = rxDoc["P"];			  // 0
		Ki = rxDoc["I"];			  // 0
		Kd = rxDoc["D"];			  // 0
		baseMotorSpeed = rxDoc["ms"]; // 255
		loopDelay = rxDoc["de"];	  // 100
		rxDoc.clear();
		char buff[64];
		sprintf(buff, "W|P : %d | I : %d | D : %d | ms : %d | de : %d", P, I, D, baseMotorSpeed, loopDelay);
		Serial.println(buff);
	}
#endif

#if USE_INBUILT_BLUETOOTH == 1
	if (SerialBT.available())
	{

		SerialBT.flush();
		String data = SerialBT.readStringUntil('\n');

		SerialBT.printf("I|Data received : %s\n", data.c_str());

		DeserializationError error = deserializeJson(rxDoc, data);

		if (error)
		{
			SerialBT.printf("E|deseriaize json failed : %s\n", error.f_str());
			return;
			rxDoc.clear();
		}

		Kp = rxDoc["P"];			  // 0
		Ki = rxDoc["I"];			  // 0
		Kd = rxDoc["D"];			  // 0
		baseMotorSpeed = rxDoc["ms"]; // 255
		loopDelay = rxDoc["de"];	  // 100
		rxDoc.clear();

		SerialBT.printf("W|P : %d | I : %d | D : %d | ms : %d | de : %d\n", P, I, D, baseMotorSpeed, loopDelay);
	}
#endif

#endif
	// Step 1
	readSensors();

	// Step 2
	calculatePID();

	// Step 3
	controlMotors();
}