#include "Config.h"
#include "Utilities.h"
#include <iostream>
#include <algorithm>

std::string FormatteFileName(std::string filename)
{
  return filename.substr(0,kWidth);
};

int FindFileIndex(std::vector<std::string> files, std::string filename)
{
  auto iterator = std::find(files.begin(), files.end(), filename);

  if (iterator != files.end()) {
    int index = iterator - files.begin();
    return index;
  } else {
    return -1;
  }
}

std::vector<std::string> StandardizeBlocks(std::string buffer, int block_size)
{
  std::vector<std::string> blocks;
  int number_of_blocks = 0;

  if (buffer.length() % block_size == 0) {
    number_of_blocks = buffer.length()  block_size;
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
}
