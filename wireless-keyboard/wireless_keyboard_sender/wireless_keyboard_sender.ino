#include <WiFi.h>
#include <esp_now.h>


// at 100 we start getting 30% transmission failures
#define TRANSMISSION_INTERVAL 200

#define BUTTON_PIN 5

// include common functionality
#include <../common.h>

data_packet command;

// receiver MAC
uint8_t receiverMac[] = { 0xE4, 0xB3, 0x23, 0xF7, 0xFF, 0xA4 };
// peer information
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // built in led for feedback
  pinMode(LED_BUILTIN, OUTPUT);

  // serial for debugging
  Serial.begin(115200);

  // button setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // read mac address
  WiFi.mode(WIFI_STA);
  WiFi.STA.begin();


  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);


  // Register peer
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void send_command() {
  esp_err_t result = esp_now_send(receiverMac, (uint8_t *)&command, sizeof(command));

  if (result == ESP_OK) {
    blink_led();
  } else {
    Serial.println("Error sending the data");
  }

  delay(TRANSMISSION_INTERVAL);
}

void loop() {
  bool switchState = digitalRead(BUTTON_PIN);  // Read switch

  if (switchState == LOW) {
    command.button_number = 123;
    command.pin_position = 1;
    send_command();
  }
}
