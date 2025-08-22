
#include <SD.h>

// #include <USBHID.h>
// #include "USBHIDKeyboard.h"
// USBHIDKeyboard Keyboard;

#include "./display-manager.h"
#include "./setup.h"
#include "./stratagem-manager.h"

const char* stratagems;
StratagemManager* sm;
DisplayManager* dm;

void setup()
{
  Serial.begin(115200);
  Serial.printf(F("[*] Starting setup\n"));

  dm         = new DisplayManager();
  int err_dm = dm->setup();
  if (err_dm)
  {
    Serial.printf("[-] display manager error %d\n", err_dm);
  }

  int err_sd = setup_sd_card();
  if (err_sd)
  {
    Serial.printf("[-] sd card error %d\n", err_sd);
  }

  int err_esp_now = setuip_esp_now();
  if (err_esp_now)
  {
    Serial.printf("[-] esp now error: %d\n", err_esp_now);
  }

  Serial.println(F("[+] Setup complete, reading stratagems"));

  stratagems = read_file(SD, STRATAGEMS_FILE);
  sm         = new StratagemManager(stratagems);
  sm->print();
}

// needs to do nothing actively
void loop() {}