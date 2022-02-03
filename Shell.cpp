#include <vector>

class Shell: public Filesys
{
  public :
    Shell(string filename, int numberofblocks, int blocksize)
      :Filesys(std::string disk_name, int number_of_blocks, int block_size) {

    };

    int dir() {
      // lists all files
      std::vector<std::string> file_list = this->ls();

      for (i=0; i<file_list.size(); i++) {
        std::cout << file_list.at(i) << std::endl;
      }

      return 1;
    };

    int add(std::string filename) {
      // add a new file using input from the keyboard
      return 1;
    };

    int del(std::string filename) {
      // deletes the file
      return 1;
    };

    int type(std::string filename) {
      //lists the contents of file
      return 1;
    };

    int copy(std::string filename1, std::string filename2) {
      //copies file1 to file2
      return 1;
    };
};
