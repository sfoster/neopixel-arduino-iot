#include "Fx_Helpers.h"
#include "Fx_AppStates.h"
#include "Fx_Animations.h"

void Fx_updateState() {
  switch (nextState) {
    case Off:
      if (nextState != currentState) {
        Serial.println("Going to Off state");

        // enqueue a dimming animation
        Anim_Clip dimmingClip;
        dimmingClip.anim = AllOff;
        dimmingClip.startColor = { .hue=0, .sat=0, .val=0 };
        dimmingClip.endColor = { .hue=0, .sat=0, .val=0 };
        dimmingClip.initialDirection = 1;
        dimmingClip.duration = 1000;
        // repeat can go to -1 so needs to be signed
        dimmingClip.repeat = 0;
        // lets keep all time values as long for now

        // enqueueClip()
      }
      currentState = nextState;
      break;
    case Connecting:
      if (nextState != currentState) {
        Serial.println("Going to Connecting state");
        // enqueue a connecting animation
        // Fx_Controller_Reset();
        // Fx_Controller_AddClip(
        //   &colorBlink, false, 1000UL, INFINITE_REPEATS, toBlueParams);
      }
      currentState = nextState;
      break;
    case Running:
      if (nextState != currentState) {
        Serial.println("Going to Running state");
        // Fx_Controller_Reset();
        // Fx_Controller_AddClip(
        //   &colorBlink, false, 1000UL, INFINITE_REPEATS, toGreenParams);
      }
      currentState = nextState;
      break;
    case Error:
      if (nextState != currentState) {
        Serial.println("Going to Error state");
        // enqueue a error animation
      }
      currentState = nextState;
      break;
    default:
      Serial.println("What state?");
  }
}
