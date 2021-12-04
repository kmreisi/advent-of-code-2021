#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../utils.h"


#if 0
static std::string INPUT_PATH = "C:\\data\\projects\\advent\\03b\\example.txt";
static const int INPUT_WIDTH   = 5;
#else
static std::string INPUT_PATH = "C:\\data\\projects\\advent\\03b\\input.txt";
static const int INPUT_WIDTH  = 12;
#endif

static std::bitset<INPUT_WIDTH> calc(std::vector<std::bitset<INPUT_WIDTH>> values, bool high)
{
  for (int pos = INPUT_WIDTH - 1; pos >= 0; --pos)
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

  auto stream = std::ifstream(INPUT_PATH);
  if (!stream)
  {
    std::cout << "Failed to read file, can not open stream.";
    return -2;
  }

  std::vector<std::bitset<INPUT_WIDTH>> values;

  std::string line;
  // read in values
  while (std::getline(stream, line))
  {
    line = trim(line);
    std::bitset<INPUT_WIDTH> value(line);
    values.emplace_back(value);
  }

  {
    // 03
    size_t half = values.size() / 2;
    std::bitset<INPUT_WIDTH> gama;
    std::bitset<INPUT_WIDTH> epsilon;

    size_t cnt[INPUT_WIDTH];
    std::fill(cnt, cnt + INPUT_WIDTH, 0);

    for (const auto& value : values)
    {
      for (int pos = 0; pos < INPUT_WIDTH; ++pos)
      {
        if (value[pos])
        {
          ++cnt[pos];
        }
      }
    }

    for (int pos = 0; pos < INPUT_WIDTH; ++pos)
    {
      gama[pos] = cnt[pos] > half;
    }
    epsilon = ~gama;

    std::cout << "03: gama " << gama << std::endl;
    std::cout << "03: epsilon " << epsilon << std::endl;
    std::cout << "03: power " << gama.to_ullong() * epsilon.to_ullong() << std::endl;
  }

  {
    // 03b
    auto oxygen = calc(values, true);
    auto co2    = calc(values, false);

    std::cout << "03b: oxygen " << oxygen << std::endl;
    std::cout << "03b: co2 " << co2 << std::endl;
    std::cout << "03b: life support rating " << oxygen.to_ullong() * co2.to_ullong() << std::endl;
  }

  return 0;
}
