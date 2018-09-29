"use strict";

const Fx_Controller_Clip_Count = 6;
var Fx_Controller_PreviousMillis = 0;
var Fx_Controller_Now = 0;

function Fx_Controller(NUMBER_OF_PIXELS) {
  this.NUMBER_OF_PIXELS = NUMBER_OF_PIXELS;
  this.foregroundPixels = new Array(NUMBER_OF_PIXELS);
  this.backgroundPixels = new Array(NUMBER_OF_PIXELS);
  this.reset();
}

Fx_Controller.prototype.reset = function() {
  this.clips = new Array(Fx_Controller_Clip_Count);
  for (let i=0; i<Fx_Controller_Clip_Count; i++) {
    this.clips[i] = null;
  }
  this.lastClipIndex = -1;
  this.now = null;
  for (let i=0; i<NUM_PIXELS; i++) {
    this.foregroundPixels[i] = {r:0,g:0,b:0};
    this.backgroundPixels[i] = {r:0,g:0,b:0};
  }
  console.log("Fx_Controller reset, lastClipIndex: ", this.lastClipIndex, this.clips);
};

Fx_Controller.prototype.start = function() {
  Fx_Controller_Now = millis();
  Fx_Controller_PreviousMillis = millis();
};

// onFrame / perFrame / updateForFrame??
Fx_Controller.prototype.updateTimeline = function() {
  let now = millis();
  if (Fx_Controller_PreviousMillis > now) {
    // overflow happened
    Fx_Controller_PreviousMillis = Fx_Controller_Now = now;
  }
  Fx_Controller_PreviousMillis = Fx_Controller_Now;
  Fx_Controller_Now = now;

  // go over the clips list
  // prune out elapsed clips, re-start repeating clips
  for (let i=0; i<=this.lastClipIndex; i++) {
    let clip = this.clips[i];
    console.log("updating clip, is isForeground", clip.isForeground);
    let fillForeground = false;
    let fillBackground = false;
    if (!clip) {
      console.log("updateTimeline: falsey clip at index: ", i, this.clips);
    }
    let elapsed = now - clip.startTime;
    if (elapsed >= clip.duration) {
      clip.repeat--;
      clip.startTime = now;
    }
    if (clip.repeat < 0) {
      console.log("updateTimeline: clip with animateFn " + clip.animateFn.name + " is out of repeats, removing it from index: " + i);
      this.removeClipAtIndex(i);
      i--;
      // reset the buffers to avoid lingering values from removed animation
      if (clip.isForeground) {
        this._fillForeground(0);
      } else {
        this._fillBackground(0);
      }
      continue;
    }
      // update the tracks with their new values
    let progress = 0;
    if (elapsed > 0) {
      progress = elapsed / clip.duration;
    }
    if (clip.isForeground) {
      clip.animateFn(progress,
                    this.foregroundPixels,
                    this.NUMBER_OF_PIXELS,
                    clip.params);
    } else {
      clip.animateFn(progress,
                    this.backgroundPixels,
                    this.NUMBER_OF_PIXELS,
                    clip.params);
    }
  }
};

Fx_Controller.prototype.addClip = function(clip) {
  let insertIndex = 1 + this.lastClipIndex;
  if (insertIndex >= Fx_Controller_Clip_Count) {
    // if we filled the array of clips, just replace the last one
    insertIndex = this.lastClipIndex;
  }
  this.clips[insertIndex] = clip;
  this.lastClipIndex = insertIndex;
  console.log("addClip: ", this.clips[insertIndex]);
}

Fx_Controller.prototype.removeClipAtIndex = function(removeIdx) {
  removeIdx = clamp(removeIdx, 0, Fx_Controller_Clip_Count - 1);
  let lastIndex = Fx_Controller_Clip_Count - 1;
  this.clips[removeIdx] = null;
  for (let i = removeIdx+1; i <= this.lastClipIndex; i++) {
    this.clips[i -1] = this.clips[i];
  }
  this.lastClipIndex--;
  console.log("removeClipAtIndex result: ", this.lastClipIndex, this.clips);
}

Fx_Controller.prototype._fillForeground = function(value = 0) {
  for(let i=0; i<NUM_PIXELS; i++) {
    let pixel = fxController.foregroundPixels[i];
    pixel.r = pixel.g = pixel.b = value;
  }
};

Fx_Controller.prototype._fillBackground = function(value = 0) {
  for(let i=0; i<NUM_PIXELS; i++) {
    let pixel = fxController.backgroundPixels[i];
    pixel.r = pixel.g = pixel.b = value;
  }
};


function Fx_Controller_Clip(animation, startTime, duration, repeat, params) {
  /*
    a clip is an animation function, with a start time and duration
    the clip tracks the number of times it will repeat
    and passes a set of params into that animation on each call
  */
  this.animateFn = animation.animateFn;
  console.log("Fx_Controller_Clip, got params: ", params);
  // lift up the animation default params to be instance state for this sequence
  this.params = Object.assign({}, animation.defaulParams, params);
  this.duration = duration;
  this.startTime = startTime;
  this.repeat = repeat || 0;
}
