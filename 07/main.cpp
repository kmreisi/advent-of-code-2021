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

namespace advent::d07
{

struct FuelCon
{
  int pos;
  size_t fuel;
};

template <typename LAMBDA, typename LAMBDA2>
FuelCon findLowestFuel(const std::vector<int>& crabs, int center, int max, LAMBDA fuelFunc, LAMBDA2 next)
{

  size_t lowestFuel = std::numeric_limits<size_t>::max();
  int lowestPos     = 0;

  for (int cnt = 0; cnt < max; ++cnt)
  {
    int pos     = center + next(cnt);
    size_t fuel = fuelFunc(pos, crabs);

    // check for an increase (break)
    if (fuel > lowestFuel)
    {
      break;
    }

    // update lowest fule / pos
    if (fuel < lowestFuel)
    {
      lowestFuel = fuel;
      lowestPos  = pos;
    }
  }

  return FuelCon{lowestPos, lowestFuel};
}


template <typename LAMBDA>
void findBestFuelConsumption(const std::string& prefix,
                             const std::vector<int>& crabs,
                             LAMBDA fuelFunc)
{
  size_t  all = 0;
  int max = 0;
  for (auto pos : crabs)
  {
    all += pos;
    max = std::max(pos, max);
  }

  int center = static_cast<int>(all / crabs.size());
  // search left and right for lowest
  std::cout << prefix << ": center " << center << std::endl;
  FuelCon left  = findLowestFuel(crabs, center, max, fuelFunc, [](int cnt) { return -cnt; });
  FuelCon right = findLowestFuel(crabs, center, max, fuelFunc, [](int cnt) { return +cnt; });

  int pos     = left.fuel < right.fuel ? left.pos : right.pos;
  size_t fuel = left.fuel < right.fuel ? left.fuel : right.fuel;

  std::cout << prefix << ": pos: " << pos << " fuel: " << fuel << std::endl;
}

void run_a(const std::string& prefix, std::vector<int> crabs)
{
  findBestFuelConsumption(prefix,
                          crabs,
                          [](int center, const std::vector<int>& crabs)
                          {
                            size_t fuel = 0;
                            std::for_each(crabs.begin(),
                                          crabs.end(),
                                          [&center, &fuel](auto pos)
                                          { fuel += std::abs(center - pos); });
                            return fuel;
                          });
}

void run_b(const std::string& prefix, std::vector<int> crabs)
{
  findBestFuelConsumption(prefix,
                          crabs,
                          [](int center, const std::vector<int>& crabs)
                          {
                            size_t fuel = 0;
                            std::for_each(crabs.begin(),
                                          crabs.end(),
                                          [&center, &fuel](auto pos)
                                          {
                                            for (int fak = 1; fak < std::abs(center - pos) + 1;
                                                 ++fak)
                                              fuel += fak;
                                          });
                            return fuel;
                          });
}

void parseCrabs(const std::string& line, std::vector<int>& crabs)
{
  for (const auto& segment : split(line, ",", true, true))
  {
    int value;
    std::stringstream(segment) >> value;
    crabs.emplace_back(value);
  }
}

}  // namespace advent::d07

int main(int argc, char* argv[])
{
  std::vector<int> exampleCrabs;
  advent::readFile("example.txt",
                   [&exampleCrabs](const auto& line)
                   { advent::d07::parseCrabs(line, exampleCrabs); });
  std::vector<int> inputCrabs;
  advent::readFile("input.txt",
                   [&inputCrabs](const auto& line) { advent::d07::parseCrabs(line, inputCrabs); });

  std::cout << "07A________________________" << std::endl;
  advent::d07::run_a("07A: EX", exampleCrabs);
  advent::d07::run_a("07A: IN", inputCrabs);

  std::cout << "\n06B________________________" << std::endl;
  advent::d07::run_b("07B: EX", exampleCrabs);
  advent::d07::run_b("07B: IN", inputCrabs);

  return 0;
}
