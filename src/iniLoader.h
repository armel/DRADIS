// Copyright (c) F4HWN Armel. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

// Ini error messages
void printErrorMessage(uint8_t e, bool eol = true) {
  char error[64];

  switch (e) {
    case IniFile::errorNoError:
      strcpy(error, "No error");
      break;
    case IniFile::errorFileNotFound:
      strcpy(error, "File not found");
      break;
    case IniFile::errorFileNotOpen:
      strcpy(error, "File not open");
      break;
    case IniFile::errorBufferTooSmall:
      strcpy(error, "Buffer too small");
      break;
    case IniFile::errorSeekError:
      strcpy(error, "Seek error");
      break;
    case IniFile::errorSectionNotFound:
      strcpy(error, "Section not found");
      break;
    case IniFile::errorKeyNotFound:
      strcpy(error, "Key not found");
      break;
    case IniFile::errorEndOfFile:
      strcpy(error, "End of file");
      break;
    case IniFile::errorUnknownError:
      strcpy(error, "Unknown error");
      break;
    default:
      strcpy(error, "Unknown error value");
      break;
  }

  Serial.println(error);

  if (eol) Serial.println();
}

// Ini log message
void iniLogMessage(char *message, uint16_t x = 160, uint16_t y = 50, boolean verbose = true) {
  if (verbose == true) {
    M5.Displays(0).drawString(message, x, y);
  } else {
    Serial.println(message);
  }
}

void iniParser(String fileNameSelected, char *media, boolean verbose) {
  const size_t bufferLen = 256;
  char buffer[bufferLen];

  IniFile ini(fileNameSelected.c_str(), FILE_READ, false);

  Serial.println(fileNameSelected.c_str());

  if (SD.begin(GPIO_NUM_4, SPI, 10000000)) {
    if (!SD.exists(fileNameSelected.c_str())) {
      iniLogMessage((char *)"Ini file does not exist.", 160, 50, verbose);
      vTaskDelay(pdMS_TO_TICKS(500));
      return;
    } else if (!ini.open()) {
      iniLogMessage((char *)"Ini file does not exist.", 160, 50, verbose);
      vTaskDelay(pdMS_TO_TICKS(500));
      return;
    }
    iniLogMessage((char *)"Find ini file.", 160, 50, verbose);
    vTaskDelay(pdMS_TO_TICKS(500));
  }

  if (!ini.validate(buffer, bufferLen)) {
    iniLogMessage((char *)"Ini file is not valid.", 160, 50, verbose);
    vTaskDelay(pdMS_TO_TICKS(500));
    printErrorMessage(ini.getError());
    return;
  } else {
    iniLogMessage((char *)"Ini file is valid.", 160, 50, verbose);
    vTaskDelay(pdMS_TO_TICKS(500));
  }

  // Common settings

  // Wifi settings

  Serial.println("----------");
  Serial.println("Wifi settings");
  Serial.println("----------");

  // Fetch wifi_ssid
  if (ini.getValue("wifi", "wifi_ssid", buffer, bufferLen)) {
    strncpy(myConfig.wifiSSID, buffer, 64);
    Serial.println(myConfig.wifiSSID);
  } else {
    Serial.print("Could not read 'wifi_ssid' from section 'wifi', error was ");
    printErrorMessage(ini.getError());
    iniLogMessage((char *)"Ini file parsing error.", 160, 50, verbose);
    return;
  }

  // Fetch wifi_password
  if (ini.getValue("wifi", "wifi_password", buffer, bufferLen)) {
    strncpy(myConfig.wifiPassword, buffer, 64);
    Serial.println(myConfig.wifiPassword);
  } else {
    Serial.print("Could not read 'wifi_password' from section 'wifi', error was ");
    printErrorMessage(ini.getError());
    iniLogMessage((char *)"Ini file parsing error.", 160, 50, verbose);
    return;
  }

  // Time settings

  Serial.println("----------");
  Serial.println("Time settings");
  Serial.println("----------");

  // Fetch time_server
  if (ini.getValue("time", "time_server", buffer, bufferLen)) {
    strncpy(myConfig.timeServer, buffer, 128);
    Serial.println(myConfig.timeServer);
  } else {
    Serial.print("Could not read 'time_server' from section 'time', error was ");
    printErrorMessage(ini.getError());
    iniLogMessage((char *)"Ini file parsing error.", 160, 50, verbose);
    // return;
  }

  // Fetch time_timezone
  if (ini.getValue("time", "time_timezone", buffer, bufferLen)) {
    strncpy(myConfig.timeTimeZone, buffer, 128);
    Serial.println(myConfig.timeTimeZone);
  } else {
    Serial.print("Could not read 'time_timezone' from section 'time', error was ");
    printErrorMessage(ini.getError());
    iniLogMessage((char *)"Ini file parsing error.", 160, 50, verbose);
    // return;
  }

  iniLogMessage((char *)"Ini file parsing is finished.", 160, 50, verbose);
  vTaskDelay(pdMS_TO_TICKS(500));
  SD.end();  // If not Bluetooth doesn't work !!!
}

// Ini Load
void iniLoad(boolean verbose) {
  iniLogMessage((char *)"Loading config file...", 160, 50, verbose);

  iniParser("/DRADIS.ini", (char *)"SD", verbose);

  return;
}

// Ini loader
void iniLoader() {
  SD.begin(GPIO_NUM_4, SPI, 10000000);

  iniLoad(false);
}