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
  uint nn = n;
  if( n<0 ) nn = -n*2-1;
  else nn = n*2;
  
  uint q = nn/m;
  uint r = nn%m;

  uint64_t code = 0;
  uint i;
  for(i=0; i<q; i++) {
    code = (code << 1) | 0x1;
  }

  int truncated = r >= (b-1)*(b-1)-1;
  code = (code << (b+truncated)) | (truncated ? (0x1<<(b-1)) : 0) | (r-truncated);

  int size = q+b+truncated;
  bs->writeNBits(code << (64-size), size);

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

  int truncated = 0;
  int i = 0;
  while( i<b+truncated-1 ) {
    bs->readBit(&bit); 
    r = (r<<1) | bit;
    i++;
    if(i==2 && r==0x3) truncated = 1;
  }

  int n = m*q + (!truncated ? r : (r&~(0x1<<(i-1)))+1);
  if( n%2==1 ) {
    n = -(n+1)/2;
  } else {
    n = n/2;
  }
  
  return n;
}
