#include <HardwareSerial.h>
#include <WiFi.h>
#include <esp_now.h>

// include common functionality
#include "./common.h"
#include "./setup.h"

// at 100 we start getting 30% transmission failures
#define TRANSMISSION_INTERVAL 200

#define ERR_ESP_NOW_NO_ERR 0;
#define ERR_ESP_NOW_INIT -1;
#define ERR_ESP_NOW_ADD_PEER -2;

// receiver MAC
uint8_t receiverMac[] = {0xB4, 0x3A, 0x45, 0xA9, 0x1C, 0x0C};

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status)
{
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == 0 ? "Delivery Success" : "Delivery Fail");
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

  esp_now_register_send_cb((esp_now_send_cb_t)OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return ERR_ESP_NOW_ADD_PEER;
  }

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