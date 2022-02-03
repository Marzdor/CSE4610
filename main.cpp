#include "Filesys.cpp"

int main()
{
  Filesys fsys("test2",256,128);

  fsys.CreateNewFile("file1");
  fsys.CreateNewFile("file2");

  std::string bfile1, bfile2;

  for (int i=1; i<=1024; i++) {
    bfile1+="1";
  }

  std::vector<std::string> blocks = StandardizeBlocks(bfile1,128);

  int blocknumber = 0;

  for (int i=0; i< blocks.size(); i++) {
    blocknumber = fsys.AddBlock("file1",blocks.at(i));
  }

  fsys.DeleteBlock("file1",fsys.GetFirstBlock("file1"));

  return 0;
}

