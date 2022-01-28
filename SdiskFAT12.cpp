#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class SdiskFAT12
{
  public:
    // Creates Sdisk and initializes it or opens existing Sdisk
    SdiskFAT12(string diskname, int numberofblocks, int blocksize) {
      this -> diskname = diskname;
      ifstream diskFile;
      diskFile.open("./sys/" + diskname);

      if (!diskFile.is_open()) {
        this -> numberofblocks = numberofblocks;
        this -> blocksize = blocksize;

        cout << "Sdisk: " << diskname << " not found.\n";
        cout << "Creating: " << diskname << " ...\n";

        ofstream newDiskFile("./sys/" + diskname);

        string emptyHex = "00";
        string blockBytes = "";
        
        for(int i=0; i<blocksize; i++) {
          blockBytes += emptyHex;
        }

        for (int i = 0; i < numberofblocks; i++) {
          string block = blockBytes + " Block " + to_string(i);
          newDiskFile << block << endl;
        }

        cout << diskname << " is created.\n";
        cout << "Connected to: " << diskname << "\nNumber of blocks: " << this -> numberofblocks << "\nBlock size: " << this -> blocksize << "\n\n";
        newDiskFile.close();
      } else {
        string lastLine, data;
        istringstream iss;

        while (diskFile >> ws && getline(diskFile, lastLine));

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

        cout << "Connected to: " << diskname << "\nNumber of blocks: " << this -> numberofblocks << "\nBlock size (bytes): " << this -> blocksize << "\n\n";
        diskFile.close();
      }
    }

    int getblock(int blocknumber, string & buffer) {
      cout << "Get Block: " << blocknumber << endl;
      ifstream diskFile;
      string block, data;
      istringstream iss;

      diskFile.open("./sys/" + this -> diskname);

      if (diskFile.is_open()) {
        cout << "Locating Block: " << blocknumber << endl;
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
        cout << "[ERROR] putblock File1 not Opened.\n\n";
        return 0;
      };

      cout << "Found Block: " << blocknumber << "\n\n";
      return 1;
    };

    int putblock(int blocknumber, string buffer) {
      cout << "Update Block: " << blocknumber << endl;
      fstream diskFile;
      string block;
      vector < string > tmp;

      diskFile.open("./sys/" + this -> diskname);

      if (diskFile.is_open()) {
        cout << "Locating Block: " << blocknumber << endl;
        int curBlock = 0;

        while (getline(diskFile, block)) {
          if (curBlock != blocknumber) {
            tmp.push_back(block);
          } else {
            string newData = buffer + " Block " + to_string(curBlock);
            tmp.push_back(newData);
          }

          curBlock++;
        }

        diskFile.close();
      } else {
        cout << "[ERROR] putblock File1 not Opened.\n\n";
        return 0;
      };

      diskFile.open("./sys/" + this -> diskname, ios::out | ios::trunc);

      if (diskFile.is_open()) {
        cout << "Updating Block: " << blocknumber << endl;
        for (int i = 0; i < tmp.size(); i++) {
          diskFile << tmp.at(i) << endl;
        }

        diskFile.close();
      } else {
        cout << "[ERROR] putblock File2 not Opened.\n\n";
        return 0;
      };

      cout << "Block " << blocknumber << " Updated.\n\n";
      return 1;
    }

    int getnumberofblocks(); // accessor function
    int getblocksize(); // accessor function
  private:
    string diskname; // file name of software-disk
    int numberofblocks; // number of blocks on disk
    int blocksize; // block size in bytes
};
