#ifndef UTILITIES
#define UTILITIES

#include <string>
#include <vector>

std::string FormatteFileName(std::string filename);
int FindFileIndex(std::vector<std::string> files, std::string filename);
std::vector<std::string> StandardizeBlocks(std::string buffer, int block_size);

#endif
