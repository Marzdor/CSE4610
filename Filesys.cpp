#include "Sdisk.cpp"
#include <vector>
#include <sstream>
#include <iomanip>

class Filesys: public Sdisk
{
  public :
    Filesys(std::string disk_name, int number_of_blocks, int block_size)
      : Sdisk(disk_name, number_of_blocks, block_size) {

      this->root_size = this->GetBlockSize() / 12;

      int bytes_fat_needs = this->GetNumberOfBlocks() * 6;
      this->fat_size = bytes_fat_needs / this->GetBlockSize();
      this->fat_size += bytes_fat_needs % this->GetBlockSize() > 0; // add 1 if we have a remainder else add 0

      this->InitializeFileSystem();
    };

    int fsclose();

    void FileSystemSync() {
      std::stringstream ss;

      // Sync Root
      for(int i=0; i<this->root_size; i++) {
        ss << this->file_names.at(i) << kDelim << std::setw(kWidth) << std::setfill(kPlaceHolder) << this->first_blocks.at(i) << kDelim;
      }

      std::vector<std::string> root_block = this->StandardizeBlock(ss.str(), this->GetBlockSize());
      this->PutBlock(0,root_block.at(0));

      ss.str("");

      // Sync Fat
      for (int i = 0; i < this->fat.size(); i++) {
        ss << std::setw(kWidth) << std::setfill(kPlaceHolder) << this->fat.at(i) << kDelim;
      }

      std::vector<std::string> fat_block = this->StandardizeBlock(ss.str(), this->GetBlockSize());
      for (int i = 0; i < fat_block.size(); i++) {
        this->PutBlock(i+1, fat_block.at(i));
      }
    };

    int newfile(std::string file);
    int rmfile(std::string file);
    int getfirstblock(std::string file);
    int addblock(std::string file, std::string block);
    int delblock(std::string file, int blocknumber);
    int readblock(std::string file, int blocknumber, std::string& buffer);
    int writeblock(std::string file, int blocknumber, std::string buffer);
    int nextblock(std::string file, int blocknumber);
  private :
    int root_size;           // maximum number of entries in ROOT
    int fat_size;            // number of blocks occupied by FAT
    std::vector<std::string> file_names;   // filenames in ROOT
    std::vector<int> first_blocks; // firstblocks in ROOT
    std::vector<int> fat;             // FAT

    void InitializeFileSystem() {
      std::string root_block;
      this->GetBlock(0,root_block);

      bool fresh_drive = root_block.at(0)=='.';

      this->LoadRoot(fresh_drive);
      this->LoadFat(fresh_drive);
      this->FileSystemSync();
    }

    void LoadRoot(bool fresh_drive) {
      std::stringstream ss;

      if(fresh_drive) {
        for(int i=0; i<this->root_size; i++) {
          ss << kBlankDirectory << kBlankIndex;
        }

      } else {
        std::string root_block;
        this->GetBlock(0,root_block);
        ss << root_block;
      }

      std::vector<std::string> root_entries = this->StandardizeBlock(ss.str(), this->GetBlockSize());
      ss.str(root_entries[0]);

      std::string current_entry;
      for (int i=0; i<(this->root_size * 2) && getline(ss, current_entry,' '); i++) {
        if (i % 2 == 0) {
          this->file_names.push_back(current_entry);
        } else {
          this->first_blocks.push_back(std::stoi(current_entry));
        }
      }
    }

    void LoadFat(bool fresh_drive) {
      std::stringstream ss;
      int fat_offset = 1 + this->fat_size;

      if(fresh_drive) {
        ss << std::setw(kWidth) << std::setfill(kPlaceHolder) << fat_offset << kDelim;
        for (int i = 1; i < fat_offset; i++) {
          ss << kBlankFatData << kDelim;
        }

        for (int i=fat_offset; i<(this->GetNumberOfBlocks()-1); i++) {
          ss << std::setw(kWidth) << std::setfill(kPlaceHolder) << (i+1) << kDelim;
        }

        ss << kBlankFatData << kDelim;

      } else {
        std::string existing_fat_data;
        for (int i=1; i<fat_offset; i++) {
          this->GetBlock(i, existing_fat_data);
          ss << existing_fat_data;
        }
      }

      std::vector<std::string> fat_entries = this->StandardizeBlock(ss.str(), this->GetBlockSize());
      ss.str("");

      for (int i = 0; i < fat_entries.size(); i++) {
        ss << fat_entries[i];
      }

      std::string current_entry;
      for (int i=0; i < this->GetNumberOfBlocks() && getline(ss, current_entry,' '); i++) {
        this->fat.push_back(std::stoi(current_entry));
      }
    }

    std::vector<std::string> StandardizeBlock(std::string buffer, int block_size) {
      std::vector<std::string> blocks;
      int number_of_blocks = 0;

      if (buffer.length() % block_size == 0) {
        number_of_blocks = buffer.length()/block_size;
      } else {
        number_of_blocks = buffer.length()/block_size +1;
      }

      std::string tmp_block;
      for (int i=0; i < number_of_blocks; i++) {
        tmp_block = buffer.substr(block_size*i, block_size);
        blocks.push_back(tmp_block);
      }

      int last_block = blocks.size() - 1;

      for (int i=blocks[last_block].length(); i<block_size; i++) {
        blocks[last_block] += kBlankData;
      }

      return blocks;
    }
};
