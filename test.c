#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "./led-arch/config.h"
#include "./led-arch/lib/Fx_Types.h"
#include "./led-arch/lib/Fx_Helpers.h"

#include "./led-arch/lib/Fx_Controller.cpp"
#include "./led-arch/lib/Fx_Animations.cpp"

// =========================================================

void sigint_handler(int sig) {
  debugPrint("%s\n", "killing process: ");
  exit(0);
}

void incrementStartR(Fx_AnimationParams *params)
{
  debugPrint("incrementStartR, got startColor.r: %d\n", params->startColor.r);
  params->startColor.r++;
  debugPrint("incrementStartR, now startColor.r: %d\n", params->startColor.r);
}

void decrementRepeat(Fx_Controller_Clip *clip)
{
  debugPrint("decrementRepeat, got repeat: %ld\n", clip->repeat);
  clip->repeat--;
  debugPrint("decrementRepeat, now repeat: %ld\n", clip->repeat);
}

int main(void) {

  // starting at time = 0 will never happen and complicates things!
  delay(123);
  debugPrint("Starting main\n");
  signal(SIGINT, sigint_handler);

  Fx_Controller_Init();
  Fx_Define_Clips();

  Fx_Controller_Clip repeating_clip = blinkRed_clip;
  repeating_clip.repeat = LONG_MAX;
  Fx_Controller_AddClip(&repeating_clip);

  Fx_Controller_Clip short_clip = blinkBlue_clip;
  short_clip.repeat = 0;
  Fx_Controller_AddClip(&short_clip);

  // debugPrint("main: blinkRed_clip repeat: %ld\n", blinkRed_clip.repeat);
  // debugPrint("main: new_clip repeat: %ld\n", new_clip.repeat);
  // debugPrint("main: new_clip duration: %ld\n", new_clip.duration);

  for (int itv = 0; itv < 200; itv++) {
    Fx_Controller_updateTimeline(millis());
    debugPrint("main: pixel.r: %d\n", fxController.foregroundPixels[6].r);
    delay(16);
  }
  return 0;
}
