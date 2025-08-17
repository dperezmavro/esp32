#include "./stratagem-manager.h"
#include <cstdio>

StratagemManager::StratagemManager(const char* as)
{
  // Serial.printf(F("Initialising with stratagems:\n%s\n"), as);
  this->all_stratagems_raw = std::string(as);

  std::tuple<std::string, std::string> button_1 = this->read_line();
  std::tuple<std::string, std::string> button_2 = this->read_line();
  std::tuple<std::string, std::string> button_3 = this->read_line();
  std::tuple<std::string, std::string> button_4 = this->read_line();

  this->stratagem_pairs[0] = button_1;
  this->stratagem_pairs[1] = button_2;
  this->stratagem_pairs[2] = button_3;
  this->stratagem_pairs[3] = button_4;
}

std::tuple<std::string, std::string> StratagemManager::read_line()
{
  std::string new_line = "\n";
  size_t nl_idx        = this->all_stratagems_raw.find(new_line);
  std::string line     = this->all_stratagems_raw.substr(0, nl_idx);
  auto parts           = this->split_line(line);
  std::tuple<std::string, std::string> button{parts[0], parts[1]};
  this->all_stratagems_raw = this->all_stratagems_raw.substr(
      nl_idx + 1, this->all_stratagems_raw.length());
  return button;
}

std::vector<std::string> StratagemManager::split_line(std::string in)
{
  size_t idx = in.find(",");
  std::vector<std::string> parts{in.substr(0, idx),
                                 in.substr(idx + 1, in.length())};
  return parts;
}

std::string StratagemManager::get_stratagem(uint8_t button_number,
                                            uint8_t stratagem_slot)
{
  if (button_number + 1 > NUM_STRATAGEMS)
  {
    return NULL;
  }

  if (stratagem_slot > 1)
  {
    return NULL;
  }

  if (stratagem_slot)
  {
    return std::get<1>(this->stratagem_pairs[button_number]);
  }

  return std::get<0>(this->stratagem_pairs[button_number]);
}

void StratagemManager::print()
{
  for (int i = 0; i < NUM_STRATAGEMS; i++)
  {
    printf("Stratagems of %d: %s - %s\n",
           i,
           this->get_stratagem(i, 0).c_str(),
           this->get_stratagem(i, 1).c_str());
  }
}
