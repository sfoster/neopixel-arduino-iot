#include <FastLED.h>
#define NUM_LEDS 150
#define DATA_PIN 5
#define TWO_HUNDRED_PI 628
#define TWO_THIRDS_PI 2.094

CRGB leds[NUM_LEDS];
unsigned int lastIndex;
unsigned char val;
unsigned long startTime;
unsigned long duration = 8000;
unsigned long elapsed;
unsigned long now;
unsigned long previousMillis;
// repeat can go to -1 so needs to be signed
long repeat;

void setup() {
  Serial.begin(9600);
  Serial.print("F_CPU: ");
  Serial.println(F_CPU);

  lastIndex = NUM_LEDS -1 -(NUM_LEDS % 3);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  startTime = previousMillis = millis();
}
void loop() {
  now = millis();
  if ((previousMillis > now) ||
      previousMillis == 0) {
    // first update, or maybe overflow happened
    previousMillis = now;
  }
  unsigned long elapsed = now - startTime;
  if (elapsed >= (unsigned long)duration) {
    startTime = now;
    elapsed = elapsed % duration;
  }
  float progress = 0.0;
  if (elapsed > 0) {
    progress = (float)elapsed / (float)duration;
  }

  if (progress <= 0.5) {
    val = (unsigned char)(progress * 180.0 * 2);
  } else {
    val = (unsigned char)((1 - progress) * 180.0 * 2);
  }
  Serial.print("progress:");
  Serial.print(progress);
  Serial.print(", val:");
  Serial.print(val);
  Serial.println("");

  for(int i=0; i<NUM_LEDS; i++) {
    leds[i] = CHSV(180+val, 255, 204);
  }
  FastLED.show();
  delay(1);
}
