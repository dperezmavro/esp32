#include <FS.h>
#include <HardwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <esp_now.h>

// include common functionality
#include "../common.h"
#include "./setup.h"

// data packet received
data_packet command;

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

void setuip_esp_now()
{
  Serial.println(F("[*] Starting ESP-NOW setup"));
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

  Serial.println(F("[+] Finished ESP-NOW setup"));
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len)
{
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

const char* read_file(fs::FS& fs, const char* path)
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