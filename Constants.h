#ifndef CONSTANTS
#define CONSTANTS

#include <string>

const int kBootBlock = 0;
const int kFatPrimaryStart = 1;
const int kFatBackupStart = 10;
const int kRootSizeOffset = 34;
const int kRootSizeLength = 4;
const int kFatsOffset = 32;
const int kFatsLength = 2;
const int kSectorsPerFatOffset = 44;
const int kSectorsPerFatLength = 4;
const int kDriveNameOffset = 6;
const int kDriveNameLength = 16;

//  boot setting defaults
const std::string kSupportedDriveType = "4D41584F53302E31";	// "MAXOS0.1"
const std::string kReservedFatClustersValue = "F0FFFF";		//fat[0] = FF0 fat[1] = FFF
const std::string kBootStrapJmpCommand = "EB3E90";
const std::string kBytesPerSector = "0002"; 				// 512
const std::string kSectorsPerCluster = "01";				// 1
const std::string kReservedSectors = "0100";				// 1
const std::string kFats = "02";								// 2
const std::string kRootDirCapacity = "E000";				// 14sectors * 16dir = 224 total
const std::string kTotalSectors = "FD0A";					// 2813
const std::string kMediaType = "F8"; 						// "Hard Disk"
const std::string kSectorsPerFat = "0900";					// 9
const std::string kSectorsPerTrack = "0100";				// 1
const std::string kHeads = "0100";							// 1
const std::string kHiddenSectors = "0000";					// 0
const std::string kReservedSignitare = "AA55"; 				// Reserved Signature


#endif

