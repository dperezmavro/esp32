#include <DHT.h>
#include <WiFi.h>
#include <esp_now.h>

// include common functionality
#include <../common.h>
#include <stratagems.h>

data_packet command;

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  // read mac address
  WiFi.mode(WIFI_STA);
  WiFi.STA.begin();
  //   big B4:3A:45:A9:1C:0C
  // small E4:B3:23:F7:FF:A4
  Serial.println("  MAC Address: " + WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  blink_led();
  memcpy(&command, incomingData, sizeof(command));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.printf("button %d, pinmode %d\n", command.button_number, command.pin_position);
}

void loop() {
  // Serial.println("loop: MAC Address: " + WiFi.macAddress());
  // delay(1000);
}