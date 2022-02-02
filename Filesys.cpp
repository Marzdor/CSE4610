#include "Sdisk.cpp"
#include <algorithm>

class Filesys: public Sdisk
{
  public :
    Filesys(std::string disk_name, int number_of_blocks, int block_size)
      : Sdisk(disk_name, number_of_blocks, block_size) {
      std::fstream system_disk;
      system_disk.open("./sys/" + disk_name);

      if(system_disk.is_open()) {
       
      }
    };
    
    int fsclose();

    int fssynch() {
    };

    int newfile(std::string file);
    int rmfile(std::string file);
    int getfirstblock(std::string file);
    int addblock(std::string file, std::string block);
    int delblock(std::string file, int blocknumber);
    int readblock(std::string file, int blocknumber, std::string& buffer);
    int writeblock(std::string file, int blocknumber, std::string buffer);
    int nextblock(std::string file, int blocknumber);
  private :
    int rootsize;           // maximum number of entries in ROOT
    int fatsize;            // number of blocks occupied by FAT
    std::vector<std::string> filename;   // filenames in ROOT
    std::vector<int> firstblock; // firstblocks in ROOT
    std::vector<int> fat;             // FAT
};
