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
  cfg.internal_imu  = true;   // default=true. use internal IMU.
  cfg.internal_rtc  = true;   // default=true. use internal RTC.
  cfg.internal_spk  = true;   // default=true. use internal speaker.
  cfg.internal_mic  = true;   // default=true. use internal microphone.
  cfg.external_imu  = false;  // default=false. use Unit Accel & Gyro.
  cfg.external_rtc  = false;  // default=false. use Unit RTC.

  cfg.external_display.module_display = true;   // default=true. use ModuleDisplay
  cfg.external_display.atom_display   = true;   // default=true. use AtomDisplay
  cfg.external_display.unit_oled      = false;  // default=true. use UnitOLED
  cfg.external_display.unit_lcd       = false;  // default=true. use UnitLCD
  cfg.external_display.unit_rca       = false;  // default=false. use UnitRCA VideoOutput
  cfg.external_display.module_rca     = false;  // default=false. use ModuleRCA VideoOutput

  M5.begin(cfg);

  // Init Preferences
  preferences.begin(NAME);
  brightness = preferences.getUInt("brightness", BRIGHTNESS);
  theme = preferences.getUInt("theme", 0);
  Serial.printf("Brightness = %d\n", brightness);

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
  viperSprite.setColorDepth(8);
  viperSprite.createSprite(17, 20);
  viperSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
  viperSprite.drawPng(viper, sizeof(viper), 0, 0, 17, 20);

  viperLabelSprite.setColorDepth(8);
  viperLabelSprite.createSprite(34, 10);
  viperLabelSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
  viperLabelSprite.drawString("vipers", 0, 0);

  raptorSprite.setColorDepth(8);
  raptorSprite.createSprite(17, 20);
  raptorSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
  raptorSprite.drawPng(raptor, sizeof(raptor), 0, 0, 17, 20);

  raptorLabelSprite.setColorDepth(8);
  raptorLabelSprite.createSprite(40, 10);
  raptorLabelSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
  raptorLabelSprite.drawString("raptors", 0, 0);

  raiderSprite.setColorDepth(8);
  raiderSprite.createSprite(19, 20);
  raiderSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
  raiderSprite.drawPng(raider, sizeof(raider), 0, 0, 19, 20);

  raiderLabelSprite.setColorDepth(8);
  raiderLabelSprite.createSprite(40, 10);
  raiderLabelSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
  raiderLabelSprite.drawString("raiders", 0, 0);

  unknownSprite.setColorDepth(8);
  unknownSprite.createSprite(19, 20);
  unknownSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
  unknownSprite.drawPng(unknown, sizeof(unknown), 0, 0, 19, 20);

  unknownLabelSprite.setColorDepth(8);
  unknownLabelSprite.createSprite(40, 10);
  unknownLabelSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
  unknownLabelSprite.drawString("unkown", 0, 0);

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
                          8192,      // Stack size in words
                          NULL,      // Task input parameter
                          4,         // Priority of the task
                          NULL,      // Task handle
                          1);        // Core where the task should run

  xTaskCreatePinnedToCore(cylon,    // Function to implement the task
                          "cylon",  // Name of the task
                          8192,     // Stack size in words
                          NULL,     // Task input parameter
                          4,        // Priority of the task
                          NULL,     // Task handle
                          1);       // Core where the task should run

  // Boot
  boot();
}

// Main loop
void loop() {
  video();
}