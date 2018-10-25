#include "./config.h"

#include <FastLED.h>
#include <Wire.h>
#include "lib/Fx_Helpers.h"
#include "lib/I2C_Anything.h"

#include "lib/Fx_HttpServer.cpp"
#include "lib/Fx_AppStates.cpp"

#define SDA_PIN 4
#define SCL_PIN 5

String IpAddress;
byte colorIndex = 0;

Anim_Clip pulsePink;
Anim_Clip raceBlue;

// -------------------------

void sendMessage(Anim_Clip clip) {
  Wire.beginTransmission(CLIENT_A_I2C_ADDRESS); // transmit to device #8
  // Wire.write ((byte *) &response, sizeof response);
  byte sentSize = I2C_writeAnything(clip);
  byte result = Wire.endTransmission(true);    // stop transmitting. true param to release the i2c bus
  // Serial.print("sendMessage sent:");
  // Serial.print(sentSize);
  // Serial.print(", result:");
  // Serial.println(result);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output

  Wire.begin();// join i2c bus (address optional for master)
  Wire.setClockStretchLimit(15000);    // in µs
  // SDA_PIN, SCL_PIN
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
  Serial.println(ipChars);

  // start sequence here
  Fx_HttpServer_init();

  Serial.print("Open http://");
  Serial.print(WiFi.localIP());
  Serial.println("/ in your browser to see it working");

  nextState = Running;

  pulsePink.anim = Pulse;
  pulsePink.startColor = { .h = 240, .s = 255, .v = 255 };
  pulsePink.endColor = { .h = 204, .s = 255, .v = 155 };
  pulsePink.initialDirection = 1;
  pulsePink.duration = 600;
  pulsePink.repeat = 3;

  raceBlue.anim = Race;
  raceBlue.startColor = { .h = 170, .s = 255, .v = 255 };
  raceBlue.endColor = { .h = 0, .s = 255, .v = 255 };
  raceBlue.initialDirection = 1;
  raceBlue.duration = 600;
  raceBlue.repeat = 3;

  Serial.println("/setup\n");
  Serial.flush();
}

bool lightOn;

void loop() {
  ArduinoOTA.handle();
  server.handleClient();

  sendMessage(raceBlue);

  Fx_updateState();
  digitalWrite(LED_BUILTIN, lightOn);
  if (lightOn) {
    lightOn = false;
  } else  {
    lightOn = true;
  }
  delay(5000); // on esp8266 delay yields
}
