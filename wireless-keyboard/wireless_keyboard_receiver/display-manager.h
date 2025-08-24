#ifndef _DISPLAY_MANAGER
#define _DISPLAY_MANAGER

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <string>

class DisplayManager
{
 private:
  Adafruit_SSD1306* display;
  int status_sd_card;
  int status_esp_now;
  std::string top_line;
  std::string bottom_line;
  int write();

 public:
  DisplayManager();
  int set_status_esp_now(int s);
  int set_status_sd_card(int s);
  int write_top_line(const char* input);
  int write_bottom_line(const char* input);
  int setup();
};

#endif
