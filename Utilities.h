#ifndef UTILITIES
#define UTILITIES

#include <string>
#include <vector>

std::string FormatteFileName(std::string filename);
int FindStringIndex(std::vector<std::string> elements, std::string to_find);
std::vector<std::string> StandardizeBlocks(std::string buffer, int block_size);

#endif
