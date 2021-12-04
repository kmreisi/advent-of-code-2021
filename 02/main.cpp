// MIT License
//
// Copyright (c) 2021 kmreisi
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "utils.h"

#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace advent::d02
{
enum class Direction
{
  FORWARD,
  DOWN,
  UP
};
static Direction toDirection(const std::string& text)
{
  if (text == "forward")
  {
    return Direction::FORWARD;
  }
  else if (text == "down")
  {
    return Direction::DOWN;
  }
  else if (text == "up")
  {
    return Direction::UP;
  }

  throw std::runtime_error(static_cast<std::ostringstream>(
                               std::ostringstream() << "Failed to parse direction " << text << ".")
                               .str());
}

struct Command
{
  Direction direction;
  size_t value;

  static Command parse(const std::string& line)
  {
    auto segments = split(line, " ", true, true);

    if (segments.size() != 2)
    {
      throw std::runtime_error(
          static_cast<std::ostringstream>(
              std::ostringstream()
              << "Failed to parse file, a line must contain two segemnts, line: " << line << ".")
              .str());
    }

    // read value
    size_t value;
    std::stringstream(segments[1]) >> value;

    return Command{toDirection(segments[0]), value};
  }
};

static void run_a(const std::string& prefix, const std::vector<Command>& input)
{
  size_t x = 0;
  size_t y = 0;

  for (const auto& command : input)
  {
    switch (command.direction)
    {
      case Direction::FORWARD:
        x += command.value;
        break;
      case Direction::DOWN:
        y += command.value;
        break;
      case Direction::UP:
        y -= command.value;
        break;
    }
  }
  std::cout << prefix << ": reached " << x << ":" << y << "=" << x * y << std::endl;
}

static void run_b(const std::string& prefix, const std::vector<Command>& input)
{
  size_t aim = 0;
  size_t x   = 0;
  size_t y   = 0;

  for (const auto& command : input)
  {
    switch (command.direction)
    {
      case Direction::FORWARD:
        x += command.value;
        y += aim * command.value;
        break;
      case Direction::DOWN:
        aim += command.value;
        break;
      case Direction::UP:
        aim -= command.value;
        break;
    }
  }

  std::cout << prefix << ": reached " << x << " : " << y << " = " << x * y << std::endl;
}

}  // namespace advent::d02

int main(int argc, char* argv[])
{
  std::vector<advent::d02::Command> exampleValues;
  advent::readFile("example.txt",
                   [&exampleValues](const auto& line)
                   { exampleValues.emplace_back(advent::d02::Command::parse(line)); });

  std::vector<advent::d02::Command> inputValues;
  advent::readFile("input.txt",
                   [&inputValues](const auto& line)
                   { inputValues.emplace_back(advent::d02::Command::parse(line)); });

  std::cout << "02A________________________" << std::endl;
  advent::d02::run_a("02A: EX", exampleValues);
  advent::d02::run_a("02A: IN", inputValues);

  std::cout << "\n02B________________________" << std::endl;
  advent::d02::run_b("02B: EX", exampleValues);
  advent::d02::run_b("02B: IN", inputValues);

  return 0;
}
