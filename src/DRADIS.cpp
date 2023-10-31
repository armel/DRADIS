// Copyright (c) F4HWN Armel. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "DRADIS.h"
#include "font.h"
#include "wav.h"
#include "sprites.h"
#include "functions.h"

// Setup
void setup() {
  // Init M5
  auto cfg = M5.config();

  cfg.clear_display = true;   // default=true. clear the screen when begin.
  cfg.internal_spk  = true;   // default=true. use internal speaker.
  cfg.internal_imu  = false;  // default=true. use internal IMU.
  cfg.internal_rtc  = false;  // default=true. use internal RTC.
  cfg.internal_mic  = false;  // default=true. use internal microphone.
  cfg.external_imu  = false;  // default=false. use Unit Accel & Gyro.
  cfg.external_rtc  = false;  // default=false. use Unit RTC.

  cfg.external_display.module_display = false;  // default=true. use ModuleDisplay
  cfg.external_display.atom_display   = false;  // default=true. use AtomDisplay
  cfg.external_display.unit_oled      = false;  // default=true. use UnitOLED
  cfg.external_display.unit_lcd       = false;  // default=true. use UnitLCD
  cfg.external_display.unit_rca       = false;  // default=false. use UnitRCA VideoOutput
  cfg.external_display.module_rca     = false;  // default=false. use ModuleRCA VideoOutput

  M5.begin(cfg);

  // Init Preferences
  preferences.begin(NAME);
  brightness = preferences.getUInt("brightness", BRIGHTNESS);
  theme      = preferences.getUInt("theme", 0);

  Serial.printf("Brightness = %d\n", brightness);
  Serial.printf("Theme = %d\n", theme);

  // Init Screen
  displayCount = M5.getDisplayCount();
  Serial.printf("On start %d\n", displayCount);

  // Init Leds
#if BOARD != CORES3
  if (M5.getBoard() == m5::board_t::board_M5Stack) {
    FastLED.addLeds<NEOPIXEL, 15>(leds,
                                  NUM_LEDS);  // GRB ordering is assumed
  } else if (M5.getBoard() == m5::board_t::board_M5StackCore2) {
    FastLED.addLeds<NEOPIXEL, 25>(leds,
                                  NUM_LEDS);  // GRB ordering is assumed
  }
  FastLED.setBrightness(16);
#endif

  // Init Rand
  esp_random();

  // Init Sprite
  labelSprite.setColorDepth(2);
  labelSprite.setPaletteColor(1, 0xFF0000U);  // Set palette

  viperSprite.setColorDepth(8);
  viperSprite.createSprite(17, 20);
  viperSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
  viperSprite.drawPng(viper, sizeof(viper), 0, 0, 17, 20);

  raptorSprite.setColorDepth(8);
  raptorSprite.createSprite(17, 20);
  raptorSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
  raptorSprite.drawPng(raptor, sizeof(raptor), 0, 0, 17, 20);

  battlestarSprite.setColorDepth(8);
  battlestarSprite.createSprite(17, 19);
  battlestarSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
  battlestarSprite.drawPng(battlestar, sizeof(battlestar), 0, 0, 17, 19);

  colonial1Sprite.setColorDepth(8);
  colonial1Sprite.createSprite(16, 10);
  colonial1Sprite.setPaletteColor(1, 0xFF0000U);  // Set palette
  colonial1Sprite.drawPng(colonial1, sizeof(colonial1), 0, 0, 16, 10);

  colonial2Sprite.setColorDepth(8);
  colonial2Sprite.createSprite(16, 13);
  colonial2Sprite.setPaletteColor(1, 0xFF0000U);  // Set palette
  colonial2Sprite.drawPng(colonial2, sizeof(colonial2), 0, 0, 16, 13);

  raiderSprite.setColorDepth(8);
  raiderSprite.createSprite(19, 20);
  raiderSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
  raiderSprite.drawPng(raider, sizeof(raider), 0, 0, 19, 20);

  unknownSprite.setColorDepth(8);
  unknownSprite.createSprite(19, 20);
  unknownSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
  unknownSprite.drawPng(unknown, sizeof(unknown), 0, 0, 19, 20);

  canvasSprite.setColorDepth(8);
  canvasSprite.createSprite(220, 180);

  clipSprite.setColorDepth(16);

  // Init Sound
  auto spk_cfg = M5.Speaker.config();

  M5.Speaker.setVolume(128);

  if (spk_cfg.use_dac || spk_cfg.buzzer) {
    spk_cfg.sample_rate = 192000;
  }
  M5.Speaker.config(spk_cfg);

  // Multitasking task for retreive button
  xTaskCreatePinnedToCore(button,    // Function to implement the task
                          "button",  // Name of the task
                          2048,      // Stack size in words
                          NULL,      // Task input parameter
                          4,         // Priority of the task
                          NULL,      // Task handle
                          1);        // Core where the task should run

#if BOARD != CORES3
  xTaskCreatePinnedToCore(cylon,    // Function to implement the task
                          "cylon",  // Name of the task
                          1024,     // Stack size in words
                          NULL,     // Task input parameter
                          4,        // Priority of the task
                          NULL,     // Task handle
                          1);       // Core where the task should run
#endif

  // Boot
  boot();
}

// Main loop
void loop() {
  video();
}
