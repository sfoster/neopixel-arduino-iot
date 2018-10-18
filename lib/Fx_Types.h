#ifndef FX_TYPES_H
#define FX_TYPES_H

#include <limits.h>
static const long INFINITE_REPEATS = LONG_MAX;
#define ZERO_MS 0

#ifndef ARDUINO
#define true 1
#define false 0

typedef int bool;

unsigned long now_ms = ZERO_MS;

unsigned long millis() {
  return now_ms;
}

void delay(unsigned long ms) {
  now_ms += ms;
  usleep((int)ms * 1000);
}

#endif

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

// unused?
typedef char Fx_Name[64];

typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} RGBColor;

typedef struct {
  RGBColor startColor;
  RGBColor endColor;
  int initialDirection;
  bool isForeground;
} Fx_AnimationParams;

typedef void (*AnimateFnPointer)(float progress, RGBColor* pixels, unsigned short pixelCount, Fx_AnimationParams *params);

typedef struct {
  AnimateFnPointer animateFn;
  bool isForeground;
  // lets keep all time values as long for now
  unsigned long startTime;
  unsigned long duration;
  // repeat can go to -1 so needs to be signed
  long repeat;
  Fx_AnimationParams params;
} Fx_Controller_Clip;

typedef struct {
  // could be more than 256
  unsigned short pixelCount;
  // unlikely to be more than 128
  char clipCount;
  // initial value is -1
  char lastClipIndex;

  unsigned long previousMillis;
  unsigned long now;

// Each buffer is an array of pixels
  RGBColor foregroundPixels[NUM_PIXELS];
  RGBColor backgroundPixels[NUM_PIXELS];

  // clips is an array of pointers to Fx_Controller_Clip-s
  Fx_Controller_Clip *clips[MAX_FX_CLIPS];
} Fx_Controller;

#if 0
enum InputTopic {
  topic_game_start,
  topic_connecting,
  topic_connectionfail,
  topic_connected,
  topic_disconnected,
  topic_game_end,
  topic_reset,
  topic_playanim
};
#endif

#if 0
typedef struct {
  Fx_Controller_Clip clipParam;
  char detailsMessage[255];
  enum InputTopic topic;
} RunState;
#endif

#endif
