
#include "MotorControl.h"
#include "PinDefinitions.h"
#include "Config.h"
#include "Defaults.h"

void moveStraight(int leftMotorSpeed, int rightMotorSpeed)
{
    // Left motor front
    digitalWrite(LEFT_MOTOR_PIN_1, HIGH);
    digitalWrite(LEFT_MOTOR_PIN_2, LOW);

    // Right motor front
    digitalWrite(RIGHT_MOTOR_PIN_1, HIGH);
    digitalWrite(RIGHT_MOTOR_PIN_2, LOW);

    leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);
    rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);

    analogWrite(LEFT_MOTOR_PWM_PIN, leftMotorSpeed);
    analogWrite(RIGHT_MOTOR_PWM_PIN, rightMotorSpeed);
}

void turnCCW(int leftMotorSpeed, int rightMotorSpeed)
{
    // Left motor back
    digitalWrite(LEFT_MOTOR_PIN_1, LOW);
    digitalWrite(LEFT_MOTOR_PIN_2, HIGH);

    // Right motor front
    digitalWrite(RIGHT_MOTOR_PIN_1, HIGH);
    digitalWrite(RIGHT_MOTOR_PIN_2, LOW);

#if (TURN_SPEED_REDUCTION_ENABLED == 1)
    leftMotorSpeed = (leftMotorSpeed * (100 - TURN_SPEED_REDUCTION_PERCENT)) / 100;
    rightMotorSpeed = (rightMotorSpeed * (100 - TURN_SPEED_REDUCTION_PERCENT)) / 100;
#endif

    leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);
    rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);

    analogWrite(LEFT_MOTOR_PWM_PIN, leftMotorSpeed);
    analogWrite(RIGHT_MOTOR_PWM_PIN, rightMotorSpeed);
}

void turnCW(int leftMotorSpeed, int rightMotorSpeed)
{
    // Left motor front
    digitalWrite(LEFT_MOTOR_PIN_1, HIGH);
    digitalWrite(LEFT_MOTOR_PIN_2, LOW);

    // Right motor back
    digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_2, HIGH);

#if (TURN_SPEED_REDUCTION_ENABLED == 1)
    leftMotorSpeed = (leftMotorSpeed * (100 - TURN_SPEED_REDUCTION_PERCENT)) / 100;
    rightMotorSpeed = (rightMotorSpeed * (100 - TURN_SPEED_REDUCTION_PERCENT)) / 100;
#endif

    leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);
    rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);

    analogWrite(LEFT_MOTOR_PWM_PIN, leftMotorSpeed);
    analogWrite(RIGHT_MOTOR_PWM_PIN, rightMotorSpeed);
}

void shortBrake(int durationMillis)
{

    analogWrite(LEFT_MOTOR_PWM_PIN, 0);
    analogWrite(RIGHT_MOTOR_PWM_PIN, 0);

    // Left motor front
    digitalWrite(LEFT_MOTOR_PIN_1, HIGH);
    digitalWrite(LEFT_MOTOR_PIN_2, HIGH);

    // Right motor front
    digitalWrite(RIGHT_MOTOR_PIN_1, HIGH);
    digitalWrite(RIGHT_MOTOR_PIN_2, HIGH);

    delay(durationMillis);
}

void stop()
{
    // Left motor stop
    digitalWrite(LEFT_MOTOR_PIN_1, LOW);
    digitalWrite(LEFT_MOTOR_PIN_2, LOW);

    // Right motor stop
    digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_2, LOW);

    analogWrite(LEFT_MOTOR_PWM_PIN, 0);
    analogWrite(RIGHT_MOTOR_PWM_PIN, 0);
}
