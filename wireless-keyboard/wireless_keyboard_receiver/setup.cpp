#include <HardwareSerial.h>
#include <SD.h>
#include <SPI.h>

#include "./setup.h"

void setup_sd_card()
{
  Serial.println(F("[*] Starting SD card setup"));
  SPI.begin(SCK, MISO, MOSI, SS);
  while (!SD.begin(SS))
  {
    Serial.println(F("Card Mount Failed"));
    sleep(1);
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    Serial.println(F("No SD card attached"));
    return;
  }
  if (cardType != CARD_SD)
  {
    Serial.printf(F("UNKNOWN card type %d\n"), cardType);
    return;
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf(F("SD Card Size: %lluMB\n"), cardSize);
  Serial.println(F("[+] Finished SD card setup"));
}