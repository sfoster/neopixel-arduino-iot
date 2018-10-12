#include "../config.h"
#include "Fx_Types.h"
#include "Fx_Helpers.h"
#include "Fx_Animations.h"

// -----------------------------------------
// Animation functions
//  take a value of 0-1 representing the %age of duration elapsed, and update the pixels buffer
//  with the r,g,b values for each pixel

void colorBlink(float progress, RGBColor* pixels, unsigned short pixelCount, Fx_AnimationParams *params) {
  // debugPrint("colorBlink got progress: %.2f\n", progress);
  float pcentOn;
  short rRange = params->endColor.r - params->startColor.r;
  short gRange = params->endColor.g - params->startColor.g;
  short bRange = params->endColor.b - params->startColor.b;

  if (progress <= 0.5f) {
    pcentOn = progress * 2.0f;
  } else {
    pcentOn = (1 - progress) * 2.0f;
  }
  for(unsigned short i=0; i<NUM_PIXELS; i++) {
    pixels[i].r = params->startColor.r  + (rRange * pcentOn);
    pixels[i].g = params->startColor.g  + (gRange * pcentOn);
    pixels[i].b = params->startColor.b  + (bRange * pcentOn);
    // debugPrint("%d: %d,%d,%d; ", i, pixels[i].r, pixels[i].g, pixels[i].b);
  }
  debugPrint("\n");
}

void allOff(float progress, RGBColor* pixels, unsigned short pixelCount, Fx_AnimationParams *params) {
  for(short i=0; i<pixelCount; i++) {
    pixels[i].r = 0;
    pixels[i].g = 0;
    pixels[i].b = 0;
  }
}

void endWithColor(float progress, RGBColor* pixels, unsigned short pixelCount, Fx_AnimationParams *params) {
  // debugPrint("endWithColor got progress: %.2f\n", progress);
  // progressively turn each pixel e.g. black from start to end
  // TODO: could fade in a bit
  unsigned short lastIndex = (pixelCount * progress);
  for(unsigned short i=0; i<=lastIndex; i++) {
    pixels[i].r = params->endColor.r;
    pixels[i].g = params->endColor.g;
    pixels[i].b = params->endColor.b;
  }
}

void bounce(float progress, RGBColor* pixels, unsigned short pixelCount, Fx_AnimationParams *params) {
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
  // debugPrint("bounce: position: %d, ledToLight %d\n", position, ledToLight);
  for(unsigned short i=0; i<pixelCount; i++) {
    unsigned short distance = (unsigned int)ledToLight - i;
    if (distance == 0) {
      pixels[i].r = params->endColor.r;
      pixels[i].g = params->endColor.g;
      pixels[i].b = params->endColor.b;
    } else if (distance <= 3) {
      pixels[i].r = params->endColor.r / distance;
      pixels[i].g = params->endColor.g / distance;
      pixels[i].b = params->endColor.b / distance;
    } else {
      pixels[i].r = 0;
      pixels[i].g = 0;
      pixels[i].b = 0;
    }
    // debugPrint("%d: %d,%d,%d; ", i, pixels[i].r, pixels[i].g, pixels[i].b);
  }
  // debugPrint("\n");
}

