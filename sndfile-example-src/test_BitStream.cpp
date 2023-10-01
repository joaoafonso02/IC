#include <stdio.h>
#include <vector>
#include "BitStream.hh"

int main(int argc, char **argv) {
  unsigned int bitCount = 10;
  char data[] = {0x11, 0x22, 0x55, 0x78, 0x6A};

  std::string fileName = argv[1];
  BitStream file = BitStream(fileName);

  char* bytes = file.readBits(bitCount, 0);
  for(unsigned int i=0; i<bitCount/8 + (bitCount%8!=0); i++) {
    printf("%d: %x\n", i, (unsigned char) bytes[i]);
  }


  bytes = file.writeBits(data, bitCount, 0);
  printf("\nAfter Writing bits\n");
  for(unsigned int i=0; i<bitCount/8 + (bitCount%8!=0); i++) {
    printf("%d: %x\n", i, (unsigned char) bytes[i]);
  }

  for(unsigned int i=0; i<bitCount/8 + (bitCount%8!=0); i++) {
    if((unsigned char) bytes[i] != (unsigned char) data[i]) {  
      printf("ERROR: %d: %x != %x\n", i, (unsigned char) bytes[i], (unsigned char) data[i]);
      return 0; 
    }
  }

  printf("SUCCESS\n");

  

  return 0;
}
