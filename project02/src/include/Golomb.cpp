#include <iostream>
#include <math.h>

// dev tools
#include <bitset>

#include "./Golomb.hh"
#include "./BitStream.hh"

Golomb::Golomb(uint im, BitStream *ibs) {
  m = im;
  b = floor(log2(m));
  cutoff = pow(b+1, 2) - m;
  bs = ibs;
}

int Golomb::encode(int n) {
  uint64_t nn;
  if( n<0 ) nn = -n*2-1;
  else nn = n*2;

  uint q = nn/m;
  uint r = nn%m;

  uint64_t code = 0;

  // add q to code
  uint i;
  for(i=0; i<q; i++) {
    code = (code << 1) | 0x1;
  }
  code <<= 1;

  uint truncated = r >= cutoff;
  if( truncated ) {
    code = (code << (b+1)) | (r + cutoff);
  } else {
    code = (code << b) | r;
  }

  uint size = q+1+b+(int)truncated;
  printf("-> %d : %lx %d\n", nn, code, size);
  bs->writeNBits(code << (64-size), size);

  return 0;
}

int Golomb::decode(int *g) {
  int err;
  uint q=0;
  uint64_t r=0;
  
  uint8_t bit = 1;
  while(bit==1) {
    err = bs->readBit(&bit);
    if(err) return 1;
    q++;
  }
  q--;

  bs->readNBits(&r, b);
  r >>= 64-b;
  if( r>=cutoff ) {
    err = bs->readBit(&bit); 
    if(err) return 1;
    r = (r<<1) | bit;
    r = r - cutoff;
  }
  // printf("q: %d  r: %lx\n", q, r);

  int64_t n = q*m + r;
  if( n%2==1 ) {
    *g = -(int)((n+1)/2);
  } else {
    *g = (int)(n/2);
  }
  
  return 0;
}
