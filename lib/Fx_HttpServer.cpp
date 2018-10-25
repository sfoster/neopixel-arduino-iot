#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include "./Fx_Types.h"
#include "./Fx_HttpServer.h"

ESP8266WebServer server(80);

void Fx_HttpServer_handleResetPost() {
  Serial.println("Got reset POST");
}

void Fx_HttpServer_handleAnimationPost() {
  // Enqueue an animation
  char nameBuf[80];
  String nameString = server.arg("animationName");
  nameString.toCharArray(nameBuf, 80);

  // if (nameString == "allOff") {
  //   animateFn = &allOff;
  // } else if (nameString == "colorBlink") {
  //   animateFn = &colorBlink;
  // } else if (nameString == "endWithColor") {
  //   animateFn = &endWithColor;
  // } else if (nameString == "bounce") {
  //   animateFn = &bounce;
  // } else {
  //   debugPrint("Unknown animation function: %s", nameBuf);
  //   return;
  // }

  long duration = (long)server.arg("duration").toInt();

  char colorBuf[7];
  server.arg("startColor").toCharArray(colorBuf, 7);
  CRGB startColor = hexColorStringToRGBStruct(colorBuf);

  server.arg("endColor").toCharArray(colorBuf, 7);
  CRGB endColor = hexColorStringToRGBStruct(colorBuf);

  int initialDirection = server.arg("initialDirection").toInt();

  String strRepeat = server.arg("repeat");
  long repeat;
  if (strRepeat.compareTo("Infinity") == 0) {
    repeat = LONG_MAX;
  } else {
    repeat = (long)strRepeat.toInt();
  }

  Anim_Clip clip;
  clip.anim = Pulse;
  clip.startColor = { .hue=0, .sat=0, .val=0 };
  clip.endColor = { .hue=0, .sat=0, .val=0 };
  clip.initialDirection = initialDirection;
  clip.repeat = repeat;
  clip.duration = duration;

  Serial.println("Prepared clip");
  // Fx_Controller_AddClip(animateFn, isForeground, duration, repeat, params);
}

void Fx_HttpServer_init() {
  server.on("/", []() {
    server.send(200, "text/plain", "Hello world");
  });

  server.on("/reset", HTTP_POST, [](){
    if (server.uri() != "/reset") {
      return;
    }
    // if (!server.authenticate(www_username, www_password)) {
    //   return server.requestAuthentication();
    // }
    Fx_HttpServer_handleResetPost();
    server.send(200, "text/plain", "Animation clips reset");
  });

  server.on("/animation", HTTP_POST, [](){
    if (server.uri() != "/animation") {
      return;
    }
    // if (!server.authenticate(www_username, www_password)) {
    //   return server.requestAuthentication();
    // }
    Fx_HttpServer_handleAnimationPost();

    //Basic Auth Method with Custom realm and Failure Response
    //return server.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
    //Digest Auth Method with realm="Login Required" and empty Failure Response
    //return server.requestAuthentication(DIGEST_AUTH);
    //Digest Auth Method with Custom realm and empty Failure Response
    //return server.requestAuthentication(DIGEST_AUTH, www_realm);
    //Digest Auth Method with Custom realm and Failure Response
    server.send(200, "text/plain", "Done");
  });

  server.begin();
}

