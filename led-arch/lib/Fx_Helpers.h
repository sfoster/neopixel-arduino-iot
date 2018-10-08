#ifndef FX_HELPERS_H
#define FX_HELPERS_H
// shared helpers

#include "../config.h"
#include "./Fx_Types.h"

void _noPrint(const char *fmt, ...);

#ifdef DEBUG
  #include <stdio.h>
  #include <stdarg.h>

  void debugPrint(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
  #ifdef HardwareSerial_h
    char buffer[100]; // swag
    vsprintf(buffer, fmt, args);
    Serial.print(buffer);
  #else
    vprintf(fmt, args);
    va_end(args);
  #endif
  }
#else
  // noop version of debugPrint
  void debugPrint(const char *fmt, ...) {}
#endif

int clamp(int num, int lbound, int ubound) {
  if (num < lbound) {
    num = lbound;
  }
  if (num > ubound) {
    num = ubound;
  }
  return num;
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

#if DEBUG
  #define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#else
  #define Assert(Expression)
#endif

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

RGBColor hexColorStringToRGBStruct(char* hexStr) {
  // expects an array with 7 items: "#fe01ba"
  // 0:48 -> 9:58
  // A:65 -> F:70
  // a:97 -> f:102
  RGBColor color;
  color.r = ((byte)asciiToInt(hexStr[1]) << 4) + asciiToInt(hexStr[2]);
  color.g = ((byte)asciiToInt(hexStr[3]) << 4) + asciiToInt(hexStr[4]);
  color.b = ((byte)asciiToInt(hexStr[5]) << 4) + asciiToInt(hexStr[6]);
  return color;
}

#endif
