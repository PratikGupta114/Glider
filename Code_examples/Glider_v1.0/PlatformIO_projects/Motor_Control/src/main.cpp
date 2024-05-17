#include <Arduino.h>

#ifdef ESP32

#define AIN2 GPIO_NUM_25
#define AIN1 GPIO_NUM_26
#define BIN2 GPIO_NUM_32
#define BIN1 GPIO_NUM_33
#define PWMA GPIO_NUM_19
#define PWMB GPIO_NUM_18

#endif

#ifdef ARDUINO_AVR_NANO

#define AIN2 10
#define AIN1 9
#define BIN2 7
#define BIN1 6
#define PWMA 11
#define PWMB 5

#endif

int dir = 1;

void setup()
{
  pinMode(AIN2, OUTPUT); // AIN2
  pinMode(AIN1, OUTPUT); // AIN1
  pinMode(BIN2, OUTPUT); // BIN2
  pinMode(BIN1, OUTPUT); // BIN1
  pinMode(PWMA, OUTPUT); // PWMA
  pinMode(PWMB, OUTPUT); // PWMB
}

void loop()
{

  for (int i = 0; i < 255; i++)
  {
    analogWrite(PWMA, i);
    analogWrite(PWMB, i);
    delay(10);
  }

  for (int i = 255; i >= 0; i--)
  {
    analogWrite(PWMA, i);
    analogWrite(PWMB, i);
    delay(10);
  }

  // toggle direction
  digitalWrite(AIN1, 1 - dir);
  digitalWrite(AIN2, dir);
  digitalWrite(BIN1, 1 - dir);
  digitalWrite(BIN2, dir);

  dir = 1 - dir;
  delay(100);
}