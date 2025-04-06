#include <DHT.h>
#include <WiFi.h>
#include <esp_now.h>

// include common functionality
#include <../common.h>

// at 100 we start getting 30% transmission failures
#define TRANSMISSION_INTERVAL 200

#define BUTTON_PIN 5

#define DHT22_PIN 21  // ESP32 pin GPIO21 connected to DHT22 sensor
DHT dht22(DHT22_PIN, DHT22);

// object to hold sensor readings
sensor_data readings;

// receiver MAC
uint8_t receiverMac[] = { 0x8c, 0x4f, 0x00, 0xad, 0x84, 0x3c };
// peer information
esp_now_peer_info_t peerInfo;


int read_humidity() {
  // read humidity
  float humi = dht22.readHumidity();
  // read temperature in Celsius
  float tempC = dht22.readTemperature();

  // check whether the reading is successful or not
  if (isnan(tempC) || isnan(humi)) {
    Serial.println("Failed to read from DHT22 sensor!");
    return 1;
  }

  readings.humidity = humi;
  readings.temperature = tempC;
  return 0;
}

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

  // read sensor information
  dht22.begin();  // initialize the DHT22 sensor


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

void send_reading() {
  int err = read_humidity();
  if (err) {
    Serial.println("error reading sensor data.");
  }

  esp_err_t result = esp_now_send(receiverMac, (uint8_t *)&readings, sizeof(readings));

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
    send_reading();
  }
}
