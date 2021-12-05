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

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

namespace advent::d05
{
struct Coordinate
{
  size_t x;
  size_t y;

  static Coordinate parse(const std::string& segment)
  {
    auto x_y = split(segment, ",", true, true);
    if (x_y.size() != 2)
    {
      throw std::runtime_error(
          static_cast<std::ostringstream>(
              std::ostringstream()
              << "Failed to parse coordinate, could not split by ',', segment: " << segment << ".")
              .str());
    }
    size_t x;
    std::stringstream(x_y[0]) >> x;
    size_t y;
    std::stringstream(x_y[1]) >> y;

    return Coordinate{x, y};
  }
};

struct Line
{
  Coordinate a;
  Coordinate b;

  bool isHorizontal() const
  {
    return a.y == b.y;
  }

  bool isVertical() const
  {
    return a.x == b.x;
  }

  bool isStraight() const
  {
    return isHorizontal() || isVertical();
  }

  static std::shared_ptr<Line> parse(const std::string& line)
  {
    auto a_b = split(line, "->", true, true);
    if (a_b.size() != 2)
    {
      throw std::runtime_error(
          static_cast<std::ostringstream>(std::ostringstream()
                                          << "Failed to parse line, could not split by '->', line: "
                                          << line << ".")
              .str());
    }

    Coordinate a = Coordinate::parse(a_b[0]);
    Coordinate b = Coordinate::parse(a_b[1]);
    return std::make_shared<Line>(std::move(Line{std::move(a), std::move(b)}));
  }
};

void run_a(const std::string& prefix, const std::vector<std::shared_ptr<Line>>& lines)
{
  // filter horizontal / vertical lines
  std::vector<std::shared_ptr<Line>> filtered;
  std::copy_if(lines.begin(),
               lines.end(),
               std::back_inserter(filtered),
               [](const auto& line) { return line->isStraight(); });

  // find size of diagram
  size_t width  = 0;
  size_t height = 0;
  for (const auto& line : filtered)
  {
    width  = line->a.x > width ? line->a.x : line->b.x > width ? line->b.x : width;
    height = line->a.y > height ? line->a.y : line->b.y > height ? line->b.y : height;
  }

  std::vector<std::vector<size_t>> diagram(width + 1, std::vector<size_t>(height + 1, 0));

  // "draw" line, count overlaps, check for every line if it overlaps with any befor
  size_t overlaps = 0;
  for (const auto& line : filtered)
  {
    if (line->isHorizontal())
    {
      size_t min = std::min(line->a.x, line->b.x);
      size_t max = std::max(line->a.x, line->b.x);
      for (size_t x = min; x <= max; ++x)
      {
        auto& value = diagram[x][line->a.y];
        ++value;
        if (value == 2)
        {
          ++overlaps;
        }
      }
    }

    if (line->isVertical())
    {
      size_t min = std::min(line->a.y, line->b.y);
      size_t max = std::max(line->a.y, line->b.y);
      for (size_t y = min; y <= max; ++y)
      {
        auto& value = diagram[line->a.x][y];
        ++value;
        if (value == 2)
        {
          ++overlaps;
        }
      }
    }
  }

  std::cout << prefix << ": overlaps " << overlaps << std::endl;
}

void run_b(const std::string& prefix, const std::vector<std::shared_ptr<Line>>& lines)
{
  // we know that we have only horizontal / vertical or diagonal lines, no need to filter
  // find size of diagram
  size_t width  = 0;
  size_t height = 0;
  for (const auto& line : lines)
  {
    width  = line->a.x > width ? line->a.x : line->b.x > width ? line->b.x : width;
    height = line->a.y > height ? line->a.y : line->b.y > height ? line->b.y : height;
  }

  std::vector<std::vector<size_t>> diagram(width + 1, std::vector<size_t>(height + 1, 0));

  // "draw" line, count overlaps, check for every line if it overlaps with any befor
  size_t overlaps = 0;
  for (const auto& line : lines)
  {
    if (line->isHorizontal())
    {
      size_t min = std::min(line->a.x, line->b.x);
      size_t max = std::max(line->a.x, line->b.x);
      for (size_t x = min; x <= max; ++x)
      {
        auto& value = diagram[x][line->a.y];
        ++value;
        if (value == 2)
        {
          ++overlaps;
        }
      }
    }

    else if (line->isVertical())
    {
      size_t min = std::min(line->a.y, line->b.y);
      size_t max = std::max(line->a.y, line->b.y);
      for (size_t y = min; y <= max; ++y)
      {
        auto& value = diagram[line->a.x][y];
        ++value;
        if (value == 2)
        {
          ++overlaps;
        }
      }
    }
    else
    {
      // diagonal lines
      size_t x;
      size_t x2;
      size_t y;
      size_t y2;

      if (line->a.x < line->b.x)
      {
        x  = line->a.x;
        y  = line->a.y;
        x2 = line->b.x;
        y2 = line->b.y;
      }
      else
      {
        x  = line->b.x;
        y  = line->b.y;
        x2 = line->a.x;
        y2 = line->a.y;
      }

      for (x; x <= x2; ++x)
      {
        auto& value = diagram[x][y];
        ++value;
        if (value == 2)
        {
          ++overlaps;
        }

        if (y < y2)
        {
          // handle \ lines
          ++y;
        }
        else
        {
          // handle / lines
          --y;
        }
      }
    }
  }

  std::cout << prefix << ": overlaps " << overlaps << std::endl;
}

}  // namespace advent::d05

int main(int argc, char* argv[])
{
  std::vector<std::shared_ptr<advent::d05::Line>> exampleLines;
  advent::readFile("example.txt",
                   [&exampleLines](const auto& line)
                   { exampleLines.emplace_back(advent::d05::Line::parse(line)); });

  std::vector<std::shared_ptr<advent::d05::Line>> inputLines;
  advent::readFile("input.txt",
                   [&inputLines](const auto& line)
                   { inputLines.emplace_back(advent::d05::Line::parse(line)); });

  std::cout << "05A________________________" << std::endl;
  advent::d05::run_a("05A: EX", exampleLines);
  advent::d05::run_a("05A: IN", inputLines);

  std::cout << "\n04B________________________" << std::endl;
  advent::d05::run_b("05B: EX", exampleLines);
  advent::d05::run_b("05B: IN", inputLines);

  return 0;
}
