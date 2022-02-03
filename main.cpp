#include "Filesys.cpp"

int main()
{
  Filesys fsys("test2",256,128);

  std::string f1block, f2block;
  fsys.ReadBlock("file1",17,f1block);

  std::cout << f1block << "\n\n";

  fsys.ReadBlock("file2", fsys.GetFirstBlock("file2"),f2block);

  std::cout << f2block << "\n\n";

  fsys.WriteBlock("file2",fsys.GetFirstBlock("file2"),f1block);

  fsys.ReadBlock("file2", fsys.GetFirstBlock("file2"),f2block);

  std::cout << f2block << "\n\n";

  std::cout << fsys.NextBlock("file2", fsys.GetFirstBlock("file2")) << "\n\n";
  std::cout << fsys.NextBlock("file2", 53) << "\n\n";
  std::cout << fsys.NextBlock("file2", 34) << "\n\n";
  return 0;
}

