#ifndef FX_APPSTATES_H
#define FX_APPSTATES_H

enum AppStates { Off, Connecting, Running, Error };
enum AppStates currentState;
enum AppStates nextState;

void Fx_updateState();

#endif
