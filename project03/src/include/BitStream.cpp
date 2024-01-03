#include <fstream>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "BitStream.hh"

BitStream::BitStream(std::string filename, BitStream::Flag fg) {
  flag = fg;
  fs.open(filename, (flag==r ? std::ios::in : std::ios::out) | std::ios::binary);
  buffer = 0;
  bufferPos = 0;
};

uint8_t BitStream::writeBit(uint8_t bit) {
  if( flag!=w ) {
    std::cerr << "[BitStream::writeBit] Cannot write a bit in a BitStream of type read\n";
    exit(1);
  }
  buffer = (buffer<<1) | bit;
  bufferPos++;

  if(bufferPos==8) {
    fs.put(buffer);
    buffer = 0;
    bufferPos = 0;
  }
  return 0;
}

uint8_t BitStream::writeNBits(uint64_t data, uint8_t n) {
  if( flag!=w ) {
    std::cerr << "[BitStream::writeNBits] Cannot write bits in a BitStream of type read\n";
    exit(1);
  }
  int i;
  for(i=63; i>63-n; i--) {
    BitStream::writeBit((data >> i) & 0x1);
  }
  return 0;
}

uint8_t BitStream::readBit(uint8_t *bit) {
  if( flag!=r ) {
    std::cerr << "[BitStream::readBit] Cannot read a bit in a BitStream of type write\n";
    exit(1);
  }
  if(bufferPos%8==0) {
    char a;
    if(!fs.get(a)) {
      return 1;
    }
    buffer = (uint8_t) a;
    bufferPos = 0;
  }
  *bit = (buffer >> (7-bufferPos)) & 0x1;
  bufferPos++;
  return 0;
}

uint8_t BitStream::readNBits(uint64_t *data, uint8_t n) {
  if( flag!=r ) {
    std::cerr << "[BitStream::readNBits] Cannot read bits in a BitStream of type write\n";
    exit(1);
  }
  *data = 0;
  uint8_t i, bit;
  for(i=0; i<n; i++) {
    if( BitStream::readBit(&bit) ) return 1;
    *data = (*data << 1) | bit;
  }
  *data <<= (64 - i);
  return 0;
}

BitStream::~BitStream() {
  if( flag==w && bufferPos!=0 ) {
    fs.put(buffer << (8-bufferPos));
  }
  fs.close();
}
