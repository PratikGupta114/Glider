#pragma once

#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <stdbool.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)       \
    (byte & 0x80 ? '1' : '0'),     \
        (byte & 0x40 ? '1' : '0'), \
        (byte & 0x20 ? '1' : '0'), \
        (byte & 0x10 ? '1' : '0'), \
        (byte & 0x08 ? '1' : '0'), \
        (byte & 0x04 ? '1' : '0'), \
        (byte & 0x02 ? '1' : '0'), \
        (byte & 0x01 ? '1' : '0')

#define TWO_BYTES_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
#define TWO_BYTES_TO_BINARY(twoBytes)    \
    (twoBytes & 0x8000 ? '1' : '0'),     \
        (twoBytes & 0x4000 ? '1' : '0'), \
        (twoBytes & 0x2000 ? '1' : '0'), \
        (twoBytes & 0x1000 ? '1' : '0'), \
        (twoBytes & 0x0800 ? '1' : '0'), \
        (twoBytes & 0x0400 ? '1' : '0'), \
        (twoBytes & 0x0200 ? '1' : '0'), \
        (twoBytes & 0x0100 ? '1' : '0'), \
        (twoBytes & 0x0080 ? '1' : '0'), \
        (twoBytes & 0x0040 ? '1' : '0'), \
        (twoBytes & 0x0020 ? '1' : '0'), \
        (twoBytes & 0x0010 ? '1' : '0'), \
        (twoBytes & 0x0008 ? '1' : '0'), \
        (twoBytes & 0x0004 ? '1' : '0'), \
        (twoBytes & 0x0002 ? '1' : '0'), \
        (twoBytes & 0x0001 ? '1' : '0')

#define TWELVE_BIT_SENSOR_PATTERN "%c%c%c%c%c%c%c%c%c%c%c%c"
#define TO_TWELVE_BIT_SENSOR_PATTERN(twoBytes) \
    (twoBytes & 0x2000 ? '1' : '0'),           \
        (twoBytes & 0x1000 ? '1' : '0'),       \
        (twoBytes & 0x0800 ? '1' : '0'),       \
        (twoBytes & 0x0400 ? '1' : '0'),       \
        (twoBytes & 0x0200 ? '1' : '0'),       \
        (twoBytes & 0x0100 ? '1' : '0'),       \
        (twoBytes & 0x0080 ? '1' : '0'),       \
        (twoBytes & 0x0040 ? '1' : '0'),       \
        (twoBytes & 0x0020 ? '1' : '0'),       \
        (twoBytes & 0x0010 ? '1' : '0'),       \
        (twoBytes & 0x0008 ? '1' : '0'),       \
        (twoBytes & 0x0004 ? '1' : '0')

uint16_t getSensorReadings();
int getCalculatedError(int fallbackError);
int isOutOfLine(uint16_t sensorReadings);

#endif