#include <DHT.h>
#include <WiFi.h>
#include <esp_wifi.h>

#define DHT22_PIN 21  // ESP32 pin GPIO21 connected to DHT22 sensor
DHT dht22(DHT22_PIN, DHT22);

void readMacAddress() {
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  } else {
    Serial.println("Failed to read MAC address");
  }
}

void setup() {
  Serial.begin(115200);

  // read mac address
  WiFi.mode(WIFI_STA);
  WiFi.STA.begin();
  Serial.print("\n[DEFAULT] ESP32 Board MAC Address: ");
  readMacAddress();

  // read sensor information
  dht22.begin();  // initialize the DHT22 sensor
}

void loop() {
  // read humidity
  float humi = dht22.readHumidity();
  // read temperature in Celsius
  float tempC = dht22.readTemperature();

  // check whether the reading is successful or not
  if (isnan(tempC) || isnan(humi)) {
    Serial.println("Failed to read from DHT22 sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C\n");
  }

  // wait a 2 seconds between readings
  delay(2000);
}