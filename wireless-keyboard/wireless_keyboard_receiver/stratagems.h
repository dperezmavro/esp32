#include <stdint.h>
#include <tuple>
#ifndef STRATAGEMS_UTILS
#define STRATAGEMS_UTILS

#include <class/msc/msc_device.h>

#define STRATAGEMS_FILE "/stratagems.txt"
#define NUM_STRATAGEMS 4

class StratagemManager {
public:
  StratagemManager(const char *as);
  const char *get_stratagem(uint8_t button, uint8_t stratagem_slot);

private:
  std::string all_stratagems_raw;
  std::tuple<std::string, std::string> stratagem_pairs[NUM_STRATAGEMS];
  std::string split_line(std::string in, uint8_t index);
  std::tuple<std::string, std::string> read_line();
};

StratagemManager::StratagemManager(const char *as) {
  Serial.printf(F("initialising with stratagems:\n%s\n"), as);
  this->all_stratagems_raw = std::string(as);

  std::tuple<std::string, std::string> button_1 = this->read_line();
  std::tuple<std::string, std::string> button_2 = this->read_line();
  std::tuple<std::string, std::string> button_3 = this->read_line();
  std::tuple<std::string, std::string> button_4 = this->read_line();

  this->stratagem_pairs[0] = button_1;
  this->stratagem_pairs[1] = button_2;
  this->stratagem_pairs[2] = button_3;
  this->stratagem_pairs[3] = button_4;

  for (int i = 0; i < NUM_STRATAGEMS; i++) {
    Serial.printf(
      "%d: %s %s\n",
      i,
      std::get<0>(this->stratagem_pairs[i]).c_str(),
      std::get<1>(this->stratagem_pairs[i]).c_str());
  }
}

std::tuple<std::string, std::string> StratagemManager::read_line() {
  std::string new_line = "\n";
  size_t nl_idx = this->all_stratagems_raw.find(new_line);
  std::string line = this->all_stratagems_raw.substr(0, nl_idx);
  std::string line_0 = this->split_line(line, 0);
  std::string line_1 = this->split_line(line, 1);
  std::tuple<std::string, std::string> button{ line_0, line_1 };
  this->all_stratagems_raw = this->all_stratagems_raw.substr(
    nl_idx + 1,
    this->all_stratagems_raw.length());
  return button;
}

std::string StratagemManager::split_line(std::string in, uint8_t index) {
  size_t idx = in.find(",");
  return index ? in.substr(idx + 1, in.length()) : in.substr(0, idx);
}

const char *StratagemManager::get_stratagem(uint8_t button_number, uint8_t stratagem_slot) {
  if (button_number + 1 > NUM_STRATAGEMS) {
    return NULL;
  }

  if (stratagem_slot > 1) {
    return NULL;
  }

  if (stratagem_slot) {
    return std::get< 1 >(this->stratagem_pairs[button_number]).c_str();
  }

  return std::get< 0 >(this->stratagem_pairs[button_number]).c_str();
}

#endif