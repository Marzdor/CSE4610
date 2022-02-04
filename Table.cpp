#include "Filesys.cpp"

class Table : public Filesys
{
  public :
    Table(std::string disk_name, int block_size, int number_of_blocks, std::string flat_file, std::string index_file)
      : Filesys(disk_name, number_of_blocks, block_size) {

    };


    int Build_Table(string input_file);
    int Search(string value);
  private :
    string flatfile;
    string indexfile;
    int IndexSearch(string value);
};

