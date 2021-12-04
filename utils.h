#include <string>
#include <vector>

std::string trim(std::string text);

std::vector<std::string> split(const std::string& original,
                               const std::string& delim,
                               bool trimSegment,
                               bool skipEmpty);

