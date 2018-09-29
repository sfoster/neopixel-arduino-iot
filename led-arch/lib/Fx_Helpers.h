#ifndef FX_HELPERS_H
#define FX_HELPERS_H
// shared helpers

#include "../config.h"

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

#if DEBUG
  #define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#else
  #define Assert(Expression)
#endif

#endif
