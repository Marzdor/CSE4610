#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

class Sdisk
{
  public:
    Sdisk(std::string disk_name, int number_of_blocks, int block_size) {
      this->disk_name = disk_name;
      this->number_of_blocks = number_of_blocks;
      this->block_size = block_size;

      std::ifstream disk_file;
      disk_file.open("./sys/" + this->disk_name);

      if (!disk_file.is_open()) {
        std::ofstream new_disk_file("./sys/" + disk_name);

        for(int i=0; i<this->GetNumberOfBlocks()*this->GetBlockSize(); i++) {
          new_disk_file.put('.');
        }

        new_disk_file.close();
      }

      disk_file.close();
    }

    int GetBlock(int block_number, std::string & buffer) {
      std::ifstream disk_file;
      disk_file.open("./sys/" + this->disk_name);

      if (!disk_file.is_open()) {
        return 0;
      };

      buffer = "";
      disk_file.seekg(block_number*this->GetBlockSize());

      for(int i=0; i<this->GetBlockSize(); i++) {
        buffer += disk_file.get();
      }

      disk_file.close();
      return 1;
    };

    int PutBlock(int block_number, std::string buffer) {
      std::fstream disk_file;
      disk_file.open("./sys/" + this->disk_name);

      if (!disk_file.is_open()) {
        return 0;
      };

      disk_file.seekg(block_number*this->GetBlockSize());

      for(int i=0; i<buffer.length() && i<this->GetBlockSize(); i++) {
        disk_file.put(buffer.at(i));
      }

      disk_file.close();
      return 1;
    }

    int GetNumberOfBlocks() {
      return this->number_of_blocks;
    };

    int GetBlockSize() {
      return this->block_size;
    };

  private:
    std::string disk_name; // file name of software-disk
    int number_of_blocks; // number of blocks on disk
    int block_size; // block size in bytes
};
