#include "./config.h"

#include <Wire.h>
#include "lib/Fx_Types.h"
#include "lib/Fx_Helpers.h"
#include "lib/I2C_Anything.h";

// include "lib/Fx_Animations.cpp"
// include "lib/Fx_Controller.cpp"
#include "lib/Fx_HttpServer.cpp"
#include "lib/Fx_AppStates.cpp"

#define SDA_PIN 4
#define SCL_PIN 5

typedef struct CHSV {
  unsigned char h;
  unsigned char s;
  unsigned char v;
} CHSV;

String IpAddress;
byte colorIndex = 0;
CHSV currentPalette[16];
// -------------------------


#if 0
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
#endif

void sendMessage(CHSV color) {
  Wire.beginTransmission(8); // transmit to device #8
  // Wire.write ((byte *) &response, sizeof response);
  I2C_writeAnything(color);
  // Wire.write("content is "); // sends five bytes
  // Wire.write(content);       // sends one byte
  Wire.endTransmission();    // stop transmitting

  Serial.print("Sent clip: ");
  Serial.print(color.h);
  Serial.print(",");
  Serial.print(color.s);
  Serial.print(",");
  Serial.print(color.v);
  Serial.println(";");
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output

  Wire.begin(SDA_PIN, SCL_PIN);// join i2c bus (address optional for master)
  Serial.begin(115200);

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
  IpAddress = WiFi.localIP().toString();
  char ipChars[32];
  strcpy(ipChars, IpAddress.c_str());
  debugPrint(ipChars);
  debugPrint("\n");

  // start sequence here
  Fx_HttpServer_init();

  Serial.print("Open http://");
  Serial.print(WiFi.localIP());
  Serial.println("/ in your browser to see it working");

  nextState = Running;
  debugPrint("/setup\n");

  // fill the palette with totally random colors.
  // void SetupTotallyRandomPalette() {
  for( int i = 0; i < 16; i++) {
    currentPalette[i] = { .h = (unsigned char)(rand()%256),
                          .s = 255,
                          .v = (unsigned char)(rand()%256)
                        };
  }
  Serial.flush();
}

bool lightOn;

void loop() {
  ArduinoOTA.handle();
  server.handleClient();

  if (++colorIndex >= 16) {
    colorIndex = 0;
  }
  sendMessage(currentPalette[colorIndex]);

  Fx_updateState();
  // Fx_Controller_updateTimeline(millis());
  // display();
  digitalWrite(LED_BUILTIN, lightOn);
  if (lightOn) {
    lightOn = false;
  } else  {
    lightOn = true;
  }
  delay(2000); // on esp8266 delay yields
}
