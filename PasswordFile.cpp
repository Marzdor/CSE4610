#include "PasswordFile.h"
#include "Config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// Saves Filename
// Opens the file and reads the userNames and passwords into the vectors user and password.
PasswordFile::PasswordFile(std::string filename)
{
  this->filename = filename;
  std::ifstream password_file(kDriveDirectory + filename);

  if (password_file.is_open()) {
    std::cout << "Getting Users\n";

    std::string user_entry, user_item;
    std::istringstream iss;

    while (getline(password_file, user_entry)) {
      iss.clear();
      iss.str(user_entry);
      int index = 0;

      while (iss.good()) {
        iss >> user_item;

        if (index == 0) {
          user.push_back(user_item);
        } else {
          password.push_back(user_item);
        }

        index++;
      }
    }

    std::cout << "Users loaded \n\n";
    password_file.close();
  } else {
    std::cout << "[ERROR] PasswordFile File not Opened.\n\n";
    exit(-1);
  }
};

// Adds new userName and password if they do not exist then synchs the password file
void PasswordFile::AddPassWord(std::string new_user, std::string new_password)
{
  auto found = find(this->user.begin(), this->user.end(), new_user);

  if (found != this->user.end()) {
    std::cout << "[ERROR] " << new_user << " is Taken.\n";
  } else {
    std::cout << "Adding user: " << new_user << "\n\n";
    this->user.push_back(new_user);
    this->password.push_back(new_password);
    this->sync();
    std::cout << new_user << " added \n\n";
  }
};

// Checks if userName and password match
bool PasswordFile::CheckPassword(std::string user, std::string password)
{
  std::cout << "Logging " << user << " in\n";
  auto found = find(this->user.begin(), this->user.end(), user);

  if (found != this->user.end()) {
    int user_index = found - this->user.begin();

    bool password_matches = this->password.at(user_index) == password;

    if (password_matches) {
      std::cout << "Welcome " << user << "\n\n";
      return true;
    } else {
      std::cout << "[ERROR] Invalid username or password! \n\n";
      return false;
    }
  } else {
    std::cout << "[ERROR] Invalid username or password! \n\n";
    return false;
  }
};


// writes the user/password vectors to the password file
void PasswordFile::sync()
{
  std::ofstream out_file;
  out_file.open("./sys/" + this->filename, std::fstream::trunc);

  if (out_file.is_open()) {
    std::cout << "synch File is Open.\n";

    for (int i = 0; i < this->user.size(); i++) {
      std::string user_entry = this->user.at(i) + " " + this->password.at(i);
      std::cout << user_entry << "\n";
      out_file << user_entry << "\n";
    }

    out_file.close();
  } else {
    std::cout << "[ERROR] synch File not Opened.\n\n";
    exit(-1);
  }
};

