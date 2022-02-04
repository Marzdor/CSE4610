#include "Utilities.h"
#include "Filesys.cpp"
#include <vector>
#include <string>

class Shell: public Filesys
{
  public :
    Shell(std::string disk_name, int number_of_blocks, int block_size)
      : Filesys(disk_name, number_of_blocks, block_size) {

    };

    int dir() {
      // lists all files
      std::vector<std::string> file_list = this->ls();

      for (int i=0; i<file_list.size(); i++) {
        std::cout << file_list.at(i) << std::endl;
      }

      return 1;
    };

    int add(std::string filename, std::string buffer) {
      // add a new file using input from the keyboard
      this->CreateNewFile(filename);
      std::vector<std::string> blocks = StandardizeBlocks(buffer, this->GetBlockSize());

      int blocknumber = 0;

      for (int i=0; i<blocks.size(); i++) {
        blocknumber = this->AddBlock(filename,blocks.at(i));

        if(blocknumber == 0 || blocknumber == -1) {
          return 0;
        }
      }

      return 1;
    };

    int del(std::string filename) {
      // deletes the file
      int first_block = this->GetFirstBlock(filename);

      while(first_block != 0) {
        this->DeleteBlock(filename, first_block);
        first_block = this->GetFirstBlock(filename);
      }

      this->RemoveFile(filename);
      return 1;
    };

    int type(std::string filename) {
      //lists the contents of file
      int current_block = this->GetFirstBlock(filename);
      std::string buffer = "";

      while(current_block != 0) {
        std::string tmp_string = "";
        this->ReadBlock(filename, current_block, tmp_string);
        buffer += tmp_string;
        current_block = this->NextBlock(filename, current_block);
      }

      std::cout << buffer.substr(0, buffer.find_first_of(kBlankData)) << std::endl;
      return 1;
    };

    int copy(std::string first_file, std::string second_file) {
      //copies file1 to file2
      int current_block = this->GetFirstBlock(first_file);
      std::string first_file_contents = "";

      while(current_block != 0) {
        std::string tmp_string = "";
        this->ReadBlock(first_file, current_block, tmp_string);
        first_file_contents += tmp_string;
        current_block = this->NextBlock(first_file, current_block);
      }

      this->del(second_file);
      this->add(second_file,first_file_contents);
      return 1;
    };
};
