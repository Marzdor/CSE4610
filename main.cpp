#include "PasswordFile.cpp"
//#include "Sdisk.cpp"
#include "Filesys.cpp"


int main()
{

  const int kBlocks = 2813; // about 1.44megabytes of storage space
  const int kBlockSize = 512; // bytes
  Filesys mainDisk("drive1",kBlocks,kBlockSize);

//  const int kBlocks = 16;
//  const int kBlockSize = 64; // bits

//  Sdisk disk1("test1",kBlocks,kBlockSize);
//
//  string block_1, block_2, block_3, block_4;
//
//  for (int i=1; i<=32; i++) block_1=block_1+"1";
//  for (int i=1; i<=32; i++) block_2=block_2+"2";
//
//  disk1.putblock(4,block_1);
//  disk1.getblock(4,block_3);
//
//  cout << "Should be 32 1s : " << block_3 << " | length: " << block_3.length() << "\n\n";
//
//  disk1.putblock(8,block_2);
//  disk1.getblock(8,block_4);
//
//  cout << "Should be 32 2s : " << block_4 << " | length: " << block_4.length() << "\n\n";


//	PasswordFile passfile("password.txt");
//
//	passfile.addpw("dbotting","123qwe");
//	passfile.addpw("egomez","qwerty");
//	passfile.addpw("tongyu","liberty");
//
//	// write some lines to see if passwords match users
//	passfile.checkpw("dbotting","123");
//	passfile.checkpw("dbotting","123qwe");
}

