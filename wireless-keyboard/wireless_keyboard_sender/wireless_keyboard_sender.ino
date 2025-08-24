/*
  Sender is ESP8266
*/

#include "./common.h"
#include "./setup.h"

#define BUTTON_PIN_1 14
#define BUTTON_TOGGLE_1 12

#define BUTTON_PIN_2 15
#define BUTTON_TOGGLE_2 13

void setup()
{
  // serial for debugging
  Serial.begin(115200);

  // button setup
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_TOGGLE_1, INPUT_PULLUP);

  int err_esp_now = setuip_esp_now();
  if (!err_esp_now)
  {
    Serial.printf("[-] error setting up espnow: %d\n", err_esp_now);
  }
}

void loop()
{
  bool switchState1 = digitalRead(BUTTON_PIN_1);     // Read switch
  bool toggleState1 = digitalRead(BUTTON_TOGGLE_1);  // Read toggle

  bool switchState2 = digitalRead(BUTTON_PIN_2);     // Read switch
  bool toggleState2 = digitalRead(BUTTON_TOGGLE_2);  // Read toggle

  if (switchState1 == LOW)
  {
    Serial.printf("clicked button %d: %d\n", switchState1, toggleState1);
    command.button_number = switchState1;
    command.pin_position  = toggleState1;
    send_command();
  }

  if (switchState2 == LOW)
  {
    Serial.printf("clicked button %d: %d\n", switchState2, toggleState2);
    command.button_number = switchState2;
    command.pin_position  = toggleState2;
    send_command();
  }
}
