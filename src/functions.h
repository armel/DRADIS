// Copyright (c) F4HWN Armel. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Pixel drawing callback
static int mjpegDrawCallback(JPEGDRAW *pDraw) {
  // Serial.printf("Draw pos = %d,%d. size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
  for (uint8_t d = 0; d < displayCount; d++) {
    M5.Displays(d).pushImage(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, pDraw->pPixels);
  }
  return 1;
}

// Fade all led
void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(100);
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

    if (counter == 1500) {
      counter = 0;
    }

    if (counter == 0) {
      viperView = random(0, 2);
      viperX    = random(-40, 40);
      viperY    = random(-40, 40);
      viperStep = random(-1, 2);

      raptorView = random(0, 2);
      raptorX    = random(-10, 10);
      raptorY    = random(-10, 10);
      raptorStep = random(-1, 2);

      raiderView = random(0, 2);
      raiderX    = random(-20, 20);
      raiderY    = random(-20, 20);
      raiderStep = random(-1, 2);

      unknownX    = random(-10, 10);
      unknownY    = random(-10, 10);
      unknownStep = random(-1, 2);
    }

    if (counter != 0 && counter % 100 == 0) {
      viperX += viperStep;
      viperY -= 1;

      raptorX += raptorStep;
      raptorY -= 1;

      raiderX -= raiderStep;
      raiderY += 1;

      unknownX -= unknownStep;
      unknownY += 1;
    }

    counter++;

    if (M5.getBoard() == m5::board_t::board_M5Stack || M5.getBoard() == m5::board_t::board_M5StackCore2) {
      step = 4;
      min  = 4;
      max  = 255;

      btnA = M5.BtnA.isPressed();
      btnB = M5.BtnB.isPressed();
      btnC = M5.BtnC.isPressed();
    } else if (M5.getBoard() == m5::board_t::board_M5StackCoreS3) {
      step = 16;
      min  = 64;
      max  = 255;

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
      wav = true;
      if(theme == 1)
        theme = 0;
      else
        theme = 1;
    } else {
      wav = false;
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
  for (uint8_t d = 0; d < displayCount; d++) {
    char string[64];

    if (!LittleFS.begin()) {
      Serial.println(F("ERROR: File System Mount Failed!"));
    } else {
      M5.Displays(d).drawPngFile(LittleFS, DRADIS_LOGO, 0, 120, 320, 90);
    }

    M5.Displays(d).setFont(&Ubuntu_Medium6pt7b);
    M5.Displays(d).setTextDatum(CC_DATUM);
    M5.Displays(d).setTextColor(TFT_WHITE, TFT_DRADIS);

    sprintf(string, "%s Version %s", NAME, VERSION);
    M5.Displays(d).drawString(string, 160, 20);

    sprintf(string, "by %s", AUTHOR);
    M5.Displays(d).drawString(string, 160, 35);
  }

  playWav(DRADIS_WAV);

  delay(2000);

  M5.Displays(0).setBrightness(brightness);
  M5.Displays(0).fillScreen(TFT_DRADIS);
}

// Video
void video() {
  uint8_t *mjpegBuf = (uint8_t *)malloc(MJPEG_BUFFER_SIZE);
  char* filename;

  while (1) {
    if(theme == 0)
      filename = (char *)DRADIS_VIDEO_GREY;
    else
      filename = (char *)DRADIS_VIDEO_SEPIA;

    if (LittleFS.exists(filename)) {
      Serial.printf("%s IS on LittleFS\n", filename);
      mjpegFile = LittleFS.open(filename, FILE_READ);
    }

    if (!mjpegFile || mjpegFile.isDirectory()) {
      Serial.print("ERROR: Failed to open ");
      Serial.print(filename);
      Serial.println(" file for reading");
    } else {
      // uint8_t *mjpegBuf = (uint8_t *)malloc(MJPEG_BUFFER_SIZE);

      if (!mjpegBuf) {
        Serial.println(F("mjpegBuf malloc failed!"));
        // free(mjpegBuf);
      } else {
        Serial.println(F("MJPEG start"));
        mjpegClass.setup(&mjpegFile, mjpegBuf, mjpegDrawCallback, true, 0, 0, 320, 240);
        while (mjpegFile.available()) {
          // Read video
          mjpegClass.readMjpegBuf();

          // Play video
          mjpegClass.drawJpg();

          if (viperView) {
            viperSprite.pushSprite(160 + viperX, 100 + viperY, 1);
            viperSprite.pushSprite(160 + viperX - 15, 100 + viperY + 15, 1);
            viperSprite.pushSprite(160 + viperX + 15, 100 + viperY + 15, 1);
            viperLabelSprite.pushSprite(160 + viperX - 8, 100 + viperY + 40, 1);
          }

          if (raptorView) {
            raptorSprite.pushSprite(160 + raptorX - 10, 160 + raptorY, 1);
            raptorSprite.pushSprite(160 + raptorX + 10, 160 + raptorY, 1);
            raptorLabelSprite.pushSprite(160 + raptorX - 8, 160 + raptorY + 25, 1);
          }

          if (raiderView) {
            raiderSprite.pushSprite(160 + raiderX, 40 + raiderY, 1);
            raiderSprite.pushSprite(160 + raiderX - 15, 40 + raiderY + 15, 1);
            raiderSprite.pushSprite(160 + raiderX + 15, 40 + raiderY + 15, 1);
            raiderSprite.pushSprite(160 + raiderX, 40 + raiderY + 30, 1);
            raiderLabelSprite.pushSprite(160 + raiderX - 12, 40 + raiderY - 10, 1);
          }

          if (!raiderView) {
            unknownSprite.pushSprite(140 + unknownX, 40 + unknownY, 1);
            unknownLabelSprite.pushSprite(140 + unknownX - 8, 40 + unknownY - 10, 1);
          }
        
          if (wav) {
            playWav(DRADIS_WAV);
            break;
          }
        }
        Serial.println(F("MJPEG end"));
        mjpegFile.close();
        // free(mjpegBuf);
      }
      Serial.printf("%d kb %d kb %d kb %d kb\n", ESP.getHeapSize() / 1024, ESP.getFreeHeap() / 1024,
                    ESP.getPsramSize() / 1024, ESP.getFreePsram() / 1024);
    }
  }
}