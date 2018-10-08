#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "./config.h"
#include "lib/Fx_Types.h"
#include "lib/Fx_Helpers.h"

#include "lib/Fx_Animations.cpp"
#include "lib/Fx_Controller.cpp"
#include "lib/Fx_HttpServer.cpp"
#include "lib/Fx_AppStates.cpp"

Adafruit_NeoPixel neopixels = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_STRIP_0, NEO_GRB + NEO_KHZ800);

// -------------------------


void display() {
  for(unsigned int i=0; i<NUM_PIXELS; i++) {
    byte r = clamp(
      fxController.backgroundPixels[i].r + fxController.foregroundPixels[i].r,
      0, 255
    );
    byte g = clamp(
      fxController.backgroundPixels[i].g + fxController.foregroundPixels[i].g,
      0, 255
    );
    byte b = clamp(
      fxController.backgroundPixels[i].b + fxController.foregroundPixels[i].b,
      0, 255
    );
    neopixels.setPixelColor(i, r, g, b);
    neopixels.show();
  }
}

void setup() {
  Serial.begin(115200);
  neopixels.begin();

  Fx_Controller_Init();

  // prepare wifi-setup sequence here
  nextState = Connecting;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }
  ArduinoOTA.begin();
  String ip = WiFi.localIP().toString();
  char ipChars[256];
  strcpy(ipChars, ip.c_str());
  debugPrint(ipChars);
  debugPrint("\n");

  // start sequence here
  Fx_HttpServer_init();

  Serial.print("Open http://");
  Serial.print(WiFi.localIP());
  Serial.println("/ in your browser to see it working");

  nextState = Running;
  debugPrint("/setup\n");
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();

  Fx_updateState();
  Fx_Controller_updateTimeline(millis());
  display();
  delay(16); // on esp8266 delay yields
}
