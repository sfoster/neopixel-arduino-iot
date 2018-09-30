"use strict";
function Fx_AnimationParams(startColor={r:0,g:0,b:0}, endColor={r:0,g:0,b:0},
                            initialDirection=1, isForeground=false)
{
  this.startColor = startColor;
  this.endColor = endColor;
  this.initialDirection = initialDirection;
  this.isForeground = isForeground;
}
const Fx_Animations = {};

// -----------------------------------------------------
// animations

function allOff(progress, pixels, pixelCount, /* AnimationParams */params) {
  for(let i=0; i<pixelCount; i++) {
    pixels[i].r = 0;
    pixels[i].g = 0;
    pixels[i].b = 0;
  }
}

function colorBlink(progress, pixels, pixelCount, /* AnimationParams */params) {
  let pcentOn;
  let rRange = params.endColor.r - params.startColor.r;
  let gRange = params.endColor.g - params.startColor.g;
  let bRange = params.endColor.b - params.startColor.b;

  if (progress <= 0.5) {
    pcentOn = progress * 2;
  } else {
    pcentOn = (1 - progress) * 2;
  }
  for(let i=0; i<NUM_PIXELS; i++) {
    pixels[i].r = params.startColor.r  + (rRange * pcentOn);
    pixels[i].g = params.startColor.g  + (gRange * pcentOn);
    pixels[i].b = params.startColor.b  + (bRange * pcentOn);
  }
}

function endWithColor(progress, pixels, pixelCount, /* AnimationParams */params) {
  // progressively turn each pixel black from start to end
  // TODO: could fade in a bit
  let lastPixelIndex = pixelCount - 1;
  let ubound = Math.floor(lastPixelIndex * progress);
  console.log("endColor, got direction: ", params.initialDirection);
  for(let i=0; i < ubound; i++) {
    let index = i;
    if (params.initialDirection < 0) {
      index = lastPixelIndex - index;
    }
    console.assert(pixels[index], "endWithColor tried to write to pixel at i:" + i);
    pixels[index].r = params.endColor.r;
    pixels[index].g = params.endColor.g;
    pixels[index].b = params.endColor.b;
  }
}

function bounce(progress, pixels, pixelCount, /* AnimationParams */params) {
  // animate a dot forward then backwards.
  let iterations = 2;
  let brightness = 255;
  let lastIndex = pixelCount - 1;
  // get a position into the total length (iterations * number of pixels)
  let position = Math.floor(lastIndex * iterations * progress);
  // the even iterations are going backwards...
  let ledToLight = position % (lastIndex * 2);
  if (ledToLight > lastIndex) {
    ledToLight = lastIndex - (ledToLight - lastIndex);
  }
  // console.log(`bounce: progress: ${progress}, position: ${position}, ledToLight: ${ledToLight}`);
  for(let i=0; i<NUM_PIXELS; i++) {
    let distance = Math.abs(ledToLight - i);
    let value;
    if (distance == 0) {
      value = brightness;
    } else if (distance <= 3) {
      value = brightness / distance;
      // console.log(`${ledToLight}, i: ${i}, distance: ${distance}, value: ${value}`);
    } else {
      value = 0;
    }
    // pixels[i].r = 0;
    pixels[i].g = value;
    // pixels[i].b = 0;
  }
}
