void allOff(double progress) {
  for(int i=0; i<NUM_PIXELS; i++) {
    pixelColors[i].r = 0;
    pixelColors[i].g = 0;
    pixelColors[i].b = 0;
  }
}

void colorFade(double progress) {
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
    pixelColors[i].r = 0;
    pixelColors[i].g = color;
    pixelColors[i].b = 0;
  }
}

