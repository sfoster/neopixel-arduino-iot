#include "Fx_Types.h"
#include "Fx_Helpers.h"
#include "Fx_Controller.h"
#include "Fx_Animations.h"

Fx_Controller fxController;

void Fx_Controller_Reset() {
  fxController.lastClipIndex = -1;
  fxController.previousMillis = fxController.now = millis();
  RGBColor pixel;
  for (byte i=0; i<fxController.clipCount; i++) {
    pixel = fxController.foregroundPixels[i];
    pixel.r = pixel.g = pixel.b = 0;
    pixel = fxController.backgroundPixels[i];
    pixel.r = pixel.g = pixel.b = 0;
  }
}

void Fx_Controller_Init() {
  fxController.pixelCount = NUM_PIXELS;
  fxController.clipCount = MAX_FX_CLIPS;

  //allocate memory for each clip in the array
  fxController.clips[MAX_FX_CLIPS] = (Fx_Controller_Clip*) malloc( sizeof(Fx_Controller_Clip) );

  Fx_Controller_Reset();
}

// onFrame / perFrame / updateForFrame??
void Fx_Controller_updateTimeline(unsigned long now) {
  fxController.now = now;
  if ((fxController.previousMillis > fxController.now) ||
      fxController.previousMillis == 0) {
    // first update, or maybe overflow happened
    fxController.previousMillis = fxController.now;
  }

  // go over the clips list
  // prune out elapsed clips, re-start repeating clips
  for (byte i=0; i<=fxController.lastClipIndex; i++) {
    Fx_Controller_Clip *clip = fxController.clips[i];
    unsigned long elapsed = fxController.now - clip->startTime;

    //debugPrint("updateTimeline clip at index %d: elapsed: %li, duration: %li\n", i, elapsed, clip->duration);

    Assert(clip->startTime > 0);
    Assert(clip->repeat >= 0);

    if (elapsed >= (unsigned long)clip->duration) {
      debugPrint("updateTimeline: animation complete, repeat: %ld\n", clip->repeat);
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
    //debugPrint("elapsed: %ld, duration: %d, progress: %.2f\n", elapsed, clip->duration, progress);
    if (clip->isForeground) {
      //debugPrint("calling foreground animateFn with progress: %.2f\n", progress);
      (*clip->animateFn)(progress,
                         fxController.foregroundPixels,
                         fxController.pixelCount,
                         &clip->params);
    } else {
      //debugPrint("calling animateFn with progress: %.2f\n", progress);
      (*clip->animateFn)(progress,
                         fxController.backgroundPixels,
                         fxController.pixelCount,
                         &clip->params);
    }
  }
  fxController.previousMillis = fxController.now;
}

void Fx_Controller_AddClip(Fx_Controller_Clip *clip) {
  debugPrint("Fx_Controller_AddClip, lastClipIndex: %d\n", fxController.lastClipIndex);
  byte insertIndex = 1 + fxController.lastClipIndex;
  if (insertIndex >= fxController.clipCount) {
    // if we filled the array of clips, just replace the last one
    insertIndex = fxController.lastClipIndex;
  }
  clip->startTime = fxController.now;
  debugPrint("Adding clip at index: %d, startTime: %ld\n", insertIndex, clip->startTime);
  fxController.clips[insertIndex] = clip;
  fxController.lastClipIndex = insertIndex;
  debugPrint("Fx_Controller_AddClip at lastClipIndex result: %d\n", fxController.lastClipIndex);
}

void Fx_Controller_RemoveClipAtIndex(byte removeIdx) {
  removeIdx = clamp(removeIdx, 0, fxController.clipCount - 1);
  for (byte i = removeIdx+1; i <= fxController.lastClipIndex; i++) {
    fxController.clips[i -1] = fxController.clips[i];
  }
  fxController.lastClipIndex--;
  debugPrint("removeClipAtIndex remove from: %d, lastClipIndex: %d\n", removeIdx, fxController.lastClipIndex);
}

void Fx_Controller_FillForeground(byte value) {
  for(unsigned int i=0; i<fxController.pixelCount; i++) {
    RGBColor pixel = fxController.foregroundPixels[i];
    pixel.r = pixel.g = pixel.b = value;
  }
}

void Fx_Controller_FillBackground(byte value) {
  for(unsigned int i=0; i<fxController.pixelCount; i++) {
    RGBColor pixel = fxController.backgroundPixels[i];
    pixel.r = pixel.g = pixel.b = value;
  }
}

void Fx_Define_Clips() {
  // some predefined clips
  blinkRed_clip.animateFn = &colorBlink;
  blinkRed_clip.isForeground = false;
  blinkRed_clip.duration = 1000UL;
  blinkRed_clip.repeat = 3L;
  blinkRed_clip.params = toRedParams;

  blinkBlue_clip.animateFn = &colorBlink;
  blinkBlue_clip.isForeground = false;
  blinkBlue_clip.duration = 1000UL;
  blinkBlue_clip.repeat = 3L;
  blinkBlue_clip.params = toBlueParams;

  blinkGreen_clip.animateFn = &colorBlink;
  blinkGreen_clip.isForeground = false;
  blinkGreen_clip.duration = 1000UL;
  blinkGreen_clip.repeat = 3L;
  blinkGreen_clip.params = toGreenParams;
}
