#include "Shell.h"
#include "Utilities.h"
#include "Config.h"
#include <vector>
#include <string>
#include <iostream>


Shell::Shell(std::string disk_name, int number_of_blocks, int block_size)
  : Filesys(disk_name, number_of_blocks, block_size)
{

};

int Shell::dir()
{
  // lists all files
  std::vector<std::string> file_list = this->ls();

  for (int i=0; i<file_list.size(); i++) {
    std::cout << file_list.at(i) << std::endl;
  }

  return 1;
};

int Shell::add(std::string filename, std::string buffer)
{
  // add a new file using input from the keyboard
  this->CreateNewFile(filename);
  std::vector<std::string> blocks = StandardizeBlocks(buffer, this->GetBlockSize());

  int blocknumber = 0;

  for (int i=0; i<blocks.size(); i++) {
    blocknumber = this->AddBlock(filename,blocks.at(i));

    if(blocknumber == 0 || blocknumber == -1) {
      std::cout << "Error: Adding File \n";
      return 0;
    }
  }

  return 1;
};

int Shell::del(std::string filename)
{
  // deletes the file
  int first_block = this->GetFirstBlock(filename);

  while(first_block != 0) {
    int delete_response = this->DeleteBlock(filename, first_block);

    if(delete_response == 0) {
      std::cout << "Error: Deleting Block \n";
      return 0;
    }

    first_block = this->GetFirstBlock(filename);
  }

  int remove_response = this->RemoveFile(filename);

  if(remove_response == 0) {
    std::cout << "Error: Deleting File \n";
    return 0;
  }

  return 1;
};

int Shell::type(std::string filename)
{
  //lists the contents of file
  int current_block = this->GetFirstBlock(filename);
  std::string buffer = "";

  if(current_block == 0) {
    std::cout << "Error: File Does Not Exist\n";
    return 0;
  }

  while(current_block != 0) {
    std::string tmp_string = "";
    this->ReadBlock(filename, current_block, tmp_string);
    buffer += tmp_string;
    current_block = this->NextBlock(filename, current_block);
  }

  std::cout << buffer.substr(0, buffer.find_first_of(kBlankData)) << std::endl;
  return 1;
};

int Shell::copy(std::string first_file, std::string second_file)
{
  //copies file1 to file2
  int current_block = this->GetFirstBlock(first_file);
  std::string first_file_contents = "";

  if(current_block == 0) {
    std::cout << "Error: File Does Not Exist\n";
    return 0;
  }

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

