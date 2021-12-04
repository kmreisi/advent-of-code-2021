#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "../utils.h"

static constexpr size_t BINGO_WIDTH = 5;


#if 0
static std::string path = "C:\\data\\projects\\advent\\04\\example.txt";
#else
static std::string path = "C:\\data\\projects\\advent\\04\\input.txt";
#endif

struct Field
{
  /* Two dimensional array coloumn, row */
  int fill_pos = 0;
  std::array<std::array<size_t, BINGO_WIDTH>, BINGO_WIDTH> data;

  bool won = false;
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

static size_t find_bingo(const std::vector<size_t>& numbers, std::vector<Field>& fields, bool last)
{
  // iterate throguh number set and ehck if there are winners
  std::unordered_set<size_t> current;
  size_t found = 0;
  for (const auto& number : numbers)
  {
    current.emplace(number);

    for (auto& field : fields)
    {
      // a filed only needs to win once, skip the ones which already won once to detect the last one
      if (field.won)
      {
        continue;
      }

      if (auto bingo = field.isBingo(current))
      {
        field.won = true;

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

int main(int argc, char* argv[])
{
  auto stream = std::ifstream(path);
  if (!stream)
  {
    std::cout << "Failed to read file, can not open stream.";
    return -2;
  }

  std::vector<size_t> numbers;
  std::vector<Field> fields;

  std::string line;

  std::unique_ptr<Field> toFill = std::make_unique<Field>();
  while (std::getline(stream, line))
  {
    line = trim(line);

    // first line is the numbers
    if (numbers.empty())
    {
      for (const auto& segment : split(line, ",", true, true))
      {
        size_t value;
        std::stringstream(segment) >> value;
        numbers.emplace_back(value);
      }
      continue;
    }

    // read fields
    for (const auto& segment : split(line, " ", true, true))
    {
      size_t value;
      std::stringstream(segment) >> value;

      if (toFill->fill(value))
      {
        fields.emplace_back(std::move(*toFill));
        toFill = std::make_unique<Field>();
      }
    }
  }

  std::cout << "bingo " << find_bingo(numbers, fields, false) << "!!" << std::endl;
  std::cout << "last_bingo " << find_bingo(numbers, fields, true) << "!!" << std::endl;
  return 0;
}
