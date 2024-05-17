
#ifndef DEFAULTS_H
#define DEFAULTS_H

#define OUT_OF_LINE_ERROR_VALUE 20
#define LOOP_ESCAPE_ERROR_VALUE 3

// The duration in milliseconds for which the short brake is applied.
#define BRAKE_DURATION_MILLIS 100

#define DEFAULT_LOOP_DELAY 0
#define DEFAULT_KP 0
#define DEFAULT_KI 0
#define DEFAULT_KD 0
#define DEFAULT_MOTOR_SPEED 0
#define WHITE_LINE_BLACK_TRACK 0
#define BLACK_LINE_WHITE_TRACK 1

// Percentage by which the base motor speed reduces
// whenever the robot rotates clockwise / counter clockwise.
#define TURN_SPEED_REDUCTION_PERCENT 10

// The duration in milliseconds for which the robot moves straight to
// confirm the presence of a stop patch.
#define STOP_CHECK_DELAY 150

#endif