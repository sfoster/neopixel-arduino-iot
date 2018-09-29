#include "../config.h"
#include "Fx_Types.h"
#include "Fx_Helpers.h"
#include "Fx_Animations.h"

// -----------------------------------------
// Animation functions
//  take a value of 0-1 representing the %age of duration elapsed, and update the pixels buffer
//  with the r,g,b values for each pixel

void colorBlink(float progress, RGBColor* pixels, int pixelCount, Fx_AnimationParams *params) {
  float pcentOn;
  debugPrint("colorBlink got progress: %.2f\n", progress);
  int rRange = params->endColor.r - params->startColor.r;
  int gRange = params->endColor.g - params->startColor.g;
  int bRange = params->endColor.b - params->startColor.b;

  if (progress <= 0.5f) {
    pcentOn = progress * 2.0f;
  } else {
    pcentOn = (1 - progress) * 2.0f;
  }
  for(int i=0; i<NUM_PIXELS; i++) {
    pixels[i].r = params->startColor.r  + (rRange * pcentOn);
    pixels[i].g = params->startColor.g  + (gRange * pcentOn);
    pixels[i].b = params->startColor.b  + (bRange * pcentOn);
    debugPrint("%d: %d,%d,%d; ", i, pixels[i].r, pixels[i].g, pixels[i].b);
  }
  debugPrint("\n");
}

void allOff(float progress, RGBColor* pixels, int pixelCount, Fx_AnimationParams *params) {
  debugPrint("allOff got progress: %.2f", progress);
  for(int i=0; i<pixelCount; i++) {
    pixels[i].r = 0;
    pixels[i].g = 0;
    pixels[i].b = 0;
  }
}

void endWithColor(float progress, RGBColor* pixels, int pixelCount, Fx_AnimationParams *params) {
  debugPrint("endWithColor got progress: %.2f", progress);
  // progressively turn each pixel e.g. black from start to end
  // TODO: could fade in a bit
  unsigned int lastIndex = (pixelCount * progress);
  for(int i=0; i<=lastIndex; i++) {
    pixels[i].r = params->endColor.r;
    pixels[i].g = params->endColor.g;
    pixels[i].b = params->endColor.b;
  }
}

void bounce(float progress, RGBColor* pixels, int pixelCount, Fx_AnimationParams *params) {
  debugPrint("bounce got progress: %.2f", progress);
  // animate a dot forward then backwards.
  byte iterations = 2;
  int lastIndex = pixelCount - 1;
  // get a position into the total length (iterations * number of pixels)
  int position = (float)lastIndex * (float)iterations * progress;
  // the even iterations are going backwards...
  int ledToLight = position % (lastIndex * 2);
  if (ledToLight > lastIndex) {
    ledToLight = lastIndex - (ledToLight - lastIndex);
  }
  debugPrint("bounce: position: %d, ledToLight %d\n", position, ledToLight);
  for(int i=0; i<pixelCount; i++) {
    int distance = (unsigned int)ledToLight - i;
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
    debugPrint("%d: %d,%d,%d; ", i, pixels[i].r, pixels[i].g, pixels[i].b);
  }
  debugPrint("\n");
}

// ------------------------
// Some pre-defined animation params

Fx_AnimationParams defaultParams = {{0, 0, 0},
                                    {0, 0, 0},
                                    1,
                                    false};

Fx_AnimationParams toBlueParams =  {{0, 0, 0},
                                    {0, 0, 255},
                                    1,
                                    false};

Fx_AnimationParams toRedParams =   {{0, 0, 0},
                                    {255, 0, 0},
                                    1,
                                    false};

Fx_AnimationParams toGreenParams = {{0, 0, 0},
                                    {0, 255, 0},
                                    1,
                                    false};

