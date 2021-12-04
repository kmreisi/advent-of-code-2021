#include "utils.h"

namespace advent
{

std::string trim(std::string text)
{
  // characters to cut
  static const char* ws = " \t\n\r\f\v";

  // cut start
  text.erase(0, text.find_first_not_of(ws));

  // cut end
  text.erase(text.find_last_not_of(ws) + 1);

  return text;
}

std::vector<std::string> split(const std::string& original,
                               const std::string& delim,
                               bool trimSegment,
                               bool skipEmpty)
{
  std::vector<std::string> result;
  size_t last  = 0;
  size_t found = 0;
  std::string segment;

  // while last did not reach end
  while (last < original.length())
  {
    // search for next instance of delimiter
    if ((found = original.find(delim, last)) == std::string::npos)
    {
      // delimiter not found, set found to end of original to get "rest"
      found = original.length();
    }

    // get segment between the delimiters
    segment = original.substr(last, found - last);
    if (trimSegment)
    {
      segment = trim(segment);
    }
    if (!(skipEmpty && segment.empty()))
    {
      result.push_back(segment);
    }

    // update last to position behind last found delimiter
    last = found + delim.length();
  }
  return result;
}

}  // namespace advent
