#include "Config.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

class PasswordFile
{
  public:
    // Saves Filename
    // Opens the file and reads the userNames and passwords into the vectors user and password.
    PasswordFile(string filename) {
      this -> filename = filename;
      ifstream inFile(kDriveDirectory + filename);

      if (inFile.is_open()) {
        cout << "Getting Users\n";

        string userEntry, userItem;
        istringstream iss;

        while (getline(inFile, userEntry)) {
          iss.clear();
          iss.str(userEntry);
          int index = 0;

          while (iss.good()) {
            iss >> userItem;

            if (index == 0) {
              user.push_back(userItem);
            } else {
              password.push_back(userItem);
            }

            index++;
          }
        }

        cout << "Users loaded \n\n";
        inFile.close();
      } else {
        cout << "[ERROR] PasswordFile File not Opened.\n\n";
        exit(-1);
      }
    };

    // Adds new userName and password if they do not exist then synchs the password file
    void addpw(string newuser, string newpassword) {
      auto found = find(this -> user.begin(), this -> user.end(), newuser);

      if (found != this -> user.end()) {
        cout << "[ERROR] " << newuser << " is Taken.\n";
      } else {
        cout << "Adding user: " << newuser << "\n\n";
        this -> user.push_back(newuser);
        this -> password.push_back(newpassword);
        this -> synch();
        cout << newuser << " added \n\n";
      }
    };

    // Checks if userName and password match
    bool checkpw(string user, string passwd) {
      cout << "Logging " << user << " in\n";
      auto found = find(this -> user.begin(), this -> user.end(), user);

      if (found != this -> user.end()) {
        int userIndex = found - this -> user.begin();

        bool passwordMatches = this -> password.at(userIndex) == passwd;

        if (passwordMatches) {
          cout << "Welcom " << user << "\n\n";
          return true;
        } else {
          cout << "[ERROR] Invalid username or password! \n\n";
          return false;
        }
      } else {
        cout << "[ERROR] Invalid username or password! \n\n";
        return false;
      }
    };

  private:
    string filename; // the file that contains password information
    vector < string > user; // the list of usernames
    vector < string > password; // the list of passwords

    // writes the user/password vectors to the password file
    void synch() {
      ofstream outFile;
      outFile.open("./sys/" + this -> filename, fstream::trunc);

      if (outFile.is_open()) {
        cout << "synch File is Open.\n";

        for (int i = 0; i < this -> user.size(); i++) {
          string userEntry = this -> user.at(i) + " " + this -> password.at(i);
          cout << userEntry << endl;
          outFile << userEntry << endl;
        }

        outFile.close();
      } else {
        cout << "[ERROR] synch File not Opened.\n\n";
        exit(-1);
      }
    };
};
