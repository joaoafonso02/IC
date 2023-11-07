#include <iostream>
#include <math.h>

// dev tools
#include <bitset>

#include "./Golomb.hh"
#include "./BitStream.hh"

Golomb::Golomb(uint im, BitStream *ibs) {
  m = im;
  b = ceil(log2(m));
  bs = ibs;
}

int Golomb::encode(int n) {
  uint nn;
  if( n<0 ) nn = -n*2-1;
  else nn = n*2;
  
  uint q = nn/m;
  uint r = nn%m;

  uint64_t code = 0;
  uint i;
  for(i=0; i<q; i++) {
    code = (code << 1) | 0x1;
  }
  code = (code << (1 + b)) | r; 

  int size = q+1+b;
  bs->writeNBits(code << (64-size), q+1+b);

  return 0;
}

int Golomb::decode() {
  uint q=0;
  uint64_t r=0;
  
  uint8_t bit = 1;
  while(bit==1) {
    bs->readBit(&bit);
    q++;
  }
  q--;

  bs->readNBits(&r, b);
  r >>= 64-b;

  int n = m*q + r;
  if( n%2==1 ) {
    n = -(n+1)/2;
  } else {
    n = n/2;
  }
  return n;
}
