#include <ESP8266WiFi.h>
#include <HardwareSerial.h>
#include <espnow.h>

// include common functionality
#include "./common.h"
#include "./setup.h"

// at 100 we start getting 30% transmission failures
#define TRANSMISSION_INTERVAL 200

#define ERR_ESP_NOW_NO_ERR 0;
#define ERR_ESP_NOW_INIT -1;

// receiver MAC
uint8_t receiverMac[] = {0xB4, 0x3A, 0x45, 0xA9, 0x1C, 0x0C};

// callback when data is sent
void OnDataSent(uint8_t* mac_addr, uint8_t sendStatus)
{
  Serial.print("Last Packet Send Status: ");
  Serial.println(sendStatus == 0 ? "Delivery Success" : "Delivery Fail");
}

int setuip_esp_now()
{
  // read mac address
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP-NOW");
    return ERR_ESP_NOW_INIT;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(receiverMac, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  return ERR_ESP_NOW_NO_ERR;
}

void send_command(int btn, int toggle)
{
  data_packet cmd;
  cmd.button_number = btn;
  cmd.pin_position  = toggle;
  esp_now_send(receiverMac, (uint8_t*)&cmd, sizeof(cmd));
  delay(TRANSMISSION_INTERVAL);
}