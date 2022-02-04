
#include "Shell.cpp"
#include <iostream>
#include <string>

int main()
{
//
//This main program inputs commands to the shell.
//It inputs commands as : command op1 op2
//You should modify it to work for your implementation.
//
  Shell shell("build1",256,128);

  std::string s;
  std::string command = "go";
  std::string op1, op2;


  std::cout << "ooo        ooooo                              .oooooo.    .oooooo..o" << std::endl;
  std::cout << "`88.       .888'                             d8P'  `Y8b  d8P'    `Y8" << std::endl;
  std::cout << " 888b     d'888   .oooo.   oooo    ooo      888      888 Y88bo.     " << std::endl;
  std::cout << " 8 Y88. .P  888  `P  )88b   `88b..8P\'       888      888  `\"Y8888o. " << std::endl;
  std::cout << " 8  `888'   888   .oP\"888     Y888'         888      888      `\"Y88b" << std::endl;
  std::cout << " 8    Y     888  d8(  888   .o8\"'88b        `88b    d88' oo     .d8P" << std::endl;
  std::cout << "o8o        o888o `Y888\"\"8o o88'   888o       `Y8bood8P'  8\"\"88888P' " << std::endl;


  while (command != "quit") {
    command.clear();
    op1.clear();
    op2.clear();
    std::cout << "$";

    getline(std::cin,s);

    int firstblank = s.find(' ');

    if (firstblank < s.length()) s[firstblank] = '#';

    int secondblank = s.find(' ');
    command = s.substr(0, firstblank);

    if (firstblank < s.length())
      op1=s.substr(firstblank+1,secondblank-firstblank-1);

    if (secondblank < s.length())
      op2=s.substr(secondblank+1);

    if (command=="dir") {
      // use the ls function
      shell.dir();
    }
    if (command=="add") {
      // The variable op1 is the new file and op2 is the file data
      shell.add(op1, op2);
    }
    if (command=="del") {
      // The variable op1 is the file
      shell.del(op1);
    }
    if (command=="type") {
      // The variable op1 is the file
      shell.type(op1);
    }
    if (command=="copy") {
      // The variable op1 is the source file and the variable op2 is the destination file.
      shell.copy(op1, op2);
    }
    if (command=="search") {
      // This is the command for Project 4
      // The variable op1 is the date
    }
  }

  return 0;
}
