#include <iostream>
#include <fstream>
#include <string>
#include "BitStream.hh"

int main(int argc, char **argv) {
  if (argc != 3) {
      std::cerr << "Usage: " << argv[0] << " <filename> <mode (w/r)>" << std::endl;
      return 1; // Return an error code
  }

  std::string fileName = argv[1], mode = argv[2];
  unsigned int bitCount = 11;

  BitStream::Flag flag;
  
  if (mode == "w") {
    flag = BitStream::w;
    BitStream file(fileName, flag);
    // write A7 and C0 in binary
    unsigned char data[] = {0xA7, 0xC3};
    file.writeBits(data, bitCount, 0);
  } else if (mode == "r") {
    flag = BitStream::r;
    BitStream file(fileName, flag);
    // read bits
    std::cout << "Read Bits: ";
    file.readBits(bitCount);

    std::cout << std::endl;
  } else {
    std::cerr << "Invalid mode. Use 'w' for write or 'r' for read." << std::endl;
    return 1; // Return an error code
  }

  return 0;
}