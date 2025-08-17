#ifndef _STRATAGEMS_H
#define _STRATAGEMS_H

#include <class/msc/msc_device.h>
#include <stdint.h>
#include <string>
#include <tuple>
#include <vector>

#define STRATAGEMS_FILE "/stratagems.txt"
#define NUM_STRATAGEMS 4

class StratagemManager
{
 public:
  StratagemManager(const char* as);
  std::string get_stratagem(uint8_t button, uint8_t stratagem_slot);
  void print();

 private:
  std::string all_stratagems_raw;
  std::tuple<std::string, std::string> stratagem_pairs[NUM_STRATAGEMS];
  std::vector<std::string> split_line(std::string in);
  std::tuple<std::string, std::string> read_line();
};

#endif