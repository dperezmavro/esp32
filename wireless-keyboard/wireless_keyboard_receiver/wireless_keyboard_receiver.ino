#include <SD.h>
// #include <format>

// #include <USBHID.h>
// #include "USBHIDKeyboard.h"
// USBHIDKeyboard Keyboard;

#include "./display-manager.h"
#include "./setup.h"
#include "./stratagem-manager.h"

const char* stratagems;
StratagemManager* sm;
DisplayManager* dm;
int loop_counter = 0;

void setup()
{
  Serial.begin(115200);
  Serial.printf(F("[*] Starting setup\n"));

  dm         = new DisplayManager();
  int err_dm = dm->setup();
  if (err_dm)
  {
    Serial.printf(F("[-] display manager error %d\n"), err_dm);
  }

  int err_sd = setup_sd_card();
  if (err_sd)
  {
    Serial.printf(F("[-] sd card error %d\n"), err_sd);
  }
  dm->set_status_sd_card(err_sd);

  int err_esp_now = setuip_esp_now();
  if (err_esp_now)
  {
    Serial.printf(F("[-] esp now error: %d\n"), err_esp_now);
  }
  dm->set_status_esp_now(err_esp_now);

  stratagems = read_file(SD, STRATAGEMS_FILE);
  sm         = new StratagemManager(stratagems);
  sm->print();
}

// needs to do nothing actively
void loop()
{
  loop_counter += 1;
  loop_counter %= 4;
  if (!loop_counter)
  {
    dm->write_bottom_line("");
  }

  delay(500);
}