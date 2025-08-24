/*
  Sender is ESP8266
*/

#include "./common.h"
#include "./setup.h"

// #define BUTTON_PIN_0 12
// #define BUTTON_TOGGLE_0 14

// #define BUTTON_PIN_1 15
#define BUTTON_PIN_1 4
#define BUTTON_TOGGLE_1 5

void setup()
{
  // serial for debugging
  Serial.begin(115200);

  // button setup
  // pinMode(BUTTON_PIN_0, INPUT_PULLUP);
  // pinMode(BUTTON_TOGGLE_0, INPUT_PULLUP);

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
  // bool button_0 = digitalRead(BUTTON_PIN_0);     // Read switch
  // bool toggle_switch_0 = digitalRead(BUTTON_TOGGLE_0);  // Read toggle
  // if (button_0 == LOW)
  // {
  //   Serial.printf("clicked button %d: %d\n", button_0, toggle_switch_0);
  //   command.button_number = 0;
  //   command.pin_position  = toggle_switch_0;
  //   send_command(0, toggle_switch_0);
  // }

  bool button_1        = digitalRead(BUTTON_PIN_1);     // Read switch
  bool toggle_switch_1 = digitalRead(BUTTON_TOGGLE_1);  // Read toggle
  if (button_1 == LOW)
  {
    Serial.printf("clicked button %d: %d\n", button_1, toggle_switch_1);
    send_command(1, toggle_switch_1);
  }

  // delay(50);
}
