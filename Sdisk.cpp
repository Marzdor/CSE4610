#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Sdisk
{
  public:
    // Creates Sdisk and initializes it or opens existing Sdisk
    Sdisk(string diskname, int numberofblocks, int blocksize) {
      this -> diskname = diskname;
      ifstream disk_file;
      disk_file.open("./sys/" + diskname);

      if (!disk_file.is_open()) {
        this -> numberofblocks = numberofblocks;
        this -> blocksize = blocksize;

        cout << "Sdisk: " << diskname << " not found.\n";
        cout << "Creating: " << diskname << " ...\n";

        ofstream new_disk_file("./sys/" + diskname);

        const string kEmptyBit = "0";
        string block_bits = "";
        for(int i=0; i<blocksize; i++) {
          block_bits += kEmptyBit;
        }

        for (int i = 0; i < numberofblocks; i++) {
          string block = block_bits + " Block " + to_string(i);
          new_disk_file << block << endl;
        }

        cout << diskname << " is created.\n";
        cout << "Connected to: " << diskname << "\nNumber of blocks: " << this -> numberofblocks << "\nBlock size (bits): " << this -> blocksize << "\n\n";
        new_disk_file.close();
      } else {
        string last_block, block_entry;
        istringstream iss;

        while (disk_file >> ws && getline(disk_file, last_block));

        iss.clear();
        iss.str(last_block);
        int index = 0;

        while (iss.good()) {
          iss >> block_entry;

          if (index == 0) {
            this -> blocksize = block_entry.length();
          } else if (index == 2) {
            this -> numberofblocks = stoi(block_entry) + 1;
          }

          index++;
        }

        cout << "Connected to: " << diskname << "\nNumber of blocks: " << this -> numberofblocks << "\nBlock size (bits): " << this -> blocksize << "\n\n";
        disk_file.close();
      }
    }

    int getblock(int blocknumber, string & buffer) {
      cout << "Get Block: " << blocknumber << endl;
      ifstream disk_file;
      string block, block_entry;
      istringstream iss;

      disk_file.open("./sys/" + this -> diskname);

      if (disk_file.is_open()) {
        cout << "Locating Block: " << blocknumber << endl;
        int current_block = 0;

        while (getline(disk_file, block)) {
          if (current_block == blocknumber) {
            iss.clear();
            iss.str(block);
            int index = 0;

            while (iss.good()) {
              iss >> block_entry;

              if (index == 0) {
                buffer = block_entry;
              }

              index++;
            }
          }

          current_block++;
        }

        disk_file.close();
      } else {
        cout << "[ERROR] getblock disk_file not Opened.\n\n";
        return 0;
      };

      cout << "Found Block: " << blocknumber << "\n\n";
      return 1;
    };

    int putblock(int blocknumber, string buffer) {
      cout << "Update Block: " << blocknumber << endl;
      fstream disk_file;
      string block;
      vector < string > new_disk_blocks;

      disk_file.open("./sys/" + this -> diskname);

      if (disk_file.is_open()) {
        cout << "Locating Block: " << blocknumber << endl;
        int current_block = 0;

        while (getline(disk_file, block)) {
          if (current_block != blocknumber) {
            new_disk_blocks.push_back(block);
          } else {
            string newData = buffer + " Block " + to_string(current_block);
            new_disk_blocks.push_back(newData);
          }

          current_block++;
        }

        disk_file.close();
      } else {
        cout << "[ERROR] putblock disk_file 1 not Opened.\n\n";
        return 0;
      };

      disk_file.open("./sys/" + this -> diskname, ios::out | ios::trunc); // Clear file

      if (disk_file.is_open()) {
        cout << "Updating Block: " << blocknumber << endl;
        for (int i = 0; i < new_disk_blocks.size(); i++) {
          disk_file << new_disk_blocks.at(i) << endl;
        }

        disk_file.close();
      } else {
        cout << "[ERROR] putblock disk_file 2 not Opened.\n\n";
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
