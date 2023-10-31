// Copyright (c) F4HWN Armel. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Version
#define VERSION "1.3.0"
#define AUTHOR  "F4HWN"
#define NAME    "DRADIS"

#define RANDOM 1  // Set to 1 for random view
#define CORE   1
#define CORE2  2
#define CORES3 3

// Others define
#define DRADIS_FOLDER      "/DRADIS"
#define DRADIS_LOGO        "/BattlestarGalactica.png"
#define DRADIS_WAV         "/DRADIS.wav"
#define DRADIS_VIDEO_GREY  "/loopGrey.mjpg"
#define DRADIS_VIDEO_BLUE  "/loopBlue.mjpg"
#define DRADIS_VIDEO_SEPIA "/loopSepia.mjpg"

#define TFT_DRADIS M5.Displays(0).color565(16, 16, 16)

#define MJPEG_BUFFER_SIZE 320 * 64  // Memory for a single JPEG frame

// Dependencies
#include <Preferences.h>
#include <LittleFS.h>
#include <SD.h>
#include <FastLED.h>
#include <IniFile.h>
#include <M5Unified.h>
#include <WiFi.h>
#include <time.h>
#include "MjpegClass.h"

// Preferences
Preferences preferences;

// Sprite
LGFX_Sprite labelSprite(&M5.Display);

LGFX_Sprite viperSprite(&M5.Display);
LGFX_Sprite raptorSprite(&M5.Display);
LGFX_Sprite battlestarSprite(&M5.Display);
LGFX_Sprite colonial1Sprite(&M5.Display);
LGFX_Sprite colonial2Sprite(&M5.Display);

LGFX_Sprite raiderSprite(&M5.Display);
LGFX_Sprite unknownSprite(&M5.Display);

LGFX_Sprite canvasSprite(&M5.Display);

LGFX_Sprite clipSprite(&M5.Display);

uint8_t viperNum  = 0;
uint8_t raptorNum = 0;
uint8_t raiderNum = 0;

int16_t viperX, viperY;
int16_t raptorX, raptorY;
int16_t battlestarX, battlestarY;
int16_t colonial1X, colonial1Y;
int16_t colonial2X, colonial2Y;
int16_t raiderX, raiderY;
int16_t unknownX, unknownY;

int8_t viperStep      = false;
int8_t raptorStep     = false;
int8_t battlestarStep = false;
int8_t raiderStep     = false;
int8_t unknownStep    = false;

boolean viperView     = false;
boolean raptorView    = false;
boolean colonial1View = false;
boolean colonial2View = false;
boolean raiderView    = false;
boolean unknownView   = false;

boolean themeChange = false;

// LED
#define FASTLED_INTERNAL  // To disable pragma messages on compile
#define NUM_LEDS 10
CRGB leds[NUM_LEDS];

int8_t ledOn        = 0;
int8_t ledDirection = 1;

// Config
struct Config {
  char wifiSSID[64];      // Wifi SSID
  char wifiPassword[64];  // Wifi Password

  char timeServer[128] = "";	// NTP Server
  char timeTimeZone[128] = "";  // Geolocation timezone
};

Config myConfig;

// Variables
static MjpegClass mjpegClass;

fs::File root;
fs::File mjpegFile;

uint8_t shiftX       = 50;
uint8_t shiftY       = 20;
uint8_t displayCount = 0;
uint8_t theme        = 0;
uint16_t brightness  = BRIGHTNESS;

boolean wifiConnected   = 0;

char localTime[10];