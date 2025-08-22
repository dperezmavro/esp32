#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <string>

#include "./display-manager.h"

#define ERR_DM_NO_ERR 0
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels
#define OLED_RESET -1     // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS \
  0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

DisplayManager::DisplayManager()
{
  this->display =
      new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
}

int DisplayManager::setup()
{

  if (!this->display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  this->display->display();
  delay(2000);  // Pause for 2 seconds

  // Clear the buffer
  this->display->clearDisplay();

  this->display->clearDisplay();

  this->display->setTextSize(1);               // Normal 1:1 pixel scale
  this->display->setTextColor(SSD1306_WHITE);  // Draw white text
  this->display->setCursor(0, 0);              // Start at top-left corner
  this->display->println(F("SD Card=OK ESP-NOW=OK"));

  this->display->setTextSize(3);  // Draw 2X-scale text
  this->display->setTextColor(SSD1306_WHITE);
  this->display->print(F("WSAASAAD"));

  this->display->display();
  return ERR_DM_NO_ERR;
}

int DisplayManager::write_top_line(const char* input)
{
  this->top_line = std::string(input);
  return this->write();
}

int DisplayManager::write_bottom_line(const char* input)
{
  this->bottom_line = std::string(input);
  return this->write();
}

int DisplayManager::write()
{
  return 0;
}