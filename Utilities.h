#ifndef UTILITIES
#define UTILITIES

#include <string>
#include <map>
#include <vector>

int HexStringToInt(std::string);
std::string HexStringToAsciiString(std::string);
std::string ReverseHexPair(std::string);
std::map<std::string, std::string> GetDirectoryData(std::string);
void PrintVectorStrings(std::vector<std::string> vec, std::string name_of_vec);
#endif
