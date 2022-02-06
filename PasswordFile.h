#ifndef PASSWORDFILE_H
#define PASSWORDFILE_H

#include <string>
#include <vector>

class PasswordFile
{
  public:
    PasswordFile(std::string filename);
    void AddPassWord(std::string new_user, std::string new_password);
    bool CheckPassword(std::string user, std::string password);

  private:
    std::string filename; // the file that contains password information
    std::vector < std::string > user; // the list of usernames
    std::vector < std::string > password; // the list of passwords
    void sync();
};

#endif
