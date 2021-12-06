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

namespace advent::d06
{

void populate(const std::string& prefix, std::vector<int> fishes, int days)
{
  // group fishes into day buckets
  // day = 0 finishes breeding next
  // day = 6 days where a finished fish resets to
  // day = 8 where a new fish is born
  std::vector<size_t> buckets(9, 0);
  for (auto& fish : fishes)
  {
    ++buckets[fish];
  }

  for (size_t day = 0; day < days; ++day)
  {
    size_t next_gen = buckets[0];
    for (int pos = 1; pos < buckets.size(); ++pos)
    {
      buckets[pos - 1] = buckets[pos];
    }

    // born new fishes
    buckets[8] = next_gen;
    // reset breeding cycle
    buckets[6] += next_gen;
  }

  size_t sum = 0;
  for (auto value : buckets)
  {
    sum += value;
  }

  std::cout << prefix << ": fishes " << sum << " days: " << days << std::endl;
}

void run_a(const std::string& prefix, std::vector<int> fishes)
{
  populate(prefix, fishes, 80);
}

void run_b(const std::string& prefix, std::vector<int> fishes)
{
  populate(prefix, fishes, 256);
}

void parseFishes(const std::string& line, std::vector<int>& fishes)
{
  for (const auto& segment : split(line, ",", true, true))
  {
    int value;
    std::stringstream(segment) >> value;
    fishes.emplace_back(value);
  }
}

}  // namespace advent::d06

int main(int argc, char* argv[])
{
  std::vector<int> exampleFishes;
  advent::readFile("example.txt",
                   [&exampleFishes](const auto& line)
                   { advent::d06::parseFishes(line, exampleFishes); });
  std::vector<int> inputFishes;
  advent::readFile("input.txt",
                   [&inputFishes](const auto& line)
                   { advent::d06::parseFishes(line, inputFishes); });

  std::cout << "06A________________________" << std::endl;
  advent::d06::run_a("06A: EX", exampleFishes);
  advent::d06::run_a("06A: IN", inputFishes);

  std::cout << "\n06B________________________" << std::endl;
  advent::d06::run_b("06B: EX", exampleFishes);
  advent::d06::run_b("06B: IN", inputFishes);

  return 0;
}
