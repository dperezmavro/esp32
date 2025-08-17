
#include <FS.h>
#include <SD.h>

// #include <USBHID.h>
// #include "USBHIDKeyboard.h"
// USBHIDKeyboard Keyboard;

#include "./setup.h"
#include "./stratagem-manager.h"

const char* stratagems;
StratagemManager* sm;

void setup()
{
  Serial.begin(115200);
  Serial.printf(F("[*] Starting setup\n"));

  setup_sd_card();
  setuip_esp_now();

  Serial.println(F("[+] Setup complete, reading stratagems"));

  stratagems = readFile(SD, STRATAGEMS_FILE);
  sm         = new StratagemManager(stratagems);
  sm->print();
}

const char* readFile(fs::FS& fs, const char* path)
{
  Serial.printf(F("Opening file: %s\n"), path);

  File file = fs.open(path);
  if (!file)
  {
    Serial.println(F("Failed to open file for reading"));
    return NULL;
  }

  if (!file.available())
  {
    Serial.println(F("File is not available"));
    return NULL;
  }
  Serial.println(F("Reading from file"));
  String stratagems = file.readString();
  file.close();

  Serial.println(F("Stratagems read correctly"));

  return stratagems.c_str();
}

// needs to do nothing actively
void loop() {}