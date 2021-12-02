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

  size_t aim = 0;
  size_t x = 0;
  size_t y = 0;

  std::string line;
  // Get first line which should be containing the header
  while (std::getline(stream, line))
  {
    auto segments = split(line, " ", true, true);

    if (segments.size() != 2)
    {
      std::cout << "Failed to parse file, a line must contain two segemnts" << line << std::endl;
      return -3;
    }

    // read value
    size_t value;
    std::stringstream(segments[1]) >> value;

    if (segments[0] == "forward")
    {
      x += value;
      y += aim * value;
    }
    else if (segments[0] == "down")
    {
      aim += value;
    }
    else if (segments[0] == "up")
    {
      aim -= value;
    }
    else
    {
      std::cout << "Failed to parse file, missing direction" << line << std::endl;
      return -3;
    }
  }

  std::cout << "reached " << x << ":" << y << "=" << x*y << std::endl;

  return 0;
}
