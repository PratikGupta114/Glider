#include <Arduino.h>

#include <Arduino.h>

#ifdef ESP32
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#endif

#ifdef ARDUINO_AVR_NANO
#define LOAD_PIN 4
#define CLOCK_ENABLE_PIN 3
#define DATA_PIN 6
#define CLOCK_PIN 5
#endif

#ifdef ESP32
#define LOAD_PIN GPIO_NUM_27
#define CLOCK_ENABLE_PIN GPIO_NUM_4
#define DATA_PIN GPIO_NUM_17
#define CLOCK_PIN GPIO_NUM_16

#endif

#define TWO_BYTES_TO_BINARY_PATTERN "%c%c    %c%c%c%c%c%c %c%c%c%c%c%c    %c%c"
#define TWO_BYTES_TO_BINARY(twoBytes)  \
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

#ifdef ESP32
uint8_t shiftIn165(gpio_num_t dataPin, gpio_num_t clockPin, uint8_t bitOrder)
#endif
#ifdef ARDUINO_AVR_NANO
    uint8_t shiftIn165(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
#endif
{
  uint8_t value = 0;
  uint8_t i;

  for (i = 0; i < 8; ++i)
  {

#ifdef ESP32
    uint8_t r = gpio_get_level(dataPin);
#endif
#ifdef ARDUINO_AVR_NANO
    uint8_t r = digitalRead(dataPin);
#endif

    if (bitOrder == LSBFIRST)
      value |= r << i;
    else
      value |= r << (7 - i);

#ifdef ESP32
    gpio_set_level(clockPin, 1);
#endif
#ifdef ARDUINO_AVR_NANO
    digitalWrite(clockPin, HIGH);
#endif
#ifdef ESP32
    gpio_set_level(clockPin, 0);
#endif
#ifdef ARDUINO_AVR_NANO
    digitalWrite(clockPin, LOW);
#endif
  }

  return value;
}

void setup()
{
  // Setup Serial Monitor
  Serial.begin(115200);

  // Setup 74HC165 connections
#ifdef ARDUINO_AVR_NANO
  pinMode(LOAD_PIN, OUTPUT);
  pinMode(CLOCK_ENABLE_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, INPUT_PULLUP);
  delay(10);
  digitalWrite(CLOCK_ENABLE_PIN, HIGH);
#endif

#ifdef ESP32
  gpio_config_t io_config;
  io_config.pin_bit_mask = (1 << DATA_PIN);
  io_config.mode = GPIO_MODE_INPUT;
  io_config.pull_up_en = GPIO_PULLUP_ENABLE;
  io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_config.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&io_config);

  io_config.pin_bit_mask = (1 << CLOCK_PIN | 1 << CLOCK_ENABLE_PIN | 1 << LOAD_PIN);
  io_config.mode = GPIO_MODE_OUTPUT;
  io_config.pull_up_en = GPIO_PULLUP_DISABLE;
  io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_config.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&io_config);
  delay(10);
  gpio_set_level(CLOCK_ENABLE_PIN, 1);
#endif
}

uint8_t horizontalFlip(uint8_t byte)
{
  uint8_t flippedByte = 0x00;
  for (int i = 0; i < 4; i++)
  {
    uint8_t bs = (byte >> i) & 0x01;
    uint8_t be = (byte >> (7 - i)) & 0x01;
    flippedByte |= (bs << (7 - i)) | (be << i);
  }
  return flippedByte;
}

int state = 0;

void loop()
{
// Write pulse to load pin
#ifdef ESP32
  gpio_set_level(LOAD_PIN, 0);
#endif
#ifdef ARDUINO_AVR_NANO
  digitalWrite(LOAD_PIN, LOW);
#endif
  delayMicroseconds(100);

#ifdef ESP32
  gpio_set_level(LOAD_PIN, 1);
#endif
#ifdef ARDUINO_AVR_NANO
  digitalWrite(LOAD_PIN, HIGH);
#endif
  delayMicroseconds(100);

  // Get data from 74HC165
#ifdef ESP32
  gpio_set_level(CLOCK_ENABLE_PIN, 0);
#endif
#ifdef ARDUINO_AVR_NANO
  digitalWrite(CLOCK_ENABLE_PIN, LOW);
#endif

  uint8_t leftSide = shiftIn165(DATA_PIN, CLOCK_PIN, MSBFIRST);
  uint8_t rightSide = shiftIn165(DATA_PIN, CLOCK_PIN, MSBFIRST);

  uint16_t reading = leftSide << 8 | rightSide;

#ifdef ESP32
  gpio_set_level(CLOCK_ENABLE_PIN, 1);
#endif
#ifdef ARDUINO_AVR_NANO
  digitalWrite(CLOCK_ENABLE_PIN, HIGH);
#endif

  char str[28];
  snprintf(str, 28, TWO_BYTES_TO_BINARY_PATTERN, TWO_BYTES_TO_BINARY(reading));
  Serial.println(str);
}