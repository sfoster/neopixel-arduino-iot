#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
// include <FastLED.h>

#include "./led-arch/config.h"

/// Pre-defined hue values for HSV objects
typedef enum {
    HUE_RED = 0,
    HUE_ORANGE = 32,
    HUE_YELLOW = 64,
    HUE_GREEN = 96,
    HUE_AQUA = 128,
    HUE_BLUE = 160,
    HUE_PURPLE = 192,
    HUE_PINK = 224
} HSVHue;

typedef struct CHSV {
  unsigned char h;
  unsigned char s;
  unsigned char v;
} CHSV;

enum Animation { None = 0, AllOff, Cylon, Blink, Pulse };

typedef struct Anim_Clip {
  enum Animation anim;
  CHSV startColor;
  CHSV endColor;
  char initialDirection;
  unsigned int duration;
  // repeat can go to -1 so needs to be signed
  short repeat;
  // lets keep all time values as long for now
  unsigned long startTime;
} Anim_Clip;

Anim_Clip currentClip;
Anim_Clip currentLoop;

void sigint_handler(int sig) {
  printf("%s\n", "killing process: ");
  exit(0);
}

void loop(void) {
  Anim_Clip clip;
  if (currentClip.anim != None) {
    clip = currentClip;
  } else if (currentLoop.anim != None) {
    clip = currentLoop;
    clip.repeat = SHRT_MAX;
  } else {
    return;
  }
  switch (clip.anim) {
    case AllOff:
      printf("%s\n", "AllOff animation");
      break;
    case Blink:
      printf("%s\n", "Blink animation");
      break;
    case Cylon:
      printf("%s\n", "Cylon animation");
      break;
    case Pulse:
      printf("%s\n", "Pulse animation");
      break;
    default:
      printf("%s: %d\n", "Another animation", clip.anim);
      break;
  }
}

int main(void) {
  printf("%s\n", "Starting main");
  signal(SIGINT, sigint_handler);

  CHSV startColor = { .h = 0, .s = 255, .v = 255 };
  CHSV endColor = { .h = 0, .s = 255, .v = 255 };
  printf("%s\n", "Defined colors");

  Anim_Clip clip = {
    .anim = Cylon,
    .startColor = startColor,
    .endColor = endColor,
    .duration = 1000,
    .initialDirection = -1,
    .repeat = SHRT_MAX
  };
  printf("%s\n", "Defined clip");

  // printf("hsv: h: %d, s: %d, v: %d\n", startColor.h, startColor.s, startColor.v);
  printf("sizeof: %ld\n", (sizeof(clip)));

  // while(1) {
  //   loop();
  // }
  // CRGB rgb = hsv2rgb_rainbow(color);
  // printRGB(rgb);
  return 0;
}

#if 0
void bounce(float progress, CHSV* pixels, unsigned short pixelCount, Anim_Clip *clip) {
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
#endif

