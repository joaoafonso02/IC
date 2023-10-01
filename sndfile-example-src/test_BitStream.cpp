#include <stdio.h>
 
#include "BitStream.hh"

int main(int argc, char **argv) {
  unsigned int bitCount = 10;

  std::string fileName = argv[1];
  BitStream file = BitStream(fileName);

  char* bytes = file.readBits(bitCount, 0);
  for(unsigned int i=0; i<bitCount/8 + (bitCount%8!=0); i++) {
    printf("%d: %x\n", i, (unsigned char) bytes[i]);
  }
  
  return 0;
}
