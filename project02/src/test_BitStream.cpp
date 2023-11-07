#include "include/BitStream.hh"
#include <vector>
#include <stdio.h>

int main() {
  int n = 11;
  uint64_t data = 0xA6BC000000000000;
  
  // BitStream write {"bin.bin", BitStream::w};
  // write.writeNBits(data, n);

  BitStream read {"bin.bin", BitStream::r};
  read.readNBits(&data, n);
  printf("Data: %lx\n", data);
}
