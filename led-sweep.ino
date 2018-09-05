#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
  #include "led-sweep.h"
#include "animations.h"

#define PIN 5

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// -------------------------

enum State currentState = start_state;
funcptr animateFn;

// last timestamp
int previousMillis;
// timestamp from the start of the current tick
int now;
int startTime;
int animationDuration;
bool isAnimating = false;

void updateState() {
  previousMillis = now;
  now = millis();
  if (previousMillis > now) {
    // overflow happened
    previousMillis = now;
  }

  switch (currentState) {
    case start_state :
      isAnimating = false;
      currentState = color_fade_transition;
      break;
    case off_transition :
      allOff(0.0);
      isAnimating = false;
      currentState = off_state;
      break;
    case off_state :
      isAnimating = false;
      break;
    case color_fade_transition :
      isAnimating = true;
      startTime = now;
      animationDuration = 5000;
      currentState = color_fade_state;
      animateFn = &colorFade;
      debugPrint("color_fade_transition, isAnimating is now true\n");
  }
  if (isAnimating) {
    int elapsed = clamp(now - startTime, 0, animationDuration);
    if (elapsed >= animationDuration) {
      currentState = off_transition;
      return;
    }
    double progress = 0.0;
    if (elapsed > 0) {
      progress = (float)elapsed / (float)animationDuration;
    }

    (*animateFn)(progress);
  }
}

void display() {
  for(int i=0; i<NUM_PIXELS; i++) {
    strip.setPixelColor(i, pixelColors[i].r, pixelColors[i].g, pixelColors[i].b);
    strip.show();
  }
}

void setup() {
  Serial.begin(115200);
  strip.begin();
  now = millis();
  // Initialize all pixels to 'off'
  allOff(0.0);
  display();

  // todo: initialize the array of rgb values we'll update each frame
  debugPrint("setup");
}


void loop() {
  updateState();
  display();
  delay(16); // on esp8266 delay yields
}

