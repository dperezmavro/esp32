#ifndef _COMMON_H
#define _COMMON_H

#define LED_BUILTIN 48
#define LED_BLINK_DURATION 250

struct data_packet {
  uint8_t button_number;
  uint8_t pin_position;
};

void blink_led() {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(LED_BLINK_DURATION);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
}

#endif