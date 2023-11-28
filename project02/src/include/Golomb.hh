#ifndef GOLOMB_HH
#define GOLOMB_HH

#include "./BitStream.hh"
#include <cstdint>

class Golomb {
private:
  uint32_t m;
  uint32_t b;
  uint32_t c;
  BitStream *bs;
public:
  Golomb(uint32_t m, BitStream *bs);
  int64_t encode(int64_t n);
  int64_t decode(int64_t *g);
};

#endif
