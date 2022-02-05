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

  this->CreateNewFile(flat_file_name);
  this->CreateNewFile(index_file_name);
};

int Table::Build_Table(std::string filename)
{
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
};

int Table::Search(std::string value)
{

};

int Table::IndexSearch(std::string value)
{

};

