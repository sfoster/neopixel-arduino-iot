#include "Fx_Types.h"
#include "Fx_AppStates.h"
#include "Fx_Animations.h"
#include "Fx_Controller.h"

void Fx_updateState() {
  switch (nextState) {
    case Off:
      if (nextState != currentState) {
        debugPrint("Going to Off state\n");

        // enqueue a dimming animation
        // Fx_Controller_AddClip(
        //   &endWithColor, false, 1000UL, 0, toBlackParams);
      }
      currentState = nextState;
      break;
    case Connecting:
      if (nextState != currentState) {
        debugPrint("Going to Connecting state\n");
        // enqueue a connecting animation
        // Fx_Controller_Reset();
        // Fx_Controller_AddClip(
        //   &colorBlink, false, 1000UL, INFINITE_REPEATS, toBlueParams);
      }
      currentState = nextState;
      break;
    case Running:
      if (nextState != currentState) {
        debugPrint("Going to Running state\n");
        // Fx_Controller_Reset();
        // Fx_Controller_AddClip(
        //   &colorBlink, false, 1000UL, INFINITE_REPEATS, toGreenParams);
      }
      currentState = nextState;
      break;
    case Error:
      if (nextState != currentState) {
        debugPrint("Going to Error state\n");
        // enqueue a error animation
      }
      currentState = nextState;
      break;
    default:
      debugPrint("What state?\n");
  }
}
