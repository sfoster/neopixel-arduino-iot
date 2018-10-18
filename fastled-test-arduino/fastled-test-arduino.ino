#include <FastLED.h>
#include <Wire.h>
#include "lib/I2C_Anything.h";

#define NUM_LEDS 7
#define DATA_PIN 5
#define ANNOUNCE_PIN 0

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

bool gotMessage;
unsigned long receivedTime;

CRGB leds[NUM_LEDS];
CHSV currentHSV;
byte currentBrightness;
char direction;
// uint8_t gHue = 0; // rotating "base color" used by many of the patterns

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  Serial.print("receiveEvent: ");
  Serial.println(howMany);

  if (howMany >= sizeof(CHSV))
  {
     I2C_readAnything (currentHSV);
     gotMessage = true;
  }  // end if have enough data

  // while (1 < Wire.available()) { // loop through all but the last
  //   char c = Wire.read(); // receive byte as a character
  //   Serial.print(c);         // print the character
  // }
  // int x = Wire.read();    // receive byte as an integer
  // Serial.println(x);         // print the integer
}

void setup() {
  receivedTime = millis();

  Serial.begin(9600);
  Serial.print("BRIGHTNESS: ");
  Serial.println(BRIGHTNESS);

  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event

  delay(3000); // 3 second delay for recovery

  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);

  // set master brightness control
  FastLED.setBrightness(204);
  currentBrightness = BRIGHTNESS;
  direction = 1;
  currentHSV = CHSV(HUE_RED, 255, 204);
  Serial.print("setup currentHSV.v:");
  Serial.println(currentHSV.v);
}

void addGlitter( fract8 chanceOfGlitter)
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void loop() {
  if (gotMessage) {
    Serial.print("Got new color:");
    Serial.print(currentHSV.h);
    Serial.print(",");
    Serial.print(currentHSV.s);
    Serial.print(",");
    Serial.print(currentHSV.v);
    Serial.println(";");
    gotMessage = false;
  }

  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = currentHSV;
  }
  // addGlitter(80);
  //fill_rainbow( leds, NUM_LEDS, gHue, 7);

  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);
  // do some periodic updates
  // EVERY_N_MILLISECONDS( 32 ) { }
  // fade up and down
  // if (currentBrightness <= 100 || currentBrightness >= 255) {
  //   direction *= -1;
  // }
  // currentBrightness += direction;
  // currentHSV.v = currentBrightness;
}
