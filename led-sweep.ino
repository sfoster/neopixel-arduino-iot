#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include "led-sweep.h"
#include "animations.h"

const char* ssid = "........";
const char* password = "........";

#define NEOPIXEL_STRIP_0 5

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_STRIP_0, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// -------------------------

ESP8266WebServer server(80);

const char* www_username = "admin";
const char* www_password = "esp8266";

// -------------------------

enum State currentState = start_state;
funcptr animateFn;

// last timestamp
int previousMillis;
// timestamp from the start of the current tick
int now;
int startTime;
int animationDuration;
bool isAnimating = false;

void updateState() {
  previousMillis = now;
  now = millis();
  if (previousMillis > now) {
    // overflow happened
    previousMillis = now;
  }

  switch (currentState) {
    case start_state :
      isAnimating = false;
      currentState = color_fade_transition;
      break;
    case off_transition :
      allOff(0.0, pixelColors, NUM_PIXELS);
      isAnimating = false;
      currentState = off_state;
      break;
    case off_state :
      isAnimating = false;
      break;
    case color_fade_transition :
      isAnimating = true;
      startTime = now;
      animationDuration = 5000;
      currentState = color_fade_state;
      animateFn = &colorFade;
      debugPrint("color_fade_transition, isAnimating is now true\n");
  }
  if (isAnimating) {
    int elapsed = clamp(now - startTime, 0, animationDuration);
    if (elapsed >= animationDuration) {
      currentState = off_transition;
      return;
    }
    double progress = 0.0;
    if (elapsed > 0) {
      progress = (float)elapsed / (float)animationDuration;
    }

    (*animateFn)(progress, pixelColors, NUM_PIXELS);
  }
}

void display() {
  for(int i=0; i<NUM_PIXELS; i++) {
    strip.setPixelColor(i, pixelColors[i].r, pixelColors[i].g, pixelColors[i].b);
    strip.show();
  }
}

void setup() {
  Serial.begin(115200);
  strip.begin();
  now = millis();
  // Initialize all pixels to 'off'
  allOff(0.0, pixelColors, NUM_PIXELS);
  display();
  // could do some wifi-setup sequence here
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }
  ArduinoOTA.begin();
  debugPrint(WiFi.localIP().toString());
  debugPrint("\n");

  // could do some start sequence here
  //

  server.on("/", []() {
    server.send(200, "text/plain", "Hello world");
  });

  server.on("/animation", HTTP_POST, []() {
    if (server.uri() != "/animation") {
      return;
    }
    //Basic Auth Method with Custom realm and Failure Response
    //return server.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
    //Digest Auth Method with realm="Login Required" and empty Failure Response
    //return server.requestAuthentication(DIGEST_AUTH);
    //Digest Auth Method with Custom realm and empty Failure Response
    //return server.requestAuthentication(DIGEST_AUTH, www_realm);
    //Digest Auth Method with Custom realm and Failure Response
    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }
    server.send(200, "text/plain", "Done");
  });

  server.begin();

  Serial.print("Open http://");
  Serial.print(WiFi.localIP());
  Serial.println("/ in your browser to see it working");

  // todo: initialize the array of rgb values we'll update each frame
  debugPrint("/setup\n");
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  updateState();
  display();
  delay(16); // on esp8266 delay yields
}

