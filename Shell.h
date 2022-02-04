#ifndef SHELL_H
#define SHELL_H

#include "Filesys.h"
#include <string>

class Shell : public Filesys
{
  public:
    Shell(std::string disk_name, int number_of_blocks, int block_size);
    int dir();
    int add(std::string filename, std::string buffer);
    int del(std::string filename);
    int type(std::string filename);
    int copy(std::string first_file, std::string second_file);

  private:
    std::string disk_name; // file name of software-disk
    int number_of_blocks; // number of blocks on disk
    int block_size; // block size in bytes
};

#endif
