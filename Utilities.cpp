#include "Config.h"
#include "Utilities.h"
#include <iostream>
#include <algorithm>
#include <vector>

std::string FormatteFileName(std::string filename)
{
  return filename.substr(0,kWidth);
};

int FindStringIndex(std::vector<std::string> elements, std::string to_find)
{
  auto iterator = std::find(elements.begin(), elements.end(), to_find);

  if (iterator != elements.end()) {
    int index = iterator - elements.begin();
    return index;
  } else {
    return -1;
  }
};

std::vector<std::string> StandardizeBlocks(std::string buffer, int block_size)
{
  std::vector<std::string> blocks;
  int number_of_blocks = 0;

  if (buffer.length() % block_size == 0) {
    number_of_blocks = buffer.length() /  block_size;
  } else {
    number_of_blocks = buffer.length() / block_size + 1;
  }

  std::string tmp_block;
  for (int i=0; i < number_of_blocks; i++) {
    tmp_block = buffer.substr(block_size*i, block_size);
    blocks.push_back(tmp_block);
  }

  int last_block = blocks.size() - 1;

  for (int i=blocks.at(last_block).length(); i<block_size; i++) {
    blocks.at(last_block) += kBlankData;
  }

  return blocks;
};

