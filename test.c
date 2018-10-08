#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include "./led-arch/config.h"
#include "./led-arch/lib/Fx_Types.h"
#include "./led-arch/lib/Fx_Helpers.h"

#include "./led-arch/lib/Fx_Animations.cpp"
#include "./led-arch/lib/Fx_Controller.cpp"
#include "./led-arch/lib/Fx_AppStates.cpp"

RunState inputState;

// =========================================================

int parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}
void printMemoryused(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    debugPrint("Memory used: %d\n", result);
}
// =========================================================

void sigint_handler(int sig) {
  debugPrint("%s\n", "killing process: ");
  exit(0);
}


void setup() {
  delay(123);
  Fx_Controller_Init();

  colorBlink(0.00, fxController.foregroundPixels, NUM_PIXELS, &toBlueParams);
  colorBlink(0.10, fxController.foregroundPixels, NUM_PIXELS, &toBlueParams);

  nextState = Connecting;
  int frameItv;
  for (frameItv = 0; frameItv < 10; frameItv++) {
    Fx_updateState();
    Fx_Controller_updateTimeline(millis());
    debugPrint("setup: at now: %ld, pixel.rgb: %d,%d,%d\n", now_ms,
               fxController.backgroundPixels[6].r, fxController.backgroundPixels[6].g, fxController.backgroundPixels[6].b);
    delay(100);
  }
  nextState = Running;
}

int main(void) {
  debugPrint("Starting main\n");
  printMemoryused();
  signal(SIGINT, sigint_handler);

  setup();
  debugPrint("/setup\n");
  printMemoryused();
  delay(123);

  Fx_Controller_AddClip(&colorBlink, false, 1000UL, LONG_MAX, toRedParams);
  Fx_Controller_AddClip(&colorBlink, true, 1000UL, LONG_MAX, toBlueParams);

  // // process input
  int frameItv;
  for (frameItv = 0; frameItv < 10; frameItv++) {
    Fx_updateState();
    Fx_Controller_updateTimeline(millis());
    debugPrint("main: pixel.rgb: %d,%d,%d\n",
               fxController.backgroundPixels[6].r, fxController.backgroundPixels[6].g, fxController.backgroundPixels[6].b);
    delay(100);
  }
  printMemoryused();

  while(1) {
    Fx_updateState();

    // enqueue a dimming animation
    Fx_Controller_AddClip(
      &endWithColor, false, 1000UL, 0, toBlackParams);

    Fx_Controller_updateTimeline(millis());
    debugPrint("main: pixel.rgb: %d,%d,%d\n",
               fxController.backgroundPixels[6].r, fxController.backgroundPixels[6].g, fxController.backgroundPixels[6].b);
    delay(100);

    Fx_updateState();
    Fx_Controller_updateTimeline(millis());
    debugPrint("main: pixel.rgb: %d,%d,%d\n",
               fxController.backgroundPixels[6].r, fxController.backgroundPixels[6].g, fxController.backgroundPixels[6].b);
    delay(100);

    Fx_updateState();
    Fx_Controller_AddClip(
      &colorBlink, false, 1000UL, INFINITE_REPEATS, toBlueParams);
    Fx_Controller_updateTimeline(millis());
    debugPrint("main: pixel.rgb: %d,%d,%d\n",
               fxController.backgroundPixels[6].r, fxController.backgroundPixels[6].g, fxController.backgroundPixels[6].b);
    delay(100);
    printMemoryused();
  }
  // switch (inputState.topic) {
  //   case topic_game_start:
  //     debugPrint("Current topic: topic_game_start\n");
  //     break;
  //   default:
  //     debugPrint("What inputState.topic is this?\n");
  // }

  // RGBColor color;
  // color = hexColorStringToRGBStruct("#606060");
  // debugPrint("%s becomes: r: %d, g: %d, b: %d\n", "#606060", color.r, color.g, color.b);

  // color = hexColorStringToRGBStruct("#ff0000");
  // debugPrint("%s becomes: r: %d, g: %d, b: %d\n", "#ff0000", color.r, color.g, color.b);

  // color = hexColorStringToRGBStruct("#FfCcAa");
  // debugPrint("%s becomes: r: %d, g: %d, b: %d\n", "#FfCcAa", color.r, color.g, color.b);

  // color = hexColorStringToRGBStruct("#aa5567");
  // debugPrint("%s becomes: r: %d, g: %d, b: %d\n", "#aa5567", color.r, color.g, color.b);
  return 0;
}
