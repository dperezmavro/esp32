/*
  Sender is ESP8266
*/

#include <ESP8266WiFi.h>
#include <espnow.h>

// at 100 we start getting 30% transmission failures
#define TRANSMISSION_INTERVAL 200

#define BUTTON_PIN 14

// include common functionality
// #include <../common.h>
#ifndef LED_BUTTON_FEEDBACK
#define LED_BUTTON_FEEDBACK 2
#endif
#define EXTERNAL_LED 13
#define LED_BLINK_DURATION 250

struct data_packet
{
  uint8_t button_number;
  uint8_t pin_position;
};

void blink_led()
{
  digitalWrite(LED_BUTTON_FEEDBACK,
               HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(EXTERNAL_LED,
               HIGH);         // turn the LED on (HIGH is the voltage level)
  delay(LED_BLINK_DURATION);  // wait for a second
  digitalWrite(EXTERNAL_LED,
               LOW);  // turn the LED off by making the voltage LOW
}

data_packet command;

// receiver MAC
uint8_t receiverMac[] = {0xE4, 0xB3, 0x23, 0xF7, 0xFF, 0xA4};

// callback when data is sent
void OnDataSent(uint8_t* mac_addr, uint8_t sendStatus)
{
  Serial.print("Last Packet Send Status: ");
  Serial.println(sendStatus == 0 ? "Delivery Success" : "Delivery Fail");
}

void setup()
{
  // built in led for feedback
  pinMode(LED_BUTTON_FEEDBACK, OUTPUT);
  pinMode(EXTERNAL_LED, OUTPUT);

  // serial for debugging
  Serial.begin(115200);

  // button setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // read mac address
  WiFi.mode(WIFI_STA);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(receiverMac, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  // feedback to know everything booted fine
  blink_builtin_led(100);
  blink_builtin_led(100);
  blink_builtin_led(100);
  blink_builtin_led(100);
  blink_builtin_led(100);
  blink_builtin_led(100);
}

void blink_builtin_led(uint8_t d)
{
  digitalWrite(LED_BUTTON_FEEDBACK,
               LOW);  // turn the LED on (HIGH is the voltage level)
  delay(d);
  digitalWrite(LED_BUTTON_FEEDBACK,
               HIGH);  // turn the LED on (HIGH is the voltage level)
}

void send_command()
{
  digitalWrite(EXTERNAL_LED,
               HIGH);  // turn the LED on (HIGH is the voltage level)
  esp_now_send(receiverMac, (uint8_t*)&command, sizeof(command));
  delay(TRANSMISSION_INTERVAL);
  digitalWrite(EXTERNAL_LED,
               LOW);  // turn the LED off by making the voltage LOW
}

void loop()
{
  bool switchState = digitalRead(BUTTON_PIN);  // Read switch

  if (switchState == LOW)
  {
    Serial.println("clicked button");
    // TODO (dio): populate these with real reads
    command.button_number = 2;
    command.pin_position  = 1;
    send_command();
  }
}
