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

function rainbow(progress, pixels, pixelCount, params) {
  // ignore startColor, endColor we'll do red, orange, yellow, green, blue, indigo and violet
  let startHSV = { h: 0, s: 255, v: 255 };
  let endHSV = { h: 360, s: 255, v: 255 };
  for(let i=0; i<NUM_PIXELS; i++) {
    let rgb = hsv2rgb({
      h: endHSV.h * progress,
      s: startHSV.s,
      v: startHSV.v
    });
    pixels[i].r = rgb.r;
    pixels[i].g = rgb.g;
    pixels[i].b = rgb.b;
  }
}

function explode(progress, pixels, pixelCount, /* AnimationParams */params) {
  // WIP: Doesnt do owt yet.
  // build a blob in the middle of the string, and decay out as particles to the edges
  // e.g. 5s, grow for 2s, move out as particles for 2s, fade for 1s
  // animation is symmetrical, we'll calculate one side and just mirror it for the other

  let halfLength = pixelCount / 2;
  // how many points to distribute along the length of the string
  let pointRatio = 1/10;
  let allPointsLen = Math.floor(halfLength * pointRatio);
  let points = new Array(allPointsLen);
  let centerLeft = pixelCount % 2 ? Math.floor(halfLength) : Math.floor(halfLength) -1;
  let centerRight = pixelCount % 2 ? Math.floor(halfLength) -1 : Math.floor(halfLength);

  for(let i=0; i<pixelCount; i++) {
    let distanceToPoint = halfLength;
    let brightness = 0;
    let pointPosition;
    let distanceFromCenter = 0;
    for(let j=1; j<=allPointsLen; j++) {
      distanceFromCenter = Math.floor(halfLength * progress / j);
      if (i <= centerLeft) {
        pointPosition = centerLeft - distanceFromCenter;
        distanceToPoint = Math.min(distanceToPoint, Math.abs(i - pointPosition));
      } else {
        pointPosition = centerRight + distanceFromCenter;
        distanceToPoint = Math.min(distanceToPoint, Math.abs(i - pointPosition));
      }
    }
    if (distanceToPoint < halfLength * pointRatio) {
      brightness = 255 - Math.min(255, distanceToPoint * distanceToPoint);
    }
    pixels[i].r = brightness;
    pixels[i].g = brightness;
    pixels[i].b = brightness;
  }
}
