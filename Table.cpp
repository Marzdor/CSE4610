#include "Table.h"
#include "Filesys.h"
#include "Config.h"
#include "Utilities.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <vector>

Table::Table(std::string disk_name, int number_of_blocks, int block_size, std::string flat_file_name, std::string index_file_name)
  : Filesys(disk_name, number_of_blocks, block_size)
{
  this->flatfile = flat_file_name;
  this->indexfile = index_file_name;

  int flat_file_response = this->CreateNewFile(flat_file_name);
  int index_file_response = this->CreateNewFile(index_file_name);
};

int Table::Build_Table(std::string filename)
{


  int flatfile_first_block = this->GetFirstBlock(this->flatfile);


  if(flatfile_first_block != 0) {
    return 0;
  }

  std::ifstream input_file;
  input_file.open(kDriveDirectory + filename);

  if (input_file.is_open()) {
    std::string index_records;
    std::string buffer;
    std::stringstream ss;

    while (getline(input_file, buffer)) {
      buffer.erase(remove(buffer.begin(), buffer.end(), '*'), buffer.end());
      ss.str("");

      std::string new_block = StandardizeBlocks(buffer, this->GetBlockSize()).at(0);
      int current_block = this->AddBlock(this->flatfile, new_block);

      std::string entry_date = buffer.substr(0,5);
      ss << std::setw(kWidth) << std::setfill(kPlaceHolder) << std::to_string(current_block);
      index_records += entry_date + ss.str();
    }

    std::vector<std::string> index_records_blocks = StandardizeBlocks(index_records, this->GetBlockSize());

    int blocknumber = 0;

    for (int i=0; i<index_records_blocks.size(); i++) {
      blocknumber = this->AddBlock(this->indexfile, index_records_blocks.at(i));
    }
  }

  input_file.close();
  return 1;
};

int Table::Search(std::string value)
{
  int found_index = this->IndexSearch(value);

  if(found_index == -1) {
    std::cout << "No Entry Found\n";
    return 0;
  }

  std::string entry = "";
  this->ReadBlock(this->flatfile, found_index, entry);
  std::string formatted_entry = entry.substr(0, entry.find_first_of(kBlankData));
  std::string date = formatted_entry.substr(0,5);
  std::string end = formatted_entry.substr(5,5);
  std::string type = formatted_entry.substr(10,8);
  std::string place = formatted_entry.substr(18,15);
  std::string reference = formatted_entry.substr(33,7);
  std::string description = formatted_entry.substr(40);

  std::cout << "======================================================================================================\n";
  std::cout << "Date        : " << date << "\n";
  std::cout << "End         : " << end << "\n";
  std::cout << "Type        : " << type << "\n";
  std::cout << "Place       : " << place << "\n";
  std::cout << "Reference   : " << reference << "\n";
  std::cout << "Description : " << description << "\n";
  std::cout << "======================================================================================================\n";
  return 1;
};

int Table::IndexSearch(std::string value)
{
  int current_block = this->GetFirstBlock(this->indexfile);
  std::string buffer = "";

  if(current_block == 0) {
    std::cout << "Error: File Does Not Exist\n";
    return 0;
  }

  while(current_block != 0) {
    std::string tmp_string = "";
    this->ReadBlock(indexfile, current_block, tmp_string);
    buffer += tmp_string;
    current_block = this->NextBlock(indexfile, current_block);
  }

  int entry_start_position = buffer.find(value);

  if(entry_start_position == std::string::npos) {
    return -1;
  }

  std::string entry = buffer.substr(entry_start_position, 10);
  int entry_position = std::stoi(entry.substr(5,5));
  return entry_position;
};

