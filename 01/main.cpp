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
#include <vector>

namespace advent::d01
{
static constexpr bool DEBUG = false;

static void run_a(const std::string& prefix, const std::vector<size_t>& input)
{
  // iterate through list of measurements and count the times the current is higher then the
  // previous
  size_t last  = 0;
  size_t times = 0;
  for (size_t pos = 1; pos < input.size(); ++pos)
  {
    if (input[pos - 1] < input[pos])
    {
      ++times;
    }
  }

  std::cout << prefix << ": Increased '" << times << "'." << std::endl;
}

static void run_b(const std::string& prefix, const std::vector<size_t>& input)
{
  size_t last  = 0;
  size_t times = 0;
  for (size_t pos = 3; pos < input.size(); ++pos)
  {
    // size_t sumLast    = input[pos - 1] + input[pos - 2] + input[pos - 3];
    // size_t sumCurrent = input[pos -1] + input[pos -2] + input[pos];
    size_t sumLast    = input[pos - 3];
    size_t sumCurrent = input[pos];

    if (DEBUG)
    {
      std::cout << " " << sumCurrent << " "
                << (sumCurrent > sumLast  ? "more" :
                    sumCurrent == sumLast ? "equal" :
                                            "less")
                << std::endl;
    }

    if (sumCurrent > sumLast)
    {
      ++times;
    }
  }

  std::cout << prefix << ": Increased '" << times << "'." << std::endl;
}

}  // namespace advent::d01

int main(int argc, char* argv[])
{
  std::vector<size_t> exampleValues;
  advent::readFile("example.txt",
                   [&exampleValues](const auto& line)
                   {
                     size_t value;
                     std::stringstream(line) >> value;
                     exampleValues.emplace_back(value);
                   });

  std::vector<size_t> inputValues;
  advent::readFile("input.txt",
                   [&inputValues](const auto& line)
                   {
                     size_t value;
                     std::stringstream(line) >> value;
                     inputValues.emplace_back(value);
                   });

  std::cout << "01A________________________" << std::endl;
  advent::d01::run_a("01A: EX", exampleValues);
  advent::d01::run_a("01A: IN", inputValues);

  std::cout << "\n01B________________________" << std::endl;
  advent::d01::run_b("01B: EX", exampleValues);
  advent::d01::run_b("01B: IN", inputValues);

  return 0;
}
