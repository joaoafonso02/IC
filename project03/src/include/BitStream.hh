#ifndef BITSTREAM_HH
#define BITSTREAM_HH

#include <fstream>
#include <stdlib.h>
#include <string>

class BitStream {
public:
  enum Flag { r, w };
private:
  std::fstream fs;
  Flag flag;
  uint8_t buffer;
  uint8_t bufferPos;
public:
  BitStream(std::string filename, Flag flag);
  uint8_t writeBit(uint8_t bit);
  uint8_t writeNBits(uint64_t data, uint8_t n);
  uint8_t readBit(uint8_t *bit);
  uint8_t readNBits(uint64_t *data, uint8_t n);
  ~BitStream();
};

#endif
