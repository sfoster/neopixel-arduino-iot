#define NUM_PIXELS 7
#define FOO 1

#ifdef HardwareSerial_h
 // my c/macro-fu fails me...
 #ifdef DEBUG
  void debugPrint(char* msg) {
    Serial.print(msg);
  }
 #else
  void debugPrint(char* msg) {}
 #endif
#else
  #include <stdio.h>
  void debugPrint(char* msg) {
    printf("%s", msg);
  }
#endif

typedef void (*funcptr)(double);

typedef struct {
  byte r;
  byte g;
  byte b;
} RBGColor;

// allocate an array of rgb structs, one for each pixel
RBGColor pixelColors[NUM_PIXELS];

enum State {
  start_state,
  off_transition,
  off_state,
  color_fade_transition,
  color_fade_state
};

int clamp(int num, int lbound, int ubound) {
  if (num < lbound) {
    num = lbound;
  }
  if (num > ubound) {
    num = ubound;
  }
  return num;
}
