#include <iostream>
#include <math.h>

// dev tools
#include <bitset>

#include "./BitStream.hh"
#include "./Golomb.hh"

Golomb::Golomb(uint32_t _m, BitStream *_bs) {
  m = _m;
  b = floor(log2(m));
  c = pow(2, b + 1) - m;
  bs = _bs;
}

int64_t Golomb::encode(int64_t n) {
  uint64_t nn;
  if (n < 0)
    nn = -n * 2 - 1;
  else
    nn = n * 2;

  uint64_t q = nn / m;
  uint64_t r = nn % m;

  uint64_t code = 0;

  // add q to code
  uint64_t i;
  for (i = 0; i < q; i++) {
    bs->writeBit(1);
  }
  bs->writeBit(0);

  uint64_t truncated = r >= c;
  if (truncated) {
    code = r + c;
  } else {
    code = r;
  }

  uint64_t size = b + truncated;
  bs->writeNBits(code << (64 - size), size);

  return 0;
}

uint64_t Golomb::encode_size(int64_t n) {
  uint64_t nn = n<0 ? uint64_t(-n*2-1) : n*2;
  return (nn/m)+1 + b + (nn%m>=c);
}

int64_t Golomb::decode(int64_t *g) {
  int err;
  uint q = 0;
  uint64_t r = 0;

  uint8_t bit = 1;
  while (bit == 1) {
    err = bs->readBit(&bit);
    if (err)
      return 1;
    q++;
  }
  q--;

  bs->readNBits(&r, b);
  r >>= 64 - b;
  if (r >= c) {
    err = bs->readBit(&bit);
    if (err)
      return 1;
    r = (r << 1) | bit;
    r = r - c;
  }
  // printf("q: %d  r: %lx\n", q, r);

  int64_t n = q * m + r;
  if (n % 2 == 1) {
    *g = -(int)((n + 1) / 2);
  } else {
    *g = (int)(n / 2);
  }

  return 0;
}
