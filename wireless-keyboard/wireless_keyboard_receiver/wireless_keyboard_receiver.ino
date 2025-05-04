
#include <WiFi.h>
#include <esp_now.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
// include common functionality
#include "../common.h"
#include "stratagems.h"

#define WAIT_BETWEEN_KEY_PRESS 75

// USB Keyboard
USBHIDKeyboard Keyboard;


const char *stratagems;
StratagemManager *sm;
data_packet command;

// create debouncer
long lastSwitchDetectedMIllis;
const long multiTriggerPreventionInterval = 300;

void setup() {
  // serial for output
  Serial.begin(115200);

  // debouncer
  lastSwitchDetectedMIllis = millis();

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


  /*
          Initialise stratagem manager 
  */
  sm = new StratagemManager(stratagems);

  /**
  Keyboard Setup
  */
  // Initialize USB HID Keyboard
  Keyboard.begin();
  USB.begin();
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  /**
  USB EXPERIMENT
  */
  Serial.println("printed");
  Keyboard.write('A');
  blink_led();
  /*
  memcpy(&command, incomingData, sizeof(command));
  auto stgm = sm->get_stratagem(command.button_number, command.pin_position);
  Serial.printf(
    F("Bytes received: %d - button %d, pinmode %d, stratagem %s, command size %d\n"),
    len,
    command.button_number,
    command.pin_position,
    stgm.c_str(),
    stgm.length());
  */
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

void callStratagem(char primaryStratagem[], char secondaryStratagem[], int mode_pin) {
  if (millis() - lastSwitchDetectedMIllis < multiTriggerPreventionInterval) {
#ifdef DEBUG
    Serial.println("spotted multiple bounces");
#endif
    return;
  }

  lastSwitchDetectedMIllis = millis();

  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(50);                        // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW

  int p = digitalRead(mode_pin);
  char *s = p == 1 ? primaryStratagem : secondaryStratagem;

#ifdef DEBUG
  Serial.print(p);
  Serial.print(" ");
  Serial.println(s);
  return;
#endif

  Keyboard.press(KEY_LEFT_CTRL);
  delay(WAIT_BETWEEN_KEY_PRESS);

  for (int i = 0; i < strlen(s); i++) {
    char c = s[i];

    Keyboard.press(c);
    delay(WAIT_BETWEEN_KEY_PRESS);
    Keyboard.release(c);
    delay(WAIT_BETWEEN_KEY_PRESS);
  }

  // TODO (dio): figure out mouse library
  // Mouse.press();
  // delay(WAIT_BETWEEN_KEY_PRESS);
  // Mouse.release();

  Keyboard.releaseAll();
}

// needs to do nothing actively
void loop() {
}