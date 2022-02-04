#ifndef SDISK_H
#define SDISK_H

#include <string>

class Sdisk
{
  public:
    Sdisk(std::string disk_name, int number_of_blocks, int block_size);
    int GetBlock(int block_number, std::string & buffer);
    int PutBlock(int block_number, std::string buffer);
    int GetNumberOfBlocks();
    int GetBlockSize();

  private:
    std::string disk_name; // file name of software-disk
    int number_of_blocks; // number of blocks on disk
    int block_size; // block size in bytes
};

#endif
