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
  this.lastClipIndex = -1;
  this.now = null;
  for (let i=0; i<NUM_PIXELS; i++) {
    this.foregroundPixels[i] = {r:0,g:0,b:0};
    this.backgroundPixels[i] = {r:0,g:0,b:0};
  }
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
    let seq = this.clips[i];
    let elapsed = now - seq.startTime;
    if (elapsed >= seq.duration) {
      seq.repeat--;
      seq.startTime = now;
    }
    if (seq.repeat < 0) {
      this.removeClipAtIndex(i);
      i--;
      continue;
    }
      // update the tracks with their new values
    let progress = 0;
    if (elapsed > 0) {
      progress = elapsed / seq.duration;
    }
    if (seq.isTranslucent) {
      seq.animateFn(progress,
                    this.foregroundPixels,
                    this.NUMBER_OF_PIXELS,
                    seq.params);
    } else {
      seq.animateFn(progress,
                    this.backgroundPixels,
                    this.NUMBER_OF_PIXELS,
                    seq.params);
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

Fx_Controller.prototype.removeClipAtIndex = function(idx) {
  idx = clamp(idx, 0, Fx_Controller_Clip_Count - 1);
  this.clips.splice(idx, 1);
  console.log("removeClipAtIndex result: ", idx, this.clips);
}

function Fx_Controller_Clip(animation, startTime, duration, repeat, params) {
  /*
    a clip is an animation function, with a start time and duration
    the clip tracks the number of times it will repeat
    and passes a set of params into that animation on each call
  */
  this.animateFn = animation.animateFn;
  // lift up the animation default params to be instance state for this sequence
  this.params = params || deepClone(animation.defaulParams);
  this.duration = duration;
  this.startTime = startTime;
  this.repeat = repeat || 0;
}
