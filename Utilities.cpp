#include "Utilities.h"
#include <sstream>
#include <iostream>

int HexStringToInt (std::string hex_value)
{
  int x;
  std::stringstream ss;
  ss << std::hex << hex_value;
  ss >> x;

  return x;
}

std::string HexStringToAsciiString(std::string hex_value)
{
  // initialize the ASCII code string as empty.
  std::string ascii = "";

  for (int i = 0; i < hex_value.length(); i += 2) {
    // extract two characters from hex string
    std::string part = hex_value.substr(i, 2);

    // change it into base 16 and
    // typecast as the character
    char ch = std::stoul(part, nullptr, 16);

    // add this char to final ASCII string
    ascii += ch;
  }
  return ascii;
}

std::string ReverseHexPair(std::string hex_pair)
{
  return hex_pair.substr(2,2) + hex_pair.substr(0,2);
}

std::map<std::string, std::string> GetDirectoryData(std::string dir_string)
{
  std::string file_name = HexStringToAsciiString(dir_string.substr(0,16));
  std::string extension = HexStringToAsciiString(dir_string.substr(16,6));
  std::string attributes = HexStringToAsciiString(dir_string.substr(22,2));
  std::string reserved = HexStringToAsciiString(dir_string.substr(24,4));
  std::string created_time = HexStringToAsciiString(dir_string.substr(28,4));
  std::string created_date = HexStringToAsciiString(dir_string.substr(32,4));
  std::string last_access_date = HexStringToAsciiString(dir_string.substr(36,4));
  std::string ignore = HexStringToAsciiString(dir_string.substr(40,4));
  std::string last_write_time = HexStringToAsciiString(dir_string.substr(44,4));
  std::string last_write_date = HexStringToAsciiString(dir_string.substr(48,4));
  std::string first_logical_cluster = std::to_string(HexStringToInt(dir_string.substr(52,4)));
  std::string file_size = HexStringToAsciiString(dir_string.substr(56,8));

  std::map<std::string, std::string> dir_data {
    {"file_name", file_name},
    {"extension", extension},
    {"attributes", attributes},
    {"reserved", ""},
    {"created_time", ""},
    {"created_date", ""},
    {"last_access_date", ""},
    {"ignore", ""},
    {"last_write_time", ""},
    {"last_write_date", ""},
    {"first_logical_cluster", first_logical_cluster},
    {"file_size", ""}
  };

  return dir_data;
};

void PrintVectorStrings(std::vector<std::string> vec, std::string name_of_vec)
{
  std::cout << name_of_vec << " elements are:\n";

  for(int i=0; i < vec.size(); i++) {
    std::cout << vec.at(i) << std::endl;
  }
  
  std::cout << name_of_vec << " end reached\n\n";
}

