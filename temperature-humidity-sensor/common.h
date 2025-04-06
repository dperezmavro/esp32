#define LED_BUILTIN 2
#define LED_BLINK_DURATION 250

struct sensor_data {
  float humidity;
  float temperature;
};

void blink_led() {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(LED_BLINK_DURATION);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
}