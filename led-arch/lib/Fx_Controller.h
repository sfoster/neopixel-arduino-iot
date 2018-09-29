#ifndef FX_CONTROLLER_H
#define FX_CONTROLLER_H

#include "../config.h"
#include "Fx_Types.h"

void Fx_Controller_Reset();

void Fx_Controller_Init();

void Fx_Controller_updateTimeline(unsigned long);

void Fx_Controller_AddClip(Fx_Controller_Clip *clip);

void Fx_Controller_RemoveClipAtIndex(byte removeIdx);

void Fx_Controller_FillForeground(byte value);

void Fx_Controller_FillBackground(byte value);

void Fx_Define_Clips();

// -------------------------------------------------
// declare some pre-defined clips

Fx_Controller_Clip blinkRed_clip;
Fx_Controller_Clip blinkBlue_clip;
Fx_Controller_Clip blinkGreen_clip;
Fx_Controller_Clip allOff_clip;

#endif
