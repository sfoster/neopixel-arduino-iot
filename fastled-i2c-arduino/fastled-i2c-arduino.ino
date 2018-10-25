#include "./config.h"
#include <FastLED.h>
#include <Wire.h>
#include "lib/I2C_Anything.h"
#include "lib/Fx_Helpers.h"
#include "lib/Fx_Animations.h"
#include <limits.h>

#define DATA_PIN 5
#define ANNOUNCE_PIN 0

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

bool gotMessage;
unsigned long receivedTime;

CRGB pixels[NUM_PIXELS];

Anim_Clip currentClip;
Anim_Clip currentLoop;
Anim_Clip* clip;
unsigned long now;

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  if (howMany >= sizeof(Anim_Clip))
  {
     I2C_readAnything(currentClip);
     gotMessage = true;
  }  // end if have enough data

}

void setup() {
  receivedTime = millis();

  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  Wire.begin(I2C_LOCAL_ADDRESS);    // join i2c bus at given address
  Wire.onReceive(receiveEvent);     // register event

  delay(3000); // 3 second delay for recovery

  FastLED.addLeds<WS2812B, NEOPIXEL_DATA_A, RGB>(pixels, NUM_PIXELS);

  // set master brightness control
  FastLED.setBrightness(204);
  Serial.print("setup: set BRIGHTNESS: ");
  Serial.println(BRIGHTNESS);

  CHSV startColor = { .h = 0, .s = 255, .v = 255 };
  CHSV endColor = { .h = 160, .s = 255, .v = 255 };

  currentClip.anim = None;

  currentClip.anim = Pulse;
  currentClip.startColor = { .h = 0, .s = 255, .v = 255 };
  currentClip.endColor = { .h = 160, .s = 255, .v = 51 };
  currentClip.initialDirection = -1;
  currentClip.duration = 2000;
  currentClip.repeat = 15;
  currentClip.startTime = millis();

  currentLoop.anim = Race;
  currentLoop.startColor = { .h = 0, .s = 255, .v = 255 };
  currentLoop.endColor = { .h = 160, .s = 255, .v = 204 };
  currentLoop.duration = 1000;
  currentLoop.initialDirection = -1;
  currentLoop.repeat = SHRT_MAX;
  currentLoop.startTime = millis();

  Serial.print("sizeof: ");
  Serial.println(sizeof(currentLoop));
}

void addGlitter( fract8 chanceOfGlitter)
{
  if( random8() < chanceOfGlitter) {
    pixels[ random16(NUM_PIXELS) ] += CRGB::White;
  }
}

float getClipProgress(Anim_Clip* clip, unsigned long now) {
  unsigned long elapsed = now - clip->startTime;
  float progress = 0.0;

  if (elapsed > clip->duration) {
    clip->startTime = now;
    clip->repeat--;
    Serial.print("elapsed: ");
    Serial.print(elapsed);
    Serial.print(", repeat: ");
    Serial.println(clip->repeat);
    elapsed = elapsed % clip->duration;
  }

  if (clip->duration > 0) {
    if (elapsed > 0) {
      progress = (float)elapsed / (float)clip->duration;
      //debugPrint("elapsed is > 0.0: %ld, progress: %.2f\n", elapsed, progress);
    }
  } else {
    // avoid divide by zero with duration: 0
    progress = 1.0;
  }
  progress = clampFloat(progress, 0.0, 1.0);
  return progress;
}

void loop() {
  now = millis();
  if (gotMessage) {
    Serial.print("Got message of size:");
    Serial.print(sizeof(currentClip));
    dumpClip(&currentClip);
    gotMessage = false;
  }

  if (currentLoop.anim != None) {
    currentLoop.repeat = SHRT_MAX;
  }

  if (currentClip.anim != None) {
    if (currentClip.repeat < 0) {
      currentClip.anim = None;
      clip = &currentLoop;
    } else {
      clip = &currentClip;
    }
  } else {
    clip = &currentLoop;
  }

  if (clip->anim == AllOff) {
    Serial.println("AllOff animation");
    memset8( pixels, 0, NUM_PIXELS * sizeof(CRGB));
  } else {
    float progress = getClipProgress(clip, now);

    if (progress < 0.1) {
      digitalWrite(LED_BUILTIN, 1);
    } else {
      digitalWrite(LED_BUILTIN, 0);
    }
    switch (clip->anim) {
      case Blink: {
        blink(progress, pixels, NUM_PIXELS, clip);
      } break;
      case Pulse: {
        colorPulse(progress, pixels, NUM_PIXELS, clip);
      } break;
      case Race: {
        race(progress, pixels, NUM_PIXELS, clip);
      } break;
      default: {
        Serial.println("No animation matched");
      }
    }
    // dumpColor(pixels[0]);
  }
  // send the 'pixels' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);
  // do some periodic updates
  // EVERY_N_MILLISECONDS( 32 ) { }
}

