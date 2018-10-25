#ifndef FX_ANIMATIONS_H
#define FX_ANIMATIONS_H
#include "Fx_Helpers.h"
// include "Fx_Helpers.h"

// -----------------------------------------
// Declare Animation functions

void blink(float progress, CRGB pixels[], unsigned short pixelCount, Anim_Clip *clip) {
  CHSV color = { .hue=0, .sat=255, .val=255 };
  if (progress > 0.5) {
    color.hue = 255 - clip->startColor.hue;
    color.val = 255 - clip->startColor.val;
  }
  for(int i = 0; i < pixelCount; i++) {
    pixels[i] = color;
  }
  // addGlitter(8);
}

void fadeToColor(float progress, CRGB pixels[], unsigned short pixelCount, Anim_Clip *clip) {
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
  for(unsigned short i=0; i<pixelCount; i++) {
    pixels[i] = newColor;
  }
}

void colorPulse(float progress, CRGB pixels[], unsigned short pixelCount, Anim_Clip *clip) {
  // debugPrint("colorBlink got progress: %.2f\n", progress);
  float pcentOn;
  short valRange = clip->endColor.val - clip->startColor.val;
  CHSV newColor;

  if (progress <= 0.5f) {
    pcentOn = progress * 2.0f;
  } else {
    pcentOn = (1 - progress) * 2.0f;
  }
  newColor.hue = clip->startColor.hue  + ((clip->endColor.hue - clip->startColor.hue) * pcentOn);
  newColor.sat = clip->startColor.sat  + ((clip->endColor.sat - clip->startColor.sat) * pcentOn);
  newColor.val = clip->startColor.val  + ((clip->endColor.val - clip->startColor.val) * pcentOn);

  for(unsigned short i=0; i<pixelCount; i++) {
    pixels[i] = newColor;
  }
}

void race(float progress, CRGB pixels[], unsigned short pixelCount, Anim_Clip *clip) {
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
  }
}

#endif
