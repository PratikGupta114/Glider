#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN GPIO_NUM_23
#define NUM_LEDS 6

#define BLINK_DELAY 50

CRGB leds[NUM_LEDS];

void setup()
{
  // Initialize the Neopixel Leds
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop()
{
  FastLED.clear();

  // blink first 3 reds 3 times
  for (int i = 0; i < 3; i++)
  {
    leds[0] = CRGB(255, 0, 0);
    leds[1] = CRGB(255, 0, 0);
    leds[2] = CRGB(255, 0, 0);
    FastLED.show();
    delay(BLINK_DELAY);
    FastLED.clear();
    FastLED.show();
    delay(BLINK_DELAY);
  }

  // blink second 3 blues 3 times
  for (int i = 0; i < 3; i++)
  {
    leds[5] = CRGB(0, 0, 255);
    leds[3] = CRGB(0, 0, 255);
    leds[4] = CRGB(0, 0, 255);
    FastLED.show();
    delay(BLINK_DELAY);
    FastLED.clear();
    FastLED.show();
    delay(BLINK_DELAY);
  }
}
