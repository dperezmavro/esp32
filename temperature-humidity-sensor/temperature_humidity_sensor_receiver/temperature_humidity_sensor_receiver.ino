#include <DHT.h>
#include <WiFi.h>
#include <esp_now.h>

// include common functionality
#include <../common.h>

sensor_data readings;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  // read mac address
  WiFi.mode(WIFI_STA);
  WiFi.STA.begin();

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
  memcpy(&readings, incomingData, sizeof(readings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.printf("Temperature: %.2f C, humidity %.2f%%\n", readings.temperature, readings.humidity);
}

void loop() {
}