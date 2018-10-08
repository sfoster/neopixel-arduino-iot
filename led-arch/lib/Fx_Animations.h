#ifndef FX_ANIMATIONS_H
#define FX_ANIMATIONS_H
#include "../config.h"
#include "Fx_Types.h"
#include "Fx_Helpers.h"

// -----------------------------------------
// Declare Animation functions

void colorBlink(float progress, RGBColor* pixels, int pixelCount, Fx_AnimationParams *params);

void allOff(float progress, RGBColor* pixels, int pixelCount, Fx_AnimationParams *params);

void endWithColor(float progress, RGBColor* pixels, int pixelCount, Fx_AnimationParams *params);

void bounce(float progress, RGBColor* pixels, int pixelCount, Fx_AnimationParams *params);

// ------------------------
// Define some pre-defined colors

static const RGBColor Black = { 0, 0, 0 };
static const RGBColor White = { 255, 255, 255 };
static const RGBColor Red = { 255, 0, 0 };
static const RGBColor Blue = { 0, 0, 255 };
static const RGBColor Green = { 0, 255, 0 };
static const RGBColor Pink = { 255, 154, 154 };

// ------------------------
// Declare pre-defined animation params

Fx_AnimationParams defaultParams = {{0, 0, 0},
                                    {0, 0, 0},
                                    1,
                                    false};

Fx_AnimationParams toBlueParams =  {{0, 0, 0},
                                    {0, 0, 255},
                                    1,
                                    false};

Fx_AnimationParams toRedParams =   {{0, 0, 0},
                                    {255, 0, 0},
                                    1,
                                    false};

Fx_AnimationParams toGreenParams = {{0, 0, 0},
                                    {0, 255, 0},
                                    1,
                                    false};

Fx_AnimationParams toBlackParams = {{0, 0, 0},
                                    {0, 0, 0},
                                    1,
                                    false};

#endif
