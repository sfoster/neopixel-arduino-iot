#ifndef FASTLED_I2C_H

enum Animation { None = 0, AllOff, Blink, Pulse, Race, Cylon };

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

float clampFloat(float num, float lbound, float ubound) {
  if (num < lbound) {
    num = lbound;
  }
  if (num > ubound) {
    num = ubound;
  }
  return num;
}


#define FASTLED_I2C_H
#endif
