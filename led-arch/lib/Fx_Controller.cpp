#include "Fx_Types.h"
#include "Fx_Helpers.h"
#include "Fx_Controller.h"
#include "Fx_Animations.h"

Fx_Controller fxController;
// allocate memory for each clip in the array
Fx_Controller_Clip clipSlots[MAX_FX_CLIPS];

void Fx_Controller_Reset() {
  fxController.lastClipIndex = -1;
  fxController.previousMillis = fxController.now = millis();
  RGBColor pixel;
  for (unsigned char i=0; i<fxController.clipCount; i++) {
    pixel = fxController.foregroundPixels[i];
    pixel.r = pixel.g = pixel.b = 0;
    pixel = fxController.backgroundPixels[i];
    pixel.r = pixel.g = pixel.b = 0;
  }
}

void Fx_Controller_Init() {
  fxController.pixelCount = NUM_PIXELS;
  fxController.clipCount = MAX_FX_CLIPS;

  // populate.clips with pointers to (static) memory allocated to clips
  for(unsigned char i=0; i<MAX_FX_CLIPS; i++) {
    fxController.clips[i] = &clipSlots[i];
  }

  Fx_Controller_Reset();
}

void Fx_Controller_updateTimeline(unsigned long now) {
  fxController.now = now;
  if ((fxController.previousMillis > fxController.now) ||
      fxController.previousMillis == 0) {
    // first update, or maybe overflow happened
    fxController.previousMillis = fxController.now;
  }

  // go over the clips list
  // prune out elapsed clips, re-start repeating clips
  for (unsigned char i=0; i<=fxController.lastClipIndex; i++) {
    Fx_Controller_Clip *clip = fxController.clips[i];
    unsigned long elapsed = fxController.now - clip->startTime;

    //debugPrint("updateTimeline clip at index %d: elapsed: %li, duration: %li\n", i, elapsed, clip->duration);

    Assert(clip->startTime > 0);
    Assert(clip->repeat >= 0);

    if (elapsed >= (unsigned long)clip->duration) {
      debugPrint("updateTimeline: animation complete, elapsed: %ld, repeat: %ld\n", elapsed, clip->repeat);
      clip->startTime = fxController.now;
      clip->repeat--;
      elapsed = elapsed - clip->duration;
      debugPrint("updateTimeline: decremented repeat %ld\n", clip->repeat);
    } else {
      //debugPrint("updateTimeline: didnt exceed duration\n");
    }
    if (clip->repeat < 0) {
      debugPrint("updateTimeline: clip is out of repeats, removing it from index: %d\n", i);
      Fx_Controller_RemoveClipAtIndex(i);
      i--;
      // reset the buffers to avoid lingering values from removed animation
      if (clip->isForeground) {
        Fx_Controller_FillForeground(0);
      } else {
        Fx_Controller_FillBackground(0);
      }
      continue;
    } else {
      //debugPrint("updateTimeline: can still repeat: %ld\n", clip->repeat);
    }
    // update the tracks with their new values
    float progress = 0.0;
    if (clip->duration > 0) {
      if (elapsed > 0) {
        progress = (float)elapsed / (float)clip->duration;
        //debugPrint("elapsed is > 0.0: %ld, progress: %.2f\n", elapsed, progress);
      }
    } else {
      // avoid divide by zero with duration: 0
      progress = 1.0;
    }
    progress = clampFloat(progress, 0.0, 1.0);
    Assert(progress <= 1 && progress >= 0);

    //debugPrint("elapsed: %ld, duration: %d, progress: %.2f\n", elapsed, clip->duration, progress);
    if (clip->isForeground) {
      // debugPrint("calling foreground animateFn with progress: %.2f\n", progress);
      (*clip->animateFn)(progress,
                         fxController.foregroundPixels,
                         fxController.pixelCount,
                         &clip->params);
    } else {
      // debugPrint("calling background animateFn with progress: %.2f\n", progress);
      (*clip->animateFn)(progress,
                         fxController.backgroundPixels,
                         fxController.pixelCount,
                         &clip->params);
    }
  }
  fxController.previousMillis = fxController.now;
}

void Fx_Controller_AddClip(AnimateFnPointer animateFn,
                     bool isForeground,
                     long duration,
                     long repeat,
                     Fx_AnimationParams params) {

  debugPrint("Fx_Controller_AddClip, lastClipIndex: %d\n", fxController.lastClipIndex);
  unsigned char insertIndex = 1 + fxController.lastClipIndex;
  if (insertIndex >= fxController.clipCount) {
    // if we filled the array of clips, just replace the last one
    insertIndex = fxController.lastClipIndex;
  }
  Assert(fxController.now > 0);

  // we have a fixed number of slots for clips and just re-use (overwrite) them
  Fx_Controller_Clip *clip = fxController.clips[insertIndex];
  clip->animateFn = animateFn;
  clip->isForeground = isForeground;
  clip->duration = duration;
  clip->repeat = repeat;
  clip->params = params;
  clip->startTime = fxController.now;

  debugPrint("Adding clip at index: %d, startTime: %ld\n", insertIndex, clip->startTime);
  fxController.lastClipIndex = insertIndex;
  debugPrint("Fx_Controller_AddClip at lastClipIndex result: %d\n", fxController.lastClipIndex);
  debugPrint("Fx_Controller_AddClip, startTime: %ld, duration: %ld, repeat: %ld\n",
      fxController.clips[insertIndex]->startTime,
      fxController.clips[insertIndex]->duration,
      fxController.clips[insertIndex]->repeat);
}

void Fx_Controller_RemoveClipAtIndex(unsigned char removeIdx) {
  // clips is a fixed-length array,
  // so memory is not freed when removing a clip, we just move the lastClipIndex
  // to allow it to be reused
  removeIdx = clamp(removeIdx, 0, fxController.clipCount - 1);
  for (unsigned char i = removeIdx+1; i <= fxController.lastClipIndex; i++) {
    fxController.clips[i -1] = fxController.clips[i];
  }
  fxController.lastClipIndex--;
  debugPrint("removeClipAtIndex remove from: %d, lastClipIndex: %d\n", removeIdx, fxController.lastClipIndex);
}

void Fx_Controller_FillForeground(unsigned char value) {
  for(unsigned short i=0; i<fxController.pixelCount; i++) {
    RGBColor pixel = fxController.foregroundPixels[i];
    pixel.r = pixel.g = pixel.b = value;
  }
}

void Fx_Controller_FillBackground(unsigned char value) {
  for(unsigned short i=0; i<fxController.pixelCount; i++) {
    RGBColor pixel = fxController.backgroundPixels[i];
    pixel.r = pixel.g = pixel.b = value;
  }
}

