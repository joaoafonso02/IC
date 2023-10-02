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
  unsigned int bitCount = 11;

  // Create a BitStream instance 
  BitStream file(fileName, BitStream::w);

  // write A7 and C0 in binary
  unsigned char data[] = {0xA7, 0xC3};

  file.writeBits(data, bitCount, 0);

  return 0;
}

