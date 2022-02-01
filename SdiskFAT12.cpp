#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

class SdiskFAT12
{
  public:
    // Creates Sdisk and initializes it or opens existing Sdisk
    SdiskFAT12(std::string diskname, int numberofblocks, int blocksize) {
      this -> diskname = diskname;
      std::ifstream diskFile;
      diskFile.open("./sys/" + diskname);

      if (!diskFile.is_open()) {
        this -> numberofblocks = numberofblocks;
        this -> blocksize = blocksize;

        std::cout << "Sdisk: " << diskname << " not found.\n";
        std::cout << "Creating: " << diskname << " ...\n";

        std::ofstream newDiskFile("./sys/" + diskname);

        std::string emptyHex = "00";
        std::string blockBytes = "";

        for(int i=0; i<blocksize; i++) {
          blockBytes += emptyHex;
        }

        for (int i = 0; i < numberofblocks; i++) {
          std::string block = blockBytes + " Block " + std::to_string(i);
          newDiskFile << block << std::endl;
        }

        std::cout << diskname << " is created.\n";
        std::cout << "Connected to: " << diskname << "\nNumber of blocks: " << this -> numberofblocks << "\nBlock size: " << this -> blocksize << "\n\n";
        newDiskFile.close();
      } else {
        std::string lastLine, data;
        std::istringstream iss;

        while (diskFile >> std::ws && getline(diskFile, lastLine));

        iss.clear();
        iss.str(lastLine);
        int index = 0;

        while (iss.good()) {
          iss >> data;

          if (index == 0) {
            this -> blocksize = data.length() / 2;
          } else if (index == 2) {
            this -> numberofblocks = stoi(data) + 1;
          }

          index++;
        }

        std::cout << "Connected to: " << diskname << "\nNumber of blocks: " << this -> numberofblocks << "\nBlock size (bytes): " << this -> blocksize << "\n\n";
        diskFile.close();
      }
    }

    int getblock(int blocknumber, std::string & buffer) {
//      std::cout << "Get Block: " << blocknumber << std::endl;
      std::ifstream diskFile;
      std::string block, data;
      std::istringstream iss;

      diskFile.open("./sys/" + this -> diskname);

      if (diskFile.is_open()) {
//        std::cout << "Locating Block: " << blocknumber << std::endl;
        int curBlock = 0;

        while (getline(diskFile, block)) {
          if (curBlock == blocknumber) {
            iss.clear();
            iss.str(block);
            int index = 0;

            while (iss.good()) {
              iss >> data;

              if (index == 0) {
                buffer = data;
              }

              index++;
            }
          }

          curBlock++;
        }

        diskFile.close();
      } else {
        std::cout << "[ERROR] putblock File1 not Opened.\n\n";
        return 0;
      };

//      std::cout << "Found Block: " << blocknumber << "\n\n";
      return 1;
    };

    int putblock(int blocknumber, std::string buffer) {
      std::cout << "Update Block: " << blocknumber << std::endl;
      std::fstream diskFile;
      std::string block;
      std::vector < std::string > tmp;

      diskFile.open("./sys/" + this -> diskname);

      if (diskFile.is_open()) {
        std::cout << "Locating Block: " << blocknumber << std::endl;
        int curBlock = 0;

        while (getline(diskFile, block)) {
          if (curBlock != blocknumber) {
            tmp.push_back(block);
          } else {
            std::string newData = buffer + " Block " + std::to_string(curBlock);
            tmp.push_back(newData);
          }

          curBlock++;
        }

        diskFile.close();
      } else {
        std::cout << "[ERROR] putblock File1 not Opened.\n\n";
        return 0;
      };

      diskFile.open("./sys/" + this -> diskname, std::ios::out | std::ios::trunc);

      if (diskFile.is_open()) {
        std::cout << "Updating Block: " << blocknumber << std::endl;
        for (int i = 0; i < tmp.size(); i++) {
          diskFile << tmp.at(i) << std::endl;
        }

        diskFile.close();
      } else {
        std::cout << "[ERROR] putblock File2 not Opened.\n\n";
        return 0;
      };

      std::cout << "Block " << blocknumber << " Updated.\n\n";
      return 1;
    }

    int getnumberofblocks() {
      return this->numberofblocks;
    }; 
    
    int getblocksize() {
      return this->blocksize;
    }; 
  private:
    std::string diskname; // file name of software-disk
    int numberofblocks; // number of blocks on disk
    int blocksize; // block size in bytes
};
