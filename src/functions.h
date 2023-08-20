// Copyright (c) F4HWN Armel. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Pixel drawing callback
static int mjpegDrawCallback(JPEGDRAW *pDraw) {
  clipSprite.deleteSprite();
  clipSprite.createSprite(pDraw->iWidth, pDraw->iHeight);
  clipSprite.pushImage(0, 0, pDraw->iWidth, pDraw->iHeight, pDraw->pPixels);
  canvasSprite.pushSprite(&clipSprite, -pDraw->x + 60, -pDraw->y + 20, 0);

  for (uint8_t d = 0; d < displayCount; d++) {
    clipSprite.pushSprite(&M5.Displays(d), pDraw->x, pDraw->y);
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

    if (counter == 600) {
      counter = 0;
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
    }

    if (counter != 0 && counter % 100 == 0) {
      viperX += viperStep * 4;
      viperY -= 4;

      raptorX += raptorStep;
      raptorY -= 1;

      battlestarX += battlestarStep;
      battlestarY -= 1;

      raiderX += raiderStep;
      raiderY += 1;

      unknownX += unknownStep;
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
      M5.Displays(d).drawPngFile(LittleFS, DRADIS_LOGO, 0, 75, 320, 90);
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

// Contact
void contact() {
  uint8_t shiftX = 60;
  uint8_t shiftY = 20;

  canvasSprite.fillSprite(0);
  //canvasSprite.drawRect(0, 0, 220, 170, TFT_BLUE); // For debug

  battlestarSprite.pushSprite(&canvasSprite, battlestarX - shiftX, battlestarY - shiftY + 15, 1);
  battlestarLabelSprite.pushSprite(&canvasSprite, battlestarX - shiftX - 21, battlestarY - shiftY + 40, 1);

  if (viperView) {
    if (viperNum == 1) {
      viperSprite.pushSprite(&canvasSprite, viperX - shiftX, viperY - shiftY, 1);
    }
    viperSprite.pushSprite(&canvasSprite, viperX - shiftX - 15, viperY - shiftY + 15, 1);
    viperSprite.pushSprite(&canvasSprite, viperX - shiftX + 15, viperY - shiftY + 15, 1);
    viperLabelSprite.pushSprite(&canvasSprite, viperX - shiftX - 8, viperY - shiftY + 40, 1);
  }

  if (raptorView) {
    if (raptorNum == 1) {
      raptorSprite.pushSprite(&canvasSprite, raptorX - shiftX, raptorY - shiftY, 1);
    }
    raptorSprite.pushSprite(&canvasSprite, raptorX - shiftX - 15, raptorY - shiftY + 15, 1);
    raptorSprite.pushSprite(&canvasSprite, raptorX - shiftX + 15, raptorY - shiftY + 15, 1);
    raptorLabelSprite.pushSprite(&canvasSprite, raptorX - shiftX - 10, raptorY - shiftY + 40, 1);
  }

  if (raiderView) {
    raiderSprite.pushSprite(&canvasSprite, raiderX - shiftX, raiderY - shiftY, 1);
    raiderSprite.pushSprite(&canvasSprite, raiderX - shiftX - 15, raiderY - shiftY + 15, 1);
    raiderSprite.pushSprite(&canvasSprite, raiderX - shiftX + 15, raiderY - shiftY + 15, 1);
    if (raiderNum == 1) {
      raiderSprite.pushSprite(&canvasSprite, raiderX - shiftX, raiderY - shiftY + 30, 1);
    }
    raiderLabelSprite.pushSprite(&canvasSprite, raiderX - shiftX - 12, raiderY - shiftY - 10, 1);
  }

  if (!raiderView) {
    unknownSprite.pushSprite(&canvasSprite, unknownX - shiftX, unknownY - shiftY, 1);
    unknownLabelSprite.pushSprite(&canvasSprite, unknownX - shiftX - 8, unknownY - shiftY - 10, 1);
  }
}

// Video
void video() {
  uint8_t *mjpegBuf = (uint8_t *)malloc(MJPEG_BUFFER_SIZE);
  char *filename;

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

          if (wav) {
            playWav(DRADIS_WAV);
            theme++;
            if (theme > 2) theme = 0;
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
