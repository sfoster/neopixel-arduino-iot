#include <FastLED.h>
#include <Wire.h>
#include "lib/I2C_Anything.h"
#include "./config.h"
#include "./fastled-i2c.h"
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
  Serial.print("receiveEvent: ");
  Serial.println(howMany);

  if (howMany >= sizeof(Anim_Clip))
  {
     I2C_readAnything (currentClip);
     gotMessage = true;
  }  // end if have enough data

}

void setup() {
  receivedTime = millis();

  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event

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
  currentClip.duration = 2000;
  currentClip.initialDirection = -1;
  currentClip.repeat = 15;
  currentClip.startTime = millis();

  currentLoop.anim = Blink;
  currentLoop.startColor = { .h = 0, .s = 255, .v = 255 };
  currentLoop.endColor = { .h = 160, .s = 255, .v = 51 };
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

void dumpCurrentClip() {
    Serial.print("currentClip, anim: ");
    Serial.print(currentClip.anim);
    Serial.print(": ");
    Serial.print(currentClip.startColor.h);
    Serial.print(",");
    Serial.print(currentClip.startColor.s);
    Serial.print(",");
    Serial.println(currentClip.startColor.v);
    Serial.print(" - ");
    Serial.print(currentClip.endColor.h);
    Serial.print(",");
    Serial.print(currentClip.endColor.s);
    Serial.print(",");
    Serial.println(currentClip.endColor.v);

    Serial.print("duration: ");
    Serial.print(currentClip.duration);
    Serial.print(", repeat: ");
    Serial.print(currentClip.repeat);
    Serial.println(";");
}

void loop() {
  now = millis();
  if (gotMessage) {
    dumpCurrentClip();
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

  switch (clip->anim) {
    case AllOff: {
      Serial.println("AllOff animation");
      memset8( pixels, 0, NUM_PIXELS * sizeof(CRGB));
    } break;
    case Blink: {
      float progress = getClipProgress(clip, now);
      blink(progress, NUM_PIXELS, clip);
    } break;
    case Pulse: {
      float progress = getClipProgress(clip, now);
      colorPulse(progress, NUM_PIXELS, clip);
    } break;
    case Race: {
      float progress = getClipProgress(clip, now);
      race(progress, NUM_PIXELS, clip);
    } break;
    default: {
      Serial.println("No animation matched");
    }
  }
  // send the 'pixels' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);
  // do some periodic updates
  // EVERY_N_MILLISECONDS( 32 ) { }
}

void blink(float progress, unsigned short pixelCount, Anim_Clip *clip) {
  CHSV color = { .hue=HUE_ORANGE, .sat=255, .val=255 };
  if (now % 1000 > 500) {
    color.hue = HUE_YELLOW;
  }
  for(int i = 0; i < pixelCount; i++) {
    pixels[i] = color;
  }
  // addGlitter(8);
}

void colorPulse(float progress, unsigned short pixelCount, Anim_Clip *clip) {
  // debugPrint("colorBlink got progress: %.2f\n", progress);
  float pcentOn;
  short valRange = clip->endColor.val - clip->startColor.val;
  CHSV newColor;

  if (progress <= 0.5f) {
    pcentOn = progress * 2.0f;
  } else {
    pcentOn = (1 - progress) * 2.0f;
  }
  newColor.val = clip->startColor.val  + (valRange * pcentOn);
  for(unsigned short i=0; i<NUM_PIXELS; i++) {
    pixels[i] = newColor;
    // debugPrint("%d: %d,%d,%d; ", i, pixels[i].r, pixels[i].g, pixels[i].b);
  }
}

void race(float progress, unsigned short pixelCount, Anim_Clip *clip) {
  // animate a dot forward then backwards.
  unsigned char iterations = 2;
  unsigned short lastIndex = pixelCount - 1;
  // get a position into the total length (iterations * number of pixels)
  unsigned short position = (float)lastIndex * (float)iterations * progress;
  // the even iterations are going backwards...
  unsigned short ledToLight = position % (lastIndex * 2);
  if (ledToLight > lastIndex) {
    ledToLight = lastIndex - (ledToLight - lastIndex);
  }
  // Serial.print("bounce: position: ");
  // Serial.print(position);
  // Serial.print(", progress: ");
  // Serial.print(progress);
  // Serial.print(", ledToLight ");
  // Serial.println(ledToLight);

  CHSV newColor;
  for(unsigned short i=0; i<pixelCount; i++) {
    unsigned short distance = (unsigned int)ledToLight - i;
    if (distance == 0) {
      newColor = clip->endColor;
    } else if (distance <= pixelCount/5) {
      newColor.hue = clip->endColor.hue / distance;
      newColor.sat = clip->endColor.sat / distance;
      newColor.val = clip->endColor.val / distance;
    } else {
      newColor.hue = 0;
      newColor.sat = 0;
      newColor.val = 0;
    }
    pixels[i] = newColor;
    // debugPrint("%d: %d,%d,%d; ", i, pixels[i].h, pixels[i].g, pixels[i].b);
  }
  // debugPrint("\n");
}
