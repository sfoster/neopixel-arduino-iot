void allOff(double progress, RBGColor* pixels, int pixelCount) {
  for(int i=0; i<pixelCount; i++) {
    pixels[i].r = 0;
    pixels[i].g = 0;
    pixels[i].b = 0;
  }
}

void colorFade(double progress, RBGColor* pixels, int pixelCount) {
  byte color;
  if (progress <= 0.5) {
    color = progress * 255 * 2;
  } else {
    color = (1 - progress) * 255 * 2;
  }
  char buffer[255];
  sprintf(buffer, "colorFade, color: %d, progress: %f\n", color, progress);
  debugPrint(buffer);

  for(int i=0; i<NUM_PIXELS; i++) {
    pixels[i].r = 0;
    pixels[i].g = color;
    pixels[i].b = 0;
  }
}

