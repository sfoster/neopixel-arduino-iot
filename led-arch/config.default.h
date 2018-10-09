#ifndef CONFIG_H

#define DEBUG 1

// the number of pixels on the neopixel string
#define NUM_PIXELS 150
// the max number of clips to have queued up at a time. Prolly don't need to change this
#define MAX_FX_CLIPS 8

// the i/o pin to send control signals to the neopixel string on
#define NEOPIXEL_STRIP_0 5

// the WiFi network to connect to
const char* ssid = "";
const char* password = "";

// the auth for POSTing to this serve
const char* www_username = "";
const char* www_password = "";


#define CONFIG_H
#endif
