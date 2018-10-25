#ifndef FX_HELPERS_H
#define FX_HELPERS_H
// shared helpers

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

void dumpColor(CRGB color) {
  Serial.print(color.r);
  Serial.print(",");
  Serial.print(color.g);
  Serial.print(",");
  Serial.println(color.b);
}

void dumpClip(Anim_Clip* clip) {
  Serial.print("clip, anim: ");
  Serial.print(clip->anim);
  Serial.print(": ");
  Serial.print(clip->startColor.h);
  Serial.print(",");
  Serial.print(clip->startColor.s);
  Serial.print(",");
  Serial.print(clip->startColor.v);
  Serial.print(" - ");
  Serial.print(clip->endColor.h);
  Serial.print(",");
  Serial.print(clip->endColor.s);
  Serial.print(",");
  Serial.println(clip->endColor.v);

  Serial.print("duration: ");
  Serial.print(clip->duration);
  Serial.print(", repeat: ");
  Serial.print(clip->repeat);
  Serial.println(";");
}


float clampFloat(float num, float lbound, float ubound) {
  if (num < lbound) {
    num = lbound;
  }
  if (num > ubound) {
    num = ubound;
  }
  return num;
}

int asciiToInt(char c) {
  if (c >= 48 && c <= 57) {
    return c-48;
  }
  if (c >= 65 && c <= 70) {
    return c - 55;
  }
  if (c >= 97 && c <= 102) {
    return c - 87;
  }
  return 0;
}

CRGB hexColorStringToRGBStruct(char* hexStr) {
  // expects an array with 7 items: "#fe01ba"
  // 0:48 -> 9:58
  // A:65 -> F:70
  // a:97 -> f:102
  CRGB color;
  color.r = ((unsigned char)asciiToInt(hexStr[1]) << 4) + asciiToInt(hexStr[2]);
  color.g = ((unsigned char)asciiToInt(hexStr[3]) << 4) + asciiToInt(hexStr[4]);
  color.b = ((unsigned char)asciiToInt(hexStr[5]) << 4) + asciiToInt(hexStr[6]);
  return color;
}

#endif
