#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../utils.h"


#if 0
static std::string path = "C:\\data\\projects\\advent\\03b\\example.txt";
static const int size   = 5;
#else
static std::string path = "C:\\data\\projects\\advent\\03b\\input.txt";
static const int size   = 12;
#endif

static std::bitset<size> calc(std::vector<std::bitset<size>> values, bool high)
{
  for (int pos = size - 1; pos >= 0; --pos)
  {
    if (values.size() == 1)
    {
      break;
    }

    size_t set =
        std::count_if(
            values.begin(), values.end(), [pos](const auto& value) { return value[pos] == 1; }) *
        10;

    size_t half = (values.size() * 10) / 2;
    bool common = set >= half;

    if (!high)
    {
      common = !common;
    }

    // remove all values which are not in common
    values.erase(std::remove_if(values.begin(),
                                values.end(),
                                [pos, common](const auto& value) { return value[pos] != common; }),
                 values.end());
  }

  return values[0];
}

int main(int argc, char* argv[])
{

  auto stream = std::ifstream(path);
  if (!stream)
  {
    std::cout << "Failed to read file, can not open stream.";
    return -2;
  }

  std::vector<std::bitset<size>> values;
  size_t cnt[size];
  std::fill(cnt, cnt + size, 0);

  std::string line;
  // read in values
  while (std::getline(stream, line))
  {
    line = trim(line);
    std::bitset<size> value(line);
    values.emplace_back(value);
    std::cout << "VALUE " << value << std::endl;
  }

  auto oxygen = calc(values, true);
  auto co2    = calc(values, false);

  std::cout << "oxygen " << oxygen << std::endl;
  std::cout << "co2 " << co2 << std::endl;
  std::cout << "life support rating " << oxygen.to_ullong() * co2.to_ullong() << std::endl;

  return 0;
}
