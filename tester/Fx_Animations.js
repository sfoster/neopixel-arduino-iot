function Fx_AnimationParams(startColor={r:0,g:0,b:0}, endColor={r:0,g:0,b:0}, initialDirection=1) {
  this.startColor = startColor;
  this.endColor = endColor;
  this.direction = initialDirection;
}
const Fx_Animations = {};

function Fx_RegisterAnimation(name, animateFn, defaultParams={}) {
  Fx_Animations[name] = { name, animateFn, defaultParams };
}

// -----------------------------------------------------
// animations

function allOff(progress, pixels, pixelCount, /* AnimationParams */params) {
  for(let i=0; i<pixelCount; i++) {
    pixels[i].r = 0;
    pixels[i].g = 0;
    pixels[i].b = 0;
  }
}
Fx_RegisterAnimation("allOff", allOff, new Fx_AnimationParams());

function colorBlink(progress, pixels, pixelCount, /* AnimationParams */params) {
  let pcentOn;
  let rRange = params.endColor.r - params.startColor.r;
  let gRange = params.endColor.g - params.startColor.g;
  let bRange = params.endColor.b - params.startColor.b;

  if (progress <= 0.5) {
    color = progress * 255 * 2;
    pcentOn = progress * 2;
  } else {
    color = (1 - progress) * 255 * 2;
    pcentOn = (1 - progress) * 2;
  }
  for(let i=0; i<NUM_PIXELS; i++) {
    pixels[i].r = params.startColor.r  + (rRange * pcentOn);
    pixels[i].g = params.startColor.g  + (gRange * pcentOn);
    pixels[i].b = params.startColor.b  + (bRange * pcentOn);
  }
}
Fx_RegisterAnimation("colorBlink", colorBlink, new Fx_AnimationParams());
Fx_RegisterAnimation("redBlink", colorBlink, new Fx_AnimationParams({r:0,g:0,b:0}, {r:255,g:0,b:0}));
Fx_RegisterAnimation("blueBlink", colorBlink, new Fx_AnimationParams({r:0,g:0,b:0}, {r:0,g:155,b:255}));

function fadeToBlack(progress, pixels, pixelCount, /* AnimationParams */params) {
  // progressively turn each pixel black from start to end
  // TODO: could fade in a bit
  let lastIndex = Math.floor(pixelCount * progress);
  for(let i=0; i<=lastIndex; i++) {
    pixels[i].r = 0;
    pixels[i].g = 0;
    pixels[i].b = 0;
  }
}
Fx_RegisterAnimation("fadeToBlack", fadeToBlack, new Fx_AnimationParams());

function swipe(progress, pixels, pixelCount, /* AnimationParams */params) {
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
  // console.log(`swipe: progress: ${progress}, position: ${position}, ledToLight: ${ledToLight}`);
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
Fx_RegisterAnimation("swipe", swipe, new Fx_AnimationParams());
