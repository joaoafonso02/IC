#include <iostream>
#include <fstream>
#include "BitStream.hh"

int main(int argc, char **argv) {
  if (argc != 3) {
      std::cerr << "Usage: " << argv[0] << " <filename> <mode (w/r)>" << std::endl;
      return 1; // Return an error code
  }

  std::string fileName = argv[1], mode = argv[2];
  std::string str= "string de teste 123123o+j.das+p+eW'É0+  ";
  unsigned char data[] = {0xA7, 0xC3};

  unsigned int bitCount = str.length() * 8 + 22; // +22 cause im writing 11 bits + string + 11 bits

  BitStream::Flag flag;
  
  if (mode == "w") {
    flag = BitStream::w;
    BitStream file(fileName, flag);

    file.writeBits(data, 11, 0);
    file.writeString1(str);
    file.writeBits(data, 11, 0);
    file.writeBitFlush();

  } else if (mode == "r") {
    flag = BitStream::r;
    BitStream file(fileName, flag);
    std::cout << "Read Bits: ";

    unsigned char outBuffer[100]; 
    int outBufferSize = bitCount / 8 + (bitCount % 8 != 0);

    // Read bits
    file.readBits(outBuffer, bitCount);

    // print as hexadecimal
    for (int i = 0; i < outBufferSize; i++) {
      std::cout << std::hex << (int)outBuffer[i] << " ";
    }
    std::cout << std::dec << std::endl;
  } else {
    std::cerr << "Invalid mode. Use 'w' for write or 'r' for read." << std::endl;
    return 1; // Return an error code
  }

  return 0;
}
