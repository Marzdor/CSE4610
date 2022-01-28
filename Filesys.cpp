#include "SdiskFAT12.cpp"

const int kBootBlock = 0;
const int kFatPrimaryStart = 1;
const int kFatBackupStart = 10;
const string kSupportedDriveType = "4D41584F53302E31";	// "MAXOS0.1"
const string kReservedFatClustersValue = "F8FF";

class Filesys: public SdiskFAT12
{
  public:
    Filesys(string diskname, int numberofblocks, int blocksize)
      : SdiskFAT12(diskname, numberofblocks, blocksize) {

      fstream system_disk;
      system_disk.open("./sys/" + diskname);

      if(system_disk.is_open()) {
        string boot_block_data = "";
        this->getblock(kBootBlock,boot_block_data);

        string drive_type = boot_block_data.substr(6,16);

        if(drive_type == kSupportedDriveType) {
          cout << "MAXOS0.1 drive found." << endl;
          // do stuff

        } else {
          char response;

          do {
            cout << "Not a valid drive. Formate?(y/n): ";
            cin >> response;
          } while( !cin.fail() && response!='y' && response!='n' );

          if(response == 'n') exit(-1);

          cout << "Formating drive...\n\n";

          // Init boot block
          cout << "Creating boot block...\n\n";
          const string kBootStrapJmpCommand = "EB3E90";
          const string kBytesPerSector = "0002"; 		// 512
          const string kSectorsPerCluster = "01";		// 1
          const string kReservedSectors = "0100";		// 1
          const string kFats = "02";					// 2
          const string kRootDirCapacity = "E000";		// 14sectors * 16dir = 224 total
          const string kTotalSectors = "FD0A";			// 2813
          const string kMediaType = "F8"; 				// "Hard Disk"
          const string kSectorsPerFat = "0900";			// 9
          const string kSectorsPerTrack = "0100";		// 1
          const string kHeads = "0100";					// 1
          const string kHiddenSectors = "0000";			// 0
          const string kReservedSignitare = "AA55"; 			// Reserved Signature

          // generate blank bootstrap code just setting them all to 0
          string bootstrap = "";
          for(int i=0; i<blocksize-32; i++) {
            bootstrap += "00";
          }

          string boot_sector_block = kBootStrapJmpCommand + kSupportedDriveType + kBytesPerSector + kSectorsPerCluster + kReservedSectors + kFats +
                                     kRootDirCapacity + kTotalSectors + kMediaType + kSectorsPerFat + kSectorsPerTrack + kHeads +
                                     kHiddenSectors + bootstrap + kReservedSignitare;

          this->putblock(kBootBlock,boot_sector_block);

          // Init FATs
          cout << "Creating fat blocks...\n\n";
          string empty_hex = "00";
          string empty_block_bytes = "";

          for(int i=0; i<blocksize; i++) {
            empty_block_bytes += empty_hex;
          }
          string first_fat_block = empty_block_bytes;
          first_fat_block.replace(0,4,kReservedFatClustersValue);

          for(int i=1; i<19; i++) {
            if(i == kFatPrimaryStart || i == kFatBackupStart) {
              this->putblock(i,first_fat_block);
            } else {
              this->putblock(i,empty_block_bytes);
            }
          }
        }
      }

    };
    int fsclose();
    int fssynch();
    int newfile(string file);
    int rmfile(string file);
    int getfirstblock(string file);
    int addblock(string file, string block);
    int delblock(string file, int blocknumber);
    int readblock(string file, int blocknumber, string & buffer);
    int writeblock(string file, int blocknumber, string buffer);
    int nextblock(string file, int blocknumber);

  private:
    int rootsize; // maximum number of entries in ROOT
    int fatsize; // number of blocks occupied by FAT
    vector < string > filename; // filenames in ROOT
    vector < int > firstblock; // firstblocks in ROOT
    vector < int > fat; // FAT
};
