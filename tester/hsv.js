'use strict';

function hsv2rgb(HSV) {
  let RGB;
  let H = HSV.h,
      S = HSV.s,
      V = HSV.v;
  let P, Q, T, fract;

  H = (H == 360.) ? 0 : H / 60;
  fract = H - Math.floor(H);

  P = V*(1 - S);
  Q = V*(1 - S*fract);
  T = V*(1 - S*(1 - fract));

  if (0 <= H && H < 1) {
    RGB = { r: V, g: T, b: P };
  }
  else if (1 <= H && H < 2) {
    RGB = { r: Q, g: V, b: P };
  }
  else if (2 <= H && H < 3) {
    RGB = { r: P, g: V, b: T };
  }
  else if (3 <= H && H < 4) {
    RGB = { r: P, g: Q, b: V };
  }
  else if (4 <= H && H < 5) {
    RGB = { r: T, g: P, b: V };
  }
  else if (5 <= H && H < 6) {
    RGB = { r: V, g: P, b: Q };
  }
  else {
    RGB = { r: 0, g: 0, b: 0 };
  }
  return RGB;
}
