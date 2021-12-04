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
#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace advent::d03
{
template <std::size_t SIZE>
void run_a(const std::string& prefix, const std::vector<std::bitset<SIZE>>& input)
{
  size_t half = input.size() / 2;
  std::bitset<SIZE> gama;
  std::bitset<SIZE> epsilon;

  size_t cnt[SIZE];
  std::fill(cnt, cnt + SIZE, 0);

  for (const auto& value : input)
  {
    for (int pos = 0; pos < SIZE; ++pos)
    {
      if (value[pos])
      {
        ++cnt[pos];
      }
    }
  }

  for (int pos = 0; pos < SIZE; ++pos)
  {
    gama[pos] = cnt[pos] > half;
  }
  epsilon = ~gama;

  std::cout << prefix << ": gama " << gama << std::endl;
  std::cout << prefix << ": epsilon " << epsilon << std::endl;
  std::cout << prefix << ": power " << gama.to_ullong() * epsilon.to_ullong() << std::endl;
}

template <std::size_t SIZE>
static std::bitset<SIZE> calc(std::vector<std::bitset<SIZE>> values, bool high)
{
  for (int pos = SIZE - 1; pos >= 0; --pos)
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

template <std::size_t SIZE>
void run_b(const std::string& prefix, const std::vector<std::bitset<SIZE>>& input)
{
  auto oxygen = calc(input, true);
  auto co2    = calc(input, false);

  std::cout << prefix << ": oxygen " << oxygen << std::endl;
  std::cout << prefix << ": co2 " << co2 << std::endl;
  std::cout << prefix << ": life support rating " << oxygen.to_ullong() * co2.to_ullong()
            << std::endl;
}

}  // namespace advent::d03

int main(int argc, char* argv[])
{
  std::vector<std::bitset<5>> exampleValues;
  advent::readFile("example.txt",
                   [&exampleValues](const auto& line)
                   { exampleValues.emplace_back(std::bitset<5>(line)); });

  std::vector<std::bitset<12>> inputValues;
  advent::readFile("input.txt",
                   [&inputValues](const auto& line)
                   { inputValues.emplace_back(std::bitset<12>(line)); });

  std::cout << "03A________________________" << std::endl;
  advent::d03::run_a("03A: EX", exampleValues);
  advent::d03::run_a("03A: IN", inputValues);

  std::cout << "\n03B________________________" << std::endl;
  advent::d03::run_b("03B: EX", exampleValues);
  advent::d03::run_b("03B: IN", inputValues);

  return 0;
}
