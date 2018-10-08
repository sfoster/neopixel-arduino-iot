#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include "./Fx_HttpServer.h"

const char* www_username = "admin";
const char* www_password = "esp8266";
ESP8266WebServer server(80);

void Fx_HttpServer_handleAnimationPost() {
  // Enqueue an animation
  String animationName = server.arg("animationName");
  int duration = server.arg("duration").toInt();

  char colorBuf[7];
  server.arg("startColor").toCharArray(colorBuf, 7);
  RGBColor startColor = hexColorStringToRGBStruct(colorBuf);

  server.arg("endColor").toCharArray(colorBuf, 7);
  RGBColor endColor = hexColorStringToRGBStruct(colorBuf);

  int initialDirection = server.arg("initialDirection").toInt();
  int isForeground = server.arg("isForeground").toInt();

  String strRepeat = server.arg("repeat");
  long repeat;
  if (strRepeat.compareTo("Infinity") == 0) {
    repeat = LONG_MAX;
  } else {
    repeat = (long)strRepeat.toInt();
  }
}

void Fx_HttpServer_init() {
  server.on("/", []() {
    server.send(200, "text/plain", "Hello world");
  });

  server.on("/animation", HTTP_POST, [](){
    if (server.uri() != "/animation") {
      return;
    }
    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }
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

