#ifndef TABLE_H
#define TABLE_H

#include "Filesys.h"
#include <string>

class Table : public Filesys
{
  public :
    Table(std::string disk_name, int block_size, int number_of_blocks, std::string flat_file, std::string index_file);
    int Build_Table(std::string input_file);
    int Search(std::string value);
  private :
    std::string flatfile;
    std::string indexfile;
    int IndexSearch(std::string value);
};

#endif
