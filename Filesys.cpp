#include "Utilities.h"
#include "Constants.h"
#include "SdiskFAT12.cpp"
#include <algorithm>
#include <map>

class Filesys: public SdiskFAT12
{
  public:
    Filesys(std::string diskname, int numberofblocks, int blocksize)
      : SdiskFAT12(diskname, numberofblocks, blocksize) {

      std::fstream system_disk;
      system_disk.open("./sys/" + diskname);

      if(system_disk.is_open()) {
        std::string boot_block_data = "";
        this->getblock(kBootBlock,boot_block_data);

        std::string base_drive_type = boot_block_data.substr(kDriveNameOffset,kDriveNameLength);

        if(base_drive_type == kSupportedDriveType) {
          std::cout << diskname << " drive found." << std::endl;
          std::cout << "Booting...\nBooted " << diskname << std::endl << std::endl;

          this->InitializeMemory();
          this->fssynch();

        } else {
          char response;

          do {
            std::cout << "Not a valid drive. Formate?(y/n): ";
            std::cin >> response;
          } while( !std::cin.fail() && response!='y' && response!='n' );

          if(response == 'n') exit(-1);

          std::cout << "Formating drive...\n\n";

          // Init boot block
          std::cout << "Creating boot block...\n\n";


          // generate blank bootstrap code just setting them all to 0
          std::string bootstrap = "";
          for(int i=0; i<blocksize-32; i++) {
            bootstrap += "00";
          }

          std::string boot_sector_block = kBootStrapJmpCommand + kSupportedDriveType + kBytesPerSector + kSectorsPerCluster + kReservedSectors + kFats
                                          +
                                          kRootDirCapacity + kTotalSectors + kMediaType + kSectorsPerFat + kSectorsPerTrack + kHeads +
                                          kHiddenSectors + bootstrap + kReservedSignitare;

          this->putblock(kBootBlock,boot_sector_block);

          // Init FATs
          std::cout << "Creating fat blocks...\n\n";
          std::string empty_hex = "00";
          std::string empty_block_bytes = "";

          for(int i=0; i<blocksize; i++) {
            empty_block_bytes += empty_hex;
          }
          std::string first_fat_block = empty_block_bytes;
          first_fat_block.replace(0,6,kReservedFatClustersValue);

          for(int i=1; i<19; i++) {
            if(i == kFatPrimaryStart || i == kFatBackupStart) {
              this->putblock(i,first_fat_block);
            } else {
              this->putblock(i,empty_block_bytes);
            }
          }

          std::cout << "Formate finished.\nBooting...\nBooted " << diskname << std::endl << std::endl;
          this->InitializeMemory();
          this->fssynch();
        }
      }

    };
    int fsclose();
    int fssynch() {
      std::cout << "Syncing...\n" << std::endl;

      int root_entries = this->filename.size();
      int root_offset = this->fatsize+1;

      int current_root_sector = root_offset;
      for(int i=0; i < root_entries; i++) {
        if(i!=0 && i&16==0) {
          current_root_sector++;
        }

        std::string tmp_entry_data = AsciiStringToHexString(this->filename.at(i)) + "000000000000000000000000000000"
                                     + IntToHexString(std::stoi(this->firstblock.at(i))) + "00000000";

        this->putblock(current_root_sector,tmp_entry_data);
      }

      std::cout << "Synced\n\n";

    };
    int newfile(std::string file);
    int rmfile(std::string file);
    int getfirstblock(std::string file);
    int addblock(std::string file, std::string block);
    int delblock(std::string file, int blocknumber);
    int readblock(std::string file, int blocknumber, std::string & buffer);
    int writeblock(std::string file, int blocknumber, std::string buffer);
    int nextblock(std::string file, int blocknumber);

  private:
    int rootsize; // maximum number of entries in ROOT
    int fatsize; // number of blocks occupied by FAT
    std::vector < std::string > filename; // filenames in ROOT
    std::vector < std::string > firstblock; // firstblocks in ROOT
    std::vector < std::string > fat; // FAT

    void InitializeMemory() {
      std::cout << "Initializing Memory...\nMemory Loaded" << std::endl << std::endl;

      std::string boot_sector_block = "";
      this->getblock(kBootBlock,boot_sector_block);

      std::string base_root_size_value = boot_sector_block.substr(kRootSizeOffset,kRootSizeLength);
      this->rootsize = HexStringToInt(ReverseHexPair(base_root_size_value));

      std::string base_number_fats = boot_sector_block.substr(kFatsOffset,kFatsLength);
      std::string base_sectors_per_fat = boot_sector_block.substr(kSectorsPerFatOffset,kSectorsPerFatLength);
      this->fatsize = HexStringToInt(base_number_fats) * HexStringToInt(ReverseHexPair(base_sectors_per_fat));

      int root_offset = this->fatsize + 1;
      int num_of_root_sectors = (this->rootsize * 32) / this->getblocksize();

      // load filenames and firstblocks
      for(int i=root_offset; i < num_of_root_sectors+root_offset; i++) {
        std::string tmp_sector_data = "";
        this->getblock(i,tmp_sector_data);

        for(int j=0; j<this->getblocksize(); j+=64) {
          std::string tmp_dir_raw_data = tmp_sector_data.substr(j,64);

          if(tmp_dir_raw_data.substr(0,2) == "00") {
//            std::cout << "No more directories found in root sector: " << i << std::endl << std::endl;
            break;
          }

          std::map<std::string, std::string> tmp_dir_data = GetDirectoryData(tmp_dir_raw_data);

          if(tmp_dir_raw_data.substr(0,2) == "E5") {
            std::cout << "Sector: " << i << " " << j << "-" << j+63 << ": Empty at \"" << tmp_dir_data.at("first_logical_cluster") << "\"\n";
          } else {
            std::cout << "Sector: " << i << " " << j << "-" << j+63 << ": \"" << tmp_dir_data.at("file_name") << "\" \""
                      << tmp_dir_data.at("extension") << "\" at \"" << tmp_dir_data.at("first_logical_cluster") << "\"\n";
            this->filename.push_back(tmp_dir_data.at("file_name") + tmp_dir_data.at("extension"));
            this->firstblock.push_back(tmp_dir_data.at("first_logical_cluster"));
          }
        }
      }

//      PrintVectorStrings(this->filename,"filename");
//      PrintVectorStrings(this->firstblock,"firstblock");

      // load fat
      int current_fat_index = 0;

      for(int i=1; i<this->fatsize/2; i++) {
        std::string tmp_sector_data = "";
        this->getblock(i,tmp_sector_data);

        for(int i=0; i<this->getblocksize(); i+= 6) {
          std::string current_bytes = tmp_sector_data.substr(i,6);
          std::string first_fat = current_bytes.substr(3,1) + current_bytes.substr(0,2);
          std::string second_fat = current_bytes.substr(2,1) + current_bytes.substr(4,2);

          this->fat.push_back(first_fat);
          this->fat.push_back(second_fat);
          current_fat_index += 2;
        }
      }

//      PrintVectorStrings(this->fat,"fat");

    }
};
