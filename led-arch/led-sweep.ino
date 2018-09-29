#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "./config.h"
#include "./Fx_Types.h"
#include "./Fx_Controller.cpp"
#include "./Fx_Animations.cpp"
#include "./Fx_HttpServer.cpp"

Adafruit_NeoPixel neopixels = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_STRIP_0, NEO_GRB + NEO_KHZ800);

// -------------------------

enum State currentState = start_state;

void updateState() {
  switch (currentState) {
    case start_state :
      Fx_Controller_Reset();
      currentState = start_transition;
      break;
    case off_transition :
      Fx_Controller_Reset();
      Fx_Controller_AddClip()
      allOff(0.0, pixelColors, NUM_PIXELS);
      isAnimating = false;
      currentState = off_state;
      break;
    case off_state :
      isAnimating = false;
      break;
    case start_transition :
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
    neopixels.setPixelColor(i, pixelColors[i].r, pixelColors[i].g, pixelColors[i].b);
    neopixels.show();
  }
}

void setup() {
  Serial.begin(115200);
  neopixels.begin();
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

