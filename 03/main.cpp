#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../utils.h"

int main(int argc, char* argv[])
{
  // read file
  if (argc != 2)
  {
    std::cout << "Failed to read file, provide the path as argument." << std::endl;
    return -1;
  }

  std::string path = argv[1];
  auto stream = std::ifstream(path);
  if (!stream)
  {
    std::cout << "Failed to read file, can not open stream.";
    return -2;
  }

  static const int size = 12;
  size_t total          = 0;
  size_t cnt[size];
  std::fill(cnt, cnt + size, 0);

  std::string line;
  while (std::getline(stream, line))
  {
    line = trim(line);
    ++total;

    std::bitset<size> value(line);
    for (int pos = 0; pos < size; ++pos)
    {
      if (value[pos])
      {
        ++cnt[pos];
      }
    }
  }

  size_t half = total / 2;
  std::bitset<size> gama;
  std::bitset<size> epsilon;
  for (int pos = 0; pos < size; ++pos)
  {
    gama[pos] = cnt[pos] > half;
  }

  epsilon = ~gama;
  std::cout << "GAMA " << gama << " " << gama.to_ullong() << std::endl;
  std::cout << "EPSILON " << epsilon << " " << epsilon.to_ullong() << std::endl;
  std::cout << "POWER " << gama.to_ullong() * epsilon.to_ullong() << std::endl;

  return 0;
}
