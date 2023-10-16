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


  unsigned int bitCount = str.length() * 8;

  BitStream::Flag flag;
  
  if (mode == "w") {
    flag = BitStream::w;
    BitStream file(fileName, flag);

    file.writeBits(data, 11, 0);
    file.writeString2(str);
    file.writeBits(data, 11, 0);
    file.writeBitFlush();

  } else if (mode == "r") {
    flag = BitStream::r;
    BitStream file(fileName, flag);
    // read bits
    std::cout << "Read Bits: \n";
    unsigned char* outBuffer = (unsigned char*) malloc(bitCount/8 + (bitCount%8!=0));
    int outBufferSize = file.readBits(outBuffer, bitCount+11);

    std::cout << "String: " << outBuffer << std::endl;

    for(int i=outBufferSize-2; i<outBufferSize; i++) {
      printf(" %02x", outBuffer[i]);
    }
    std::cout << std::endl;
  } else {
    std::cerr << "Invalid mode. Use 'w' for write or 'r' for read." << std::endl;
    return 1; // Return an error code
  }

  return 0;
}
