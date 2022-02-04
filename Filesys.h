#ifndef FILESYS_H
#define FILESYS_H

#include "Sdisk.h"
#include <vector>
#include <string>

class Filesys : public Sdisk
{
  public:
    Filesys(std::string disk_name, int number_of_blocks, int block_size);
    int fsclose();
    void FileSystemSync();
    int CreateNewFile(std::string filename);
    int RemoveFile(std::string filename);
    int GetFirstBlock(std::string filename);
    int AddBlock(std::string filename, std::string buffer);
    int DeleteBlock(std::string filename, int block_number);
    int ReadBlock(std::string filename, int block_number, std::string & buffer);
    int WriteBlock(std::string filename, int block_number, std::string buffer);
    int NextBlock(std::string filename, int block_number);
    std::vector<std::string> ls();

  private:
    int root_size;                        // maximum number of entries in ROOT
    int fat_size;                         // number of blocks occupied by FAT
    std::vector<std::string> file_names;  // filenames in ROOT
    std::vector<int> first_blocks;        // firstblocks in ROOT
    std::vector<int> fat;                 // FAT

    void InitializeFileSystem();
    void LoadRoot(bool fresh_drive);
    void LoadFat(bool fresh_drive);
    int CheckBlock(std::string filename, int block_number);
};

#endif
