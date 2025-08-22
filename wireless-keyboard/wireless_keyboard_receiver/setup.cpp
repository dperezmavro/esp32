#include <FS.h>
#include <HardwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <esp_now.h>

// include common functionality
#include "../common.h"
#include "./setup.h"

#define ERR_SD_SETUP_NO_SD_CARD -1
#define ERR_SD_SETUP_UNKNOWN_CARD -2
#define ERR_SD_SETUP_BEGIN_FAILED -3
#define ERR_SD_SETUP_NO_ERR 0;
#define ERR_ESP_NOW_NO_ERR 0;
#define ERR_ESP_NOW_INIT -1;

// data packet received
data_packet command;

int setup_sd_card()
{
  SPI.begin(SCK, MISO, MOSI, SS);
  if (!SD.begin(SS))
  {
    return ERR_SD_SETUP_BEGIN_FAILED;
  }

  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    return ERR_SD_SETUP_NO_SD_CARD;
  }
  if (cardType != CARD_SD)
  {
    Serial.printf(F("[-] UNKNOWN card type %d\n"), cardType);
    return ERR_SD_SETUP_UNKNOWN_CARD;
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf(F("SD Card Size: %lluMB\n"), cardSize);
  return ERR_SD_SETUP_NO_ERR;
}

int setuip_esp_now()
{
  WiFi.mode(WIFI_STA);
  WiFi.STA.begin();

  Serial.printf(F("MAC Address: %s\n"), WiFi.macAddress().c_str());
  if (esp_now_init() != ESP_OK)
  {
    return ERR_ESP_NOW_INIT;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  return ERR_ESP_NOW_NO_ERR;
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len)
{
  memcpy(&command, incomingData, sizeof(command));
  std::string stgm =
      sm->get_stratagem(command.button_number, command.pin_position);
  if (stgm.empty())
  {
    Serial.printf("[-] received invalida stratagem code: %d:%d\n",
                  command.button_number,
                  command.pin_position);
    return;
  }
  Serial.printf(F("Bytes received: %d - button %d, pinmode %d, stratagem %s, "
                  "command size %d\n"),
                len,
                command.button_number,
                command.pin_position,
                stgm.c_str(),
                stgm.length());

  dm->write_bottom_line(stgm.c_str());

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