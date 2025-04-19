#include "class/msc/msc_device.h"
#ifndef STRATAGEMS_UTILS
#define STRATAGEMS_UTILS

#define STRATAGEMS_FILE "/stratagems.txt"

#define NUM_STRATAGEMS 4

class StratagemManager {
public:
  StratagemManager(const char *as);
  std::string get_stratagem(uint8_t button, uint8_t stratagem_slot);

private:
  std::string all_stratagems_raw;
  std::string stratagems_array[NUM_STRATAGEMS][2];
  std::string split_line(std::string in, uint8_t index);
};

StratagemManager::StratagemManager(const char *as) {
  Serial.printf(F("initialising with stratagems:\n%s\n"), as);
  this->all_stratagems_raw = std::string(as);
  std::string new_line = "\n";


  size_t nl = this->all_stratagems_raw.find(new_line);
  std::string line_1 = this->all_stratagems_raw.substr(0, nl);
  std::string line_1_0 = this->split_line(line_1, 0);
  std::string line_1_1 = this->split_line(line_1, 1);
  this->all_stratagems_raw = this->all_stratagems_raw.substr(
    nl + 1,
    this->all_stratagems_raw.length());
  Serial.printf(
    "line 1_0 %s, 1_1 %s : %d\n",
    line_1_0.c_str(),
    line_1_1.c_str(),
    nl);

  nl = this->all_stratagems_raw.find(new_line);
  std::string line_2 = this->all_stratagems_raw.substr(0, nl);
  std::string line_2_0 = this->split_line(line_2, 0);
  std::string line_2_1 = this->split_line(line_2, 1);
  this->all_stratagems_raw = this->all_stratagems_raw.substr(
    nl + 1,
    this->all_stratagems_raw.length());
  Serial.printf(
    "line 2_0 %s, 2_1 %s\n",
    line_2_0.c_str(),
    line_2_1.c_str());

  nl = this->all_stratagems_raw.find(new_line);
  std::string line_3 = this->all_stratagems_raw.substr(0, nl);
  std::string line_3_0 = this->split_line(line_3, 0);
  std::string line_3_1 = this->split_line(line_3, 1);
  this->all_stratagems_raw = this->all_stratagems_raw.substr(
    nl + 1,
    this->all_stratagems_raw.length());
  Serial.printf(
    "line 3_0 %s, 3_1 %s\n",
    line_3_0.c_str(),
    line_3_1.c_str());

  std::string line_4 = this->all_stratagems_raw;
  std::string line_4_0 = this->split_line(line_4, 0);
  std::string line_4_1 = this->split_line(line_4, 1);
  Serial.printf(
    "line 4_0 %s, 4_1 %s\n",
    line_4_0.c_str(),
    line_4_1.c_str());
}

std::string StratagemManager::split_line(std::string in, uint8_t index) {
  size_t idx = in.find(",");
  return index ? in.substr(idx + 1, in.length()) : in.substr(0, idx);
}

std::string StratagemManager::get_stratagem(uint8_t button, uint8_t stratagem_slot) {
  if (button + 1 > NUM_STRATAGEMS) {
    return "";
  }

  return stratagem_slot ? stratagems_array[button][1] : stratagems_array[button][0];
}

#endif