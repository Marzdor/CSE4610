#ifndef TABLE_H
#define TABLE_H

#include "Filesys.h"
#include <string>

class Table : public Filesys
{
  public :
    Table(std::string disk_name, int number_of_blocks, int block_size, std::string flat_file_name, std::string index_file_name);
    int Build_Table(std::string filename);
    int Search(std::string value);
  private :
    std::string flatfile;
    std::string indexfile;
    int IndexSearch(std::string value);
};

#endif
