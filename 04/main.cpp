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
#include <array>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace advent::d04
{

static constexpr size_t BINGO_WIDTH = 5;

struct Field
{
  int fill_pos = 0;
  /* Two dimensional array coloumn, row */
  std::array<std::array<size_t, BINGO_WIDTH>, BINGO_WIDTH> data;

  size_t sumOfUnmarked(const std::unordered_set<size_t>& current) const
  {
    size_t sum = 0;
    for (const auto& col : data)
    {
      for (const auto& cell : col)
      {
        if (current.find(cell) == current.end())
        {
          sum += cell;
        }
      }
    }
    return sum;
  }

  std::optional<size_t> isBingo(const std::unordered_set<size_t>& current) const
  {
    // check rows
    for (int col = 0; col < data.size(); ++col)
    {
      bool found = true;
      for (int row = 0; row < data.size(); ++row)
      {
        if (current.find(data[col][row]) == current.end())
        {
          found = false;
        }
      }

      if (found)
      {
        return sumOfUnmarked(current);
      }
    }

    // check coloums
    for (int row = 0; row < data.size(); ++row)
    {
      bool found = true;
      for (int col = 0; col < data.size(); ++col)
      {
        if (current.find(data[col][row]) == current.end())
        {
          found = false;
        }
      }

      if (found)
      {
        return sumOfUnmarked(current);
      }
    }

    return {};
  }

  bool fill(size_t value)
  {
    int x      = fill_pos % BINGO_WIDTH;
    int y      = fill_pos / BINGO_WIDTH;
    data[x][y] = value;
    ++fill_pos;
    return fill_pos == BINGO_WIDTH * BINGO_WIDTH;
  }
};

static size_t find_bingo(const std::vector<size_t>& numbers,
                         const std::vector<std::shared_ptr<Field>>& fields,
                         bool last)
{
  // iterate throguh number set and ehck if there are winners
  std::unordered_set<size_t> current;
  std::unordered_set<std::shared_ptr<Field>> won;

  size_t found = 0;
  for (const auto& number : numbers)
  {
    current.emplace(number);

    for (auto& field : fields)
    {
      // a filed only needs to win once, skip the ones which already won once to detect the last one
      if (won.find(field) != won.end())
      {
        continue;
      }

      if (const auto& bingo = field->isBingo(current))
      {
        won.insert(field);

        found = *bingo * number;
        if (!last)
        {
          // return the first;
          return found;
        }
      }
    }
  }

  // return last / 0;
  return found;
}

void run_a(const std::string& prefix,
           const std::vector<size_t>& numbers,
           const std::vector<std::shared_ptr<Field>>& fields)
{
  std::cout << prefix << ": bingo " << find_bingo(numbers, fields, false) << "!!" << std::endl;
}

void run_b(const std::string& prefix,
           const std::vector<size_t>& numbers,
           const std::vector<std::shared_ptr<Field>>& fields)
{
  std::cout << prefix << ": last_bingo " << find_bingo(numbers, fields, true) << "!!" << std::endl;
}

struct ParseContext
{
  std::vector<size_t> numbers;
  std::vector<std::shared_ptr<Field>> fields;
  std::unique_ptr<Field> toFill;

  void parse(const std::string& line)
  {
    if (!toFill)
    {
      toFill = std::make_unique<Field>();
    }

    // first line is the numbers
    if (numbers.empty())
    {
      for (const auto& segment : split(line, ",", true, true))
      {
        size_t value;
        std::stringstream(segment) >> value;
        numbers.emplace_back(value);
      }
      return;
    }

    // read fields
    for (const auto& segment : split(line, " ", true, true))
    {
      size_t value;
      std::stringstream(segment) >> value;

      if (toFill->fill(value))
      {
        fields.emplace_back(std::move(toFill));
        toFill = std::make_unique<Field>();
      }
    }
  }
};

}  // namespace advent::d04

int main(int argc, char* argv[])
{
  std::vector<size_t> exampleNumbers;
  std::vector<std::shared_ptr<advent::d04::Field>> exampleFields;
  {
    advent::d04::ParseContext ctx;
    advent::readFile("example.txt", [&ctx](const auto& line) { ctx.parse(line); });
    exampleNumbers = ctx.numbers;
    exampleFields  = ctx.fields;
  }

  std::vector<size_t> inputNumbers;
  std::vector<std::shared_ptr<advent::d04::Field>> inputFields;
  {
    advent::d04::ParseContext ctx;
    advent::readFile("input.txt", [&ctx](const auto& line) { ctx.parse(line); });
    inputNumbers = ctx.numbers;
    inputFields  = ctx.fields;
  }

  std::cout << "04A________________________" << std::endl;
  advent::d04::run_a("04A: EX", exampleNumbers, exampleFields);
  advent::d04::run_a("04A: IN", inputNumbers, inputFields);

  std::cout << "\n04B________________________" << std::endl;
  advent::d04::run_b("04B: EX", exampleNumbers, exampleFields);
  advent::d04::run_b("04B: IN", inputNumbers, inputFields);

  return 0;
}
