#ifndef GOLOMB_HH
#define GOLOMB_HH

#include "./BitStream.hh"
#include <sys/types.h>

class Golomb {
private:
  uint m;
  uint b;
  uint c;
  BitStream *bs;
public:
  Golomb(uint m, BitStream *bs);
  int encode(int n);
  int decode(int *g);
};

#endif
