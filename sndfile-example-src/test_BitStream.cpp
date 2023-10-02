#include <iostream>
#include <fstream>
#include <string>
#include "BitStream.hh" 

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1; // Return an error code
  }

  std::string fileName = argv[1];
  unsigned int bitCount =11;

  // Check if the specified file exists, and create it if it doesn't
  std::ifstream fileExists(fileName);
  if (!fileExists) {
    // The file doesn't exist; create it
    std::ofstream createFile(fileName, std::ios::binary | std::ios::trunc);
    if (!createFile) {
      std::cerr << "Failed to create the file '" << fileName << "'" << std::endl;
      return 1; // Return an error code
    }
    createFile.close();
  }

  // Create a BitStream instance 
  BitStream file(fileName);

  // write A7 and C0 in binary
  unsigned char data[] = {0xA7, 0xC0};

  file.writeBits(reinterpret_cast<const char*>(data), bitCount, 0);

  // return first 11 bits from this file
  char* bits = file.readBits(bitCount, 0);
  std::cout << "Binary representation: ";
  for (unsigned int i = 0; i < bitCount; i++) {
    std::cout << ((bits[i / 8] >> (7 - (i % 8))) & 1);
  }
  std::cout << std::endl;


  

  return 0;
}

