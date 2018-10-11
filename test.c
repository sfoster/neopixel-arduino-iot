#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "./led-arch/config.h"
#include "./led-arch/lib/Fx_Types.h"
#include "./led-arch/lib/Fx_Helpers.h"
#include "./led-arch/lib/hsv.h"

void sigint_handler(int sig) {
  debugPrint("%s\n", "killing process: ");
  exit(0);
}

void printRGB(RGBColor color) {
  debugPrint("r: %d, g: %d, b: %d\n", color.r, color.g, color.b);
}

int main(void) {
  debugPrint("Starting main\n");
  signal(SIGINT, sigint_handler);

  HSVColor color = { .h = 0, .s = 255, .v = 255 };
  printf("hsv: h: %d, s: %d, v: %d\n", color.h, color.s, color.v);
  RGBColor c = HsvToRgb(color);
  printRGB(c);
  return 0;
}
