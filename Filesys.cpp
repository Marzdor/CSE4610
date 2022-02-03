#include "Utilities.h"
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

      std::vector<std::string> root_block = StandardizeBlocks(ss.str(), this->GetBlockSize());
      this->PutBlock(0,root_block.at(0));

      ss.str("");

      // Sync Fat
      for (int i = 0; i < this->fat.size(); i++) {
        ss << std::setw(kWidth) << std::setfill(kPlaceHolder) << this->fat.at(i) << kDelim;
      }

      std::vector<std::string> fat_block = StandardizeBlocks(ss.str(), this->GetBlockSize());
      for (int i = 0; i < fat_block.size(); i++) {
        this->PutBlock(i+1, fat_block.at(i));
      }
    };

    int CreateNewFile(std::string filename) {
      std::string formatted_filename = FormatteFileName(filename);
      int file_location = FindFileIndex(this->file_names,formatted_filename);
      int empty_file_index = FindFileIndex(this->file_names,kBlankDirectory);

      bool file_exists = file_location != -1;
      bool no_free_space = empty_file_index == -1;
      if(file_exists || no_free_space) {
        return 0;
      }

      this->file_names.at(empty_file_index) = formatted_filename;
      this->first_blocks.at(empty_file_index) = 0;
      this->FileSystemSync();

      return 1;
    };

    int RemoveFile(std::string filename) {
      std::string formatted_filename = FormatteFileName(filename);
      int file_location = FindFileIndex(this->file_names,formatted_filename);
      bool file_exists = file_location != -1;

      if(!file_exists) {
        return 0;
      }

      bool file_is_empty = this->GetFirstBlock(formatted_filename) == 0;

      if(!file_is_empty) {
        return 0;
      }

      this->file_names.at(file_location) = kBlankDirectory;
      this->FileSystemSync();
      return 1;
    };

    int GetFirstBlock(std::string filename) {
      std::string formatted_filename = FormatteFileName(filename);
      int found = FindFileIndex(this->file_names, filename);

      if(found != -1) {
        return this->first_blocks.at(found);
      }

      return 0;
    };

    int AddBlock(std::string filename, std::string buffer) {
      int free_block = this->fat.at(0);

      if(free_block == kEndOfFile) {
        return -1;
      }

      std::string formatted_filename = FormatteFileName(filename);
      int file_location = FindFileIndex(this->file_names,formatted_filename);
      bool file_exists = file_location != -1;

      if(!file_exists) {
        return 0;
      }

      int file_first_block = this->GetFirstBlock(formatted_filename);

      if(file_first_block == kEndOfFile) {
        this->first_blocks.at(file_location) = free_block;
        this->fat.at(0) = this->fat.at(free_block);
        this->fat.at(free_block) = kEndOfFile;
        this->PutBlock(free_block, buffer);

      } else {
        int current_file_block = file_first_block;

        while(this->fat.at(current_file_block) != kEndOfFile) {
          current_file_block = this->fat.at(current_file_block);
        }

        this->fat.at(current_file_block) = free_block;
        this->fat.at(0) = this->fat.at(free_block);
        this->fat.at(free_block) = kEndOfFile;
        this->PutBlock(free_block, buffer);
      }

      this->FileSystemSync();
      return free_block;
    };

    int DeleteBlock(std::string filename, int block_number) {
      std::string formatted_filename = FormatteFileName(filename);
      int file_location = FindFileIndex(this->file_names,formatted_filename);
      bool file_exists = file_location != -1;

      if(!file_exists) {
        return 0;
      }

      int file_first_block = this->GetFirstBlock(formatted_filename);

      if(file_first_block == block_number) {
        this->first_blocks.at(file_location) = this->fat.at(block_number);
        this->fat.at(block_number) = this->fat.at(0);
        this->fat.at(0) = block_number;
      } else {
        int current_file_block = file_first_block;

        while(this->fat.at(current_file_block) != block_number) {
          current_file_block = this->fat.at(current_file_block);
        }

        this->fat.at(current_file_block) = this->fat.at(block_number);
        this->fat.at(block_number) = this->fat.at(0);
        this->fat.at(0) = block_number;
      }

      this->FileSystemSync();
      return 1;
    };

    int ReadBlock(std::string filename, int block_number, std::string & buffer) {
      std::string formatted_filename = FormatteFileName(filename);
      int found_block = this->CheckBlock(formatted_filename,block_number);

      if(found_block == -1) {
        return 0;
      }

      this->GetBlock(found_block, buffer);
      return 1;
    };

    int WriteBlock(std::string filename, int block_number, std::string buffer) {
      std::string formatted_filename = FormatteFileName(filename);
      int found_block = this->CheckBlock(formatted_filename,block_number);

      if(found_block == -1) {
        return 0;
      }

      this->PutBlock(found_block, buffer);
    };

    int NextBlock(std::string filename, int block_number) {
      std::string formatted_filename = FormatteFileName(filename);
      int found_block = this->CheckBlock(formatted_filename,block_number);

      if(found_block == -1) {
        return -1;
      }

      return this->fat.at(found_block);
    };

  private :
    int root_size;                        // maximum number of entries in ROOT
    int fat_size;                         // number of blocks occupied by FAT
    std::vector<std::string> file_names;  // filenames in ROOT
    std::vector<int> first_blocks;        // firstblocks in ROOT
    std::vector<int> fat;                 // FAT

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
          ss << kBlankDirectory << kDelim << kBlankIndex << kDelim;
        }

      } else {
        std::string root_block;
        this->GetBlock(0,root_block);
        ss << root_block;
      }

      std::vector<std::string> root_entries = StandardizeBlocks(ss.str(), this->GetBlockSize());

      ss.str(root_entries.at(0));

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

      std::vector<std::string> fat_entries = StandardizeBlocks(ss.str(), this->GetBlockSize());
      ss.str("");

      for (int i = 0; i < fat_entries.size(); i++) {
        ss << fat_entries.at(i);
      }

      std::string current_entry;
      for (int i=0; i < this->GetNumberOfBlocks() && getline(ss, current_entry,' '); i++) {
        this->fat.push_back(std::stoi(current_entry));
      }
    }

    int CheckBlock(std::string filename, int block_number) {
      std::string formatted_filename = FormatteFileName(filename);
      int current_file_block = this->GetFirstBlock(formatted_filename);

      bool searching = true;

      while(searching) {
        if(current_file_block == block_number) {
          searching= false;
          return current_file_block;
        }

        if(current_file_block == kEndOfFile) {
          searching= false;
          return -1;
        }

        current_file_block = this->fat.at(current_file_block);
      }


    }
};
