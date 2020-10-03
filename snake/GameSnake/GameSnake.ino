#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>

// Params for width and height
const uint8_t kMatrixWidth = 30; // length of string
const uint8_t kMatrixHeight = 30;
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)

CRGB leds[NUM_LEDS];

bool kMatrixSerpentineLayout = true;

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  if (x > kMatrixWidth) return 0;
  if (y > kMatrixHeight) return 0;

  if ( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if ( kMatrixSerpentineLayout == true) {
    if ( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }

  if (i >= (NUM_LEDS - 1)) return 0;
  return i;
}

#include "snake.h"
GameSnake snake;

#if defined(CORE_TEENSY)
#include "control_teensy.h"
#else
#include "control_esp.h"
#endif

void setup() {
  Serial.begin(115200);
  controlSetup();
  FastLED.addLeds<WS2812, 2, BRG>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);

  FastLED.setBrightness(10);
}

int incomingByte = 0;
void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    snake.input(incomingByte);
  }
  controlLoop();
  snake.frame();
}
