#include "Table.h"
#include "Filesys.h"
#include <string>

Table::Table(std::string disk_name, int block_size, int number_of_blocks, std::string flat_file, std::string index_file)
  : Filesys(disk_name, number_of_blocks, block_size)
{

};

int Table::Build_Table(std::string input_file)
{

};

int Table::Search(std::string value)
{

};

int Table::IndexSearch(std::string value)
{

};

