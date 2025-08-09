
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <esp_now.h>

// #include <USBHID.h>
// #include "USBHIDKeyboard.h"
// USBHIDKeyboard Keyboard;

// include common functionality
#include "../common.h"
#include "stratagems.h"

const char* stratagems;
StratagemManager* sm;
data_packet command;

void setup()
{
  Serial.begin(115200);
  Serial.printf(F("[*] Starting setup\n"));

  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK: ");
  Serial.println(SCK);
  Serial.print("SS: ");
  Serial.println(SS);

  setup_sd_card();
  setuip_esp_now();

  // stratagems = readFile(SD, STRATAGEMS_FILE);
  // sm = new StratagemManager(stratagems);
}

void setuip_esp_now()
{
  WiFi.mode(WIFI_STA);
  WiFi.STA.begin();
  // esp32s3 receiver E4:B3:23:F7:FF:A4
  Serial.printf(F("MAC Address: %s\n"), WiFi.macAddress().c_str());
  if (esp_now_init() != ESP_OK)
  {
    Serial.println(F("Error initializing ESP-NOW"));
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void setup_sd_card()
{
  while (!SD.begin())
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
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len)
{
  blink_led();
  memcpy(&command, incomingData, sizeof(command));
  auto stgm = sm->get_stratagem(command.button_number, command.pin_position);
  Serial.printf(F("Bytes received: %d - button %d, pinmode %d, stratagem %s, "
                  "command size %d\n"),
                len,
                command.button_number,
                command.pin_position,
                stgm.c_str(),
                stgm.length());

  // Keyboard.print("You pressed the button ");
  // Keyboard.print(command.button_number);
  // Keyboard.println(".");
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

  return stratagems.c_str();
}

// needs to do nothing actively
void loop() {}