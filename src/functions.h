// Copyright (c) F4HWN Armel. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Wifi callback On
void callbackWifiOn(WiFiEvent_t event, WiFiEventInfo_t info) {
  wifiConnected = true;
  Serial.println("Wifi Client Connected");
}

// Wifi callback Got IP
void callbackWifiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println(WiFi.localIP());
}

// Wifi callback Off
void callbackWifiOff(WiFiEvent_t event, WiFiEventInfo_t info) {
  wifiConnected = false;
  Serial.println("Wifi Client Disconnected");

  WiFi.begin(myConfig.wifiSSID, myConfig.wifiPassword);
}

// Get local time
void updateLocalTime(boolean startup = false) {
  char timeStringBuff[10];  // 10 chars should be enough
  char utcStringBuff[10];   // 10 chars should be enough
  int utc = 1;

  struct tm timeinfo;

  if (startup) {
    while (!getLocalTime(&timeinfo)) {
      M5.Displays(0).drawString("Synchronisation heure locale", 160, 70);
      Serial.println("Failed to obtain time");
      delay(1000);
    }
  } else {
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
  }

  strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M:%S", &timeinfo);
  strftime(utcStringBuff, sizeof(utcStringBuff), "%z", &timeinfo);

  sscanf(utcStringBuff, "%d", &utc);
  utc = utc / 100;

  // Serial.println(utc);

  strcpy(localTime, timeStringBuff);

  Serial.println(localTime);
}

// Pixel drawing callback
static int mjpegDrawCallback(JPEGDRAW *pDraw) {
  clipSprite.deleteSprite();
  clipSprite.createSprite(pDraw->iWidth, pDraw->iHeight);
  clipSprite.pushImage(0, 0, pDraw->iWidth, pDraw->iHeight, pDraw->pPixels);
  canvasSprite.pushSprite(&clipSprite, -pDraw->x + shiftX, -pDraw->y + shiftY, 0);
  clipSprite.pushSprite(&M5.Displays(0), pDraw->x, pDraw->y);

  return 1;
}

// Fade all led
void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(100);
  }
}

// Clock
void clock(void *pvParameters) {
  for (;;) {
    if (WiFi.status() == WL_CONNECTED) {
      updateLocalTime();
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// Cylon
void cylon(void *pvParameters) {
  for (;;) {
    // First slide the led in one direction
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Red;
      ;
      // Show the leds
      FastLED.show();
      fadeall();
      vTaskDelay(pdMS_TO_TICKS(50));
    }

    // Now go in the other direction.
    for (int i = (NUM_LEDS)-1; i >= 0; i--) {
      leds[i] = CRGB::Red;
      ;
      FastLED.show();
      fadeall();
      vTaskDelay(pdMS_TO_TICKS(50));
    }
    // Serial.printf("stackHWM Cylon: %d\n", uxTaskGetStackHighWaterMark(NULL));
  }
}

// Check button
void button(void *pvParameters) {
  uint8_t step = 2;
  uint8_t min  = 4;
  uint8_t max  = 255;

  uint64_t counter = 0;

  uint8_t btnA = 0;
  uint8_t btnB = 0;
  uint8_t btnC = 0;

  uint16_t brightnessOld = 0;

  struct Button {
    char name[16];   // name
    int x;           // x
    int y;           // y
    int w;           // width
    int h;           // height
    int d;           // distance
    boolean active;  // active, if true, check this button, else bypass
    boolean read;    // read, if true, button is push, else false
  };

#if BOARD == CORES3
  Button myBtn[] = {
    {"myBtnA", 0, 160, 100, 80, 1000, true, false},
    {"myBtnB", 110, 160, 100, 80, 1000, true, false},
    {"myBtnC", 220, 160, 100, 80, 1000, true, false},
  };
#else
  Button myBtn[] = {
    {"myBtnA", 0, 240, 100, 80, 1000, true, false},
    {"myBtnB", 110, 240, 100, 80, 1000, true, false},
    {"myBtnC", 220, 240, 100, 80, 1000, true, false},
  };
#endif

  for (;;) {
    M5.update();

    if (counter == 600) {
      counter = 0;
      // Serial.printf("stackHWM Button: %d\n", uxTaskGetStackHighWaterMark(NULL));
    }

    if (counter == 0) {
      viperNum  = random(0, 2);
      viperView = random(0, 2);
      viperX    = 160 + random(-50, 50);
      viperY    = 70 + random(-30, 40);
      viperStep = random(-1, 2);

      raptorNum  = random(0, 2);
      raptorView = random(0, 2);
      raptorX    = 160 + random(-40, 40);
      raptorY    = 100 + random(-10, 20);
      raptorStep = random(-1, 2);

      battlestarX    = 160 + random(-15, 15);
      battlestarY    = 130 + random(-10, 10);
      battlestarStep = random(-1, 2);

      colonial1X = 160 + random(-60, -30);
      colonial1Y = 115 + random(-10, 10);

      colonial2X = 160 + random(30, 60);
      colonial2Y = 125 + random(-10, 10);

      raiderNum  = random(0, 2);
      raiderView = random(0, 2);
      raiderX    = 160 + random(-30, 30);
      raiderY    = 40 + random(-10, 30);
      raiderStep = random(-1, 2);

      unknownX    = 160 + random(-40, 40);
      unknownY    = 40 + random(-10, 10);
      unknownStep = random(-1, 2);

      // Limit colision

      if (viperView == 1) {
        if (sqrt((viperX - battlestarX) * (viperX - battlestarX) + (viperY - battlestarY) * (viperY - battlestarY)) <
            40) {
          viperView = 0;
        }
      }

      if (raptorView == 1) {
        if (sqrt((raptorX - battlestarX) * (raptorX - battlestarX) +
                 (raptorY - battlestarY) * (raptorY - battlestarY)) < 40) {
          raptorView = 0;
        }
      }

      if (viperView == 1 && raptorView == 1) {
        if (sqrt((viperX - raptorX) * (viperX - raptorX) + (viperY - raptorY) * (viperY - raptorY)) < 20) {
          raptorView = 0;
        }
      }

      if (viperView == 0 && raptorView == 0) {
        colonial1View = 1;
        colonial2View = 1;

        if (sqrt((colonial1X - colonial2X) * (colonial1X - colonial2X) +
                 (colonial1Y - colonial2Y) * (colonial1Y - colonial2Y)) < 20) {
          colonial2View = 0;
        }
      } else {
        colonial1View = 0;
        colonial2View = 0;
      }
    }

    if (counter != 0 && counter % 100 == 0) {
      viperX += viperStep * 4;
      viperY -= 4;

      raptorX += raptorStep;
      raptorY -= 1;

      battlestarX += battlestarStep;
      battlestarY -= 1;

      colonial1X -= 1;
      colonial1Y += 1;

      colonial2X += 1;
      colonial2Y += 1;

      raiderX += raiderStep;
      raiderY += 1;

      unknownX += unknownStep;
      unknownY += 1;
    }

    counter++;

    if (M5.getBoard() == m5::board_t::board_M5Stack || M5.getBoard() == m5::board_t::board_M5StackCore2) {
      step = 4;
      min  = 4;
      max  = 254;

      btnA = M5.BtnA.isPressed();
      btnB = M5.BtnB.isPressed();
      btnC = M5.BtnC.isPressed();
    } else if (M5.getBoard() == m5::board_t::board_M5StackCoreS3) {
      step = 16;
      min  = 64;
      max  = 240;

      auto t = M5.Touch.getDetail();
      auto c = M5.Touch.getCount();

      uint8_t limit = sizeof myBtn / sizeof myBtn[0];

      int distanceBtn     = 0;
      int distanceMin     = 1000;
      int distanceCurrent = 1000;

      if (c == 1) {
        for (uint8_t i = 0; i < limit; i++) {
          myBtn[i].read = false;
          if (myBtn[i].active == true) {
            distanceCurrent = (int)(sqrt(pow(t.x - (myBtn[i].x + (myBtn[i].w / 2)), 2) +
                                         pow(t.y - (myBtn[i].y + (myBtn[i].h / 2)), 2)));
            myBtn[i].d      = distanceCurrent;
            if (distanceCurrent < distanceMin) {
              distanceMin = distanceCurrent;
              distanceBtn = i;
            }
          }
        }

        if (t.state == 1 || t.state == 3 || t.state == 5) {
          myBtn[distanceBtn].read = true;
        }
      }

      btnA = myBtn[0].read;
      btnB = myBtn[1].read;
      btnC = myBtn[2].read;
    }

    if (btnB) {
      themeChange = true;
    } else {
      themeChange = false;
    }

    if (btnA || btnC) {
      if (M5.getBoard() == m5::board_t::board_M5StackCoreS3) {
        vTaskDelay(pdMS_TO_TICKS(100));
      }

      brightnessOld = brightness;

      if (btnA) {
        brightnessOld -= step;
        brightnessOld = (brightnessOld <= min) ? min : brightnessOld;
      } else if (btnC) {
        brightnessOld += step;
        brightnessOld = (brightnessOld >= max) ? max : brightnessOld;
      }

      if (brightnessOld != brightness) {
        brightness = brightnessOld;
        M5.Displays(0).setBrightness(brightness);
        preferences.putUInt("brightness", brightness);
        Serial.println(brightness);
      }
    }

    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

// Boot
void boot() {
  char string[64];

  if (!LittleFS.begin()) {
    Serial.println(F("ERROR: File System Mount Failed!"));
  } else {
    M5.Displays(0).drawPngFile(LittleFS, DRADIS_LOGO, 0, 75, 320, 90);
  }

  M5.Displays(0).setFont(&Ubuntu_Medium6pt7b);
  M5.Displays(0).setTextDatum(CC_DATUM);
  M5.Displays(0).setTextColor(TFT_WHITE, TFT_DRADIS);

  sprintf(string, "%s Version %s", NAME, VERSION);
  M5.Displays(0).drawString(string, 160, 20);

  sprintf(string, "by %s", AUTHOR);
  M5.Displays(0).drawString(string, 160, 35);

  // We start by connecting to the WiFi network
  if (ESP.getPsramSize() == 0) {
    Serial.println("No PSRAM");
  }
  else
  {
    if (strcmp(myConfig.wifiSSID, "") != 0 && strcmp(myConfig.wifiPassword, "") != 0) {

      WiFi.onEvent(callbackWifiOn, ARDUINO_EVENT_WIFI_STA_CONNECTED);
      WiFi.onEvent(callbackWifiGotIP, ARDUINO_EVENT_WIFI_STA_GOT_IP);
      WiFi.onEvent(callbackWifiOff, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

      while (true) {
        uint8_t attempt = 1;
        WiFi.begin(myConfig.wifiSSID, myConfig.wifiPassword);
        while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          attempt++;
          if (attempt > 10) {
            break;
          }
        }
        if (WiFi.status() == WL_CONNECTED) {
          break;
        }
      }
      configTzTime(myConfig.timeTimeZone, myConfig.timeServer);
    }
    else {
      Serial.println("No Wifi credentials");
    }
  }
  // Play WAV
  playWav(DRADIS_WAV);

  delay(2000);

  M5.Displays(0).setBrightness(brightness);
  M5.Displays(0).fillScreen(TFT_DRADIS);
}

// Contact
void contact() {
  canvasSprite.fillSprite(0);
  // canvasSprite.drawRect(0, 0, 320 - (2 * shiftX), 220, TFT_BLUE);  // For debug

  if (WiFi.status() == WL_CONNECTED) {
    labelSprite.deleteSprite();
    labelSprite.setColorDepth(2);
    labelSprite.createSprite(170, 40);
    labelSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
    labelSprite.setFont(&digital_7__mono_24pt7b);
    labelSprite.drawString(localTime, 0, 0);
    labelSprite.pushSprite(&canvasSprite, 27, 176, 1);
  }

  labelSprite.setFont(0);
  labelSprite.setColor(TFT_WHITE);

  battlestarSprite.pushSprite(&canvasSprite, battlestarX - shiftX, battlestarY - shiftY + 15, 1);
  labelSprite.deleteSprite();
  labelSprite.setColorDepth(2);
  labelSprite.createSprite(60, 10);
  labelSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
  labelSprite.drawString("battlestar", 0, 0);
  labelSprite.pushSprite(&canvasSprite, battlestarX - shiftX - 21, battlestarY - shiftY + 40, 1);

  if (colonial1View) {
    colonial1Sprite.pushSprite(&canvasSprite, colonial1X - shiftX, colonial1Y - shiftY, 1);
    labelSprite.deleteSprite();
    labelSprite.setColorDepth(2);
    labelSprite.createSprite(48, 10);
    labelSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
    labelSprite.drawString("colonial", 0, 0);
    labelSprite.pushSprite(&canvasSprite, colonial1X - shiftX - 17, colonial1Y - shiftY + 15, 1);
  }

  if (colonial2View) {
    colonial2Sprite.pushSprite(&canvasSprite, colonial2X - shiftX, colonial2Y - shiftY, 1);
    labelSprite.deleteSprite();
    labelSprite.setColorDepth(2);
    labelSprite.createSprite(48, 10);
    labelSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
    labelSprite.drawString("colonial", 0, 0);
    labelSprite.pushSprite(&canvasSprite, colonial2X - shiftX - 17, colonial2Y - shiftY + 15, 1);
  }

  if (viperView) {
    if (viperNum == 1) {
      viperSprite.pushSprite(&canvasSprite, viperX - shiftX, viperY - shiftY, 1);
    }
    viperSprite.pushSprite(&canvasSprite, viperX - shiftX - 15, viperY - shiftY + 15, 1);
    viperSprite.pushSprite(&canvasSprite, viperX - shiftX + 15, viperY - shiftY + 15, 1);

    labelSprite.deleteSprite();
    labelSprite.setColorDepth(2);
    labelSprite.createSprite(34, 10);
    labelSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
    labelSprite.drawString("vipers", 0, 0);

    labelSprite.pushSprite(&canvasSprite, viperX - shiftX - 8, viperY - shiftY + 40, 1);
  }

  if (raptorView) {
    if (raptorNum == 1) {
      raptorSprite.pushSprite(&canvasSprite, raptorX - shiftX, raptorY - shiftY, 1);
    }
    raptorSprite.pushSprite(&canvasSprite, raptorX - shiftX - 15, raptorY - shiftY + 15, 1);
    raptorSprite.pushSprite(&canvasSprite, raptorX - shiftX + 15, raptorY - shiftY + 15, 1);

    labelSprite.deleteSprite();
    labelSprite.setColorDepth(2);
    labelSprite.createSprite(40, 10);
    labelSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
    labelSprite.drawString("raptors", 0, 0);

    labelSprite.pushSprite(&canvasSprite, raptorX - shiftX - 10, raptorY - shiftY + 40, 1);
  }

  if (raiderView) {
    raiderSprite.pushSprite(&canvasSprite, raiderX - shiftX, raiderY - shiftY, 1);
    raiderSprite.pushSprite(&canvasSprite, raiderX - shiftX - 15, raiderY - shiftY + 15, 1);
    raiderSprite.pushSprite(&canvasSprite, raiderX - shiftX + 15, raiderY - shiftY + 15, 1);
    if (raiderNum == 1) {
      raiderSprite.pushSprite(&canvasSprite, raiderX - shiftX, raiderY - shiftY + 30, 1);
    }

    labelSprite.deleteSprite();
    labelSprite.setColorDepth(2);
    labelSprite.createSprite(40, 10);
    labelSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
    labelSprite.drawString("raiders", 0, 0);

    labelSprite.pushSprite(&canvasSprite, raiderX - shiftX - 12, raiderY - shiftY - 10, 1);
  }

  if (!raiderView) {
    unknownSprite.pushSprite(&canvasSprite, unknownX - shiftX, unknownY - shiftY, 1);
    labelSprite.deleteSprite();
    labelSprite.setColorDepth(2);
    labelSprite.createSprite(60, 10);
    labelSprite.setPaletteColor(1, 0xFF0000U);  // Set palette
    labelSprite.drawString("unknown", 0, 0);

    labelSprite.pushSprite(&canvasSprite, unknownX - shiftX - 10, unknownY - shiftY - 10, 1);
  }
}

// Video
void video() {
  Serial.printf("Start %d kb %d kb %d kb %d kb\n", ESP.getHeapSize() / 1024, ESP.getFreeHeap() / 1024,
                ESP.getPsramSize() / 1024, ESP.getFreePsram() / 1024);

  uint8_t *mjpegBuf = (uint8_t *)malloc(MJPEG_BUFFER_SIZE);
  char *filename;

  Serial.printf("Malloc %d kb %d kb %d kb %d kb\n", ESP.getHeapSize() / 1024, ESP.getFreeHeap() / 1024,
                ESP.getPsramSize() / 1024, ESP.getFreePsram() / 1024);

  while (1) {
    switch (theme) {
      case 0:
        filename = (char *)DRADIS_VIDEO_GREY;
        break;

      case 1:
        filename = (char *)DRADIS_VIDEO_BLUE;
        break;

      case 2:
        filename = (char *)DRADIS_VIDEO_SEPIA;
        break;

      default:
        filename = (char *)DRADIS_VIDEO_GREY;
        break;
    }

    if (LittleFS.exists(filename)) {
      Serial.printf("%s IS on LittleFS\n", filename);
      mjpegFile = LittleFS.open(filename, FILE_READ);
    }

    if (!mjpegFile || mjpegFile.isDirectory()) {
      Serial.print("ERROR: Failed to open ");
      Serial.print(filename);
      Serial.println(" file for reading");
    } else {
      if (!mjpegBuf) {
        Serial.println(F("mjpegBuf malloc failed!"));
        // free(mjpegBuf);
      } else {
        Serial.println(F("MJPEG start"));
        mjpegClass.setup(&mjpegFile, mjpegBuf, mjpegDrawCallback, true, 0, 0, 320, 240);
        while (mjpegFile.available()) {
          // Read video
          mjpegClass.readMjpegBuf();

          // Render sprites
          contact();

          // Play video
          mjpegClass.drawJpg();

          if (themeChange) {
            playWav(DRADIS_WAV);
            theme++;
            if (theme > 2) theme = 0;
            preferences.putUInt("theme", theme);  // Save current theme
            break;
          }
        }
        Serial.println(F("MJPEG end"));
        mjpegFile.close();
        // free(mjpegBuf);
      }
      Serial.printf("Stop %d kb %d kb %d kb %d kb\n", ESP.getHeapSize() / 1024, ESP.getFreeHeap() / 1024,
                    ESP.getPsramSize() / 1024, ESP.getFreePsram() / 1024);
    }
  }
}
