#pragma once

#include <exception>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace advent
{
std::string trim(std::string text);

std::vector<std::string> split(const std::string& original,
                               const std::string& delim,
                               bool trimSegment,
                               bool skipEmpty);


template <typename LAMBDA>
void readFile(const std::string& path, LAMBDA callback)
{
  auto stream = std::ifstream(path);
  if (!stream)
  {
    throw std::runtime_error(static_cast<std::ostringstream>(std::ostringstream()
                                                             << "Failed to read file '" << path
                                                             << "', can not open stream.")
                                 .str());
  }

  std::string line;
  while (std::getline(stream, line))
  {
    callback(trim(line));
  }
}

}  // namespace advent
