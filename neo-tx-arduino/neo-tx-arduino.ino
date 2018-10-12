#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "./config.h"
#include "lib/Fx_Types.h"
#include "lib/Fx_Helpers.h"

#include "lib/Fx_Animations.cpp"
#include "lib/Fx_Controller.cpp"

Adafruit_NeoPixel neopixels = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_STRIP_0, NEO_GRB + NEO_KHZ800);
String rxCommand;
// -------------------------

void display() {
  for(unsigned short i=0; i<NUM_PIXELS; i++) {
    unsigned char r = clamp(
      fxController.backgroundPixels[i].r + fxController.foregroundPixels[i].r,
      0, 255
    );
    unsigned char g = clamp(
      fxController.backgroundPixels[i].g + fxController.foregroundPixels[i].g,
      0, 255
    );
    unsigned char b = clamp(
      fxController.backgroundPixels[i].b + fxController.foregroundPixels[i].b,
      0, 255
    );
    neopixels.setPixelColor(i, r, g, b);
    neopixels.show();
  }
}

void setup() {
  Serial.begin(9600);
  Serial.print("F_CPU: ");
  Serial.println(F_CPU);

  neopixels.begin();

  Fx_Controller_Init();

  Fx_Controller_AddClip(&colorBlink, false, 3000UL, LONG_MAX, toBlueParams);

  // debugPrint("/setup\n");
  // Serial.flush();
}

void loop() {
  // if (Serial.available() > 0) {
  //   // read the incoming byte:
  //   rxCommand = Serial.readString();
  //   Serial.flush();
  //   // if (rxCommand == "?") {
  //     Serial.print("Got command: ");
  //     Serial.print(rxCommand);
  //     Serial.println("");
  //   // }
  // }

  Fx_Controller_updateTimeline(millis());
  display();
  // delay(16);
}

#if 0
void setup() {
  Serial.begin(9600);
  Serial.print("F_CPU: ");
  Serial.println(F_CPU);

  neopixels.begin();
  neopixels.show();
}

void loop() {
 // Some example procedures showing how to display to the pixels:
  for(uint16_t i=0; i<neopixels.numPixels(); i++) {
    neopixels.setPixelColor(i, neopixels.Color(255, 0, 0));
  }
  neopixels.show();
  delay(1000);
  for(uint16_t i=0; i<neopixels.numPixels(); i++) {
    neopixels.setPixelColor(i, neopixels.Color(0, 0, 0));
  }
  neopixels.show();
  delay(1000);
}
#endif

