
#include <WiFi.h>
#include <esp_now.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

// #include <USBHID.h>
// #include "USBHIDKeyboard.h"
// USBHIDKeyboard Keyboard;

// include common functionality
#include "../common.h"
#include "stratagems.h"

const char *stratagems;
data_packet command;

void setup() {
  // serial for output
  Serial.begin(115200);

  // LED for feedBACK
  pinMode(LED_BUILTIN, OUTPUT);

  /*
  
  ESP NOW AND WIFI SETUP

  */
  // read mac address
  WiFi.mode(WIFI_STA);
  WiFi.STA.begin();
  // esp32s3 receiver E4:B3:23:F7:FF:A4
  Serial.printf(F("MAC Address: %s\n"), WiFi.macAddress().c_str());
  if (esp_now_init() != ESP_OK) {
    Serial.println(F("Error initializing ESP-NOW"));
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));


  /*
  
  SD card setup

  */
  if (!SD.begin()) {
    Serial.println(F("Card Mount Failed"));
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println(F("No SD card attached"));
    return;
  }
  if (cardType != CARD_SD) {
    Serial.printf(F("UNKNOWN card type %d\n"), cardType);
    return;
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf(F("SD Card Size: %lluMB\n"), cardSize);

  stratagems = readFile(SD, STRATAGEMS_FILE);
  StratagemManager sm(stratagems);
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  blink_led();
  memcpy(&command, incomingData, sizeof(command));
  Serial.print(F("Bytes received: "));
  Serial.println(len);
  Serial.printf(F("button %d, pinmode %d\n"), command.button_number, command.pin_position);

  StratagemManager sm(stratagems);

  // Keyboard.print("You pressed the button ");
  // Keyboard.print(command.button_number);
  // Keyboard.println(".");
}


const char *readFile(fs::FS &fs, const char *path) {
  Serial.printf(F("Opening file: %s\n"), path);

  File file = fs.open(path);
  if (!file) {
    Serial.println(F("Failed to open file for reading"));
    return NULL;
  }

  if (!file.available()) {
    Serial.println(F("File is not available"));
    return NULL;
  }
  Serial.println(F("Reading from file"));
  String stratagems = file.readString();
  file.close();

  return stratagems.c_str();
}

// needs to do nothing actively
void loop() {
}