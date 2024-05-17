#pragma once

#ifndef CONFIG_H
#define CONFIG_H

// Enable logging over Serial / Bluetooth
#define BLUETOOTH_LOGGING_ENABLED 1

// // Enable PID tuning over Serial / Bluetooth
#define BLUETOOTH_TUNING_ENABLED 0

// Enable Communication over Serial ( Serial monitor / external bluetooth host )
#define USE_SERIAL_BLUETOOTH 0

// Enable Communication over Inbuilt bluetooth.
#define USE_INBUILT_BLUETOOTH 1

// THe last two configurations can co-exist.

#if ((BLUETOOTH_LOGGING_ENABLED == 0 && BLUETOOTH_TUNING_ENABLED == 0) && (USE_SERIAL_BLUETOOTH == 1))
#error "Serial Bluetooth should be disabled when both logging and PID tuning over bluetooth is disabled"
#endif

#if ((BLUETOOTH_LOGGING_ENABLED == 0 && BLUETOOTH_TUNING_ENABLED == 0) && (USE_INBUILT_BLUETOOTH == 1))
#error "Inbuilt Bluetooth should be disabled when both logging and PID tuning over bluetooth is disabled"
#endif

#if ((BLUETOOTH_LOGGING_ENABLED == 1 || BLUETOOTH_TUNING_ENABLED == 1) && (USE_INBUILT_BLUETOOTH == 0 && USE_SERIAL_BLUETOOTH == 0))
#error "Either Inbuilt bluetooth or Serial Bluetooth should be enabled when logging or tuning over bluetooth is enabled"
#endif

#if (USE_INBUILT_BLUETOOTH == 1 && (!defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)))
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// This config allows to reverse the default polarity of the motors
#define REVERSE_MOTOR_POLARITY 1

// Allow the robot to reduce the speed by a certain percent when making a
// clockwise / counter clockwise rotation. The percent value is defined in Defaults.h
#define TURN_SPEED_REDUCTION_ENABLED 1

#define BRAKING_ENABLED 0
#define GAPS_ENABLED 1
#define INVERSION_ENABLED 1

#define SERIAL_BAUD_RATE 115200

#endif