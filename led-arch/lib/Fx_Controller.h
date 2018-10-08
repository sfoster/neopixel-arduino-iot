#ifndef FX_CONTROLLER_H
#define FX_CONTROLLER_H

#include "../config.h"
#include "Fx_Types.h"
#include "Fx_Animations.h"

void Fx_Controller_Reset();

void Fx_Controller_Init();

void Fx_Controller_updateTimeline(unsigned long);

void Fx_Controller_AddClip(AnimateFnPointer animateFn,
                     bool isForeground,
                     long duration,
                     long repeat,
                     Fx_AnimationParams params);

void Fx_Controller_RemoveClipAtIndex(byte removeIdx);

void Fx_Controller_FillForeground(byte value);

void Fx_Controller_FillBackground(byte value);

#endif
