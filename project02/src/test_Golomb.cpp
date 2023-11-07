#include "./include/Golomb.hh"
#include "./include/BitStream.hh"


int main() {
  int a=-20, b=20, c=1;
  if(0) {
    BitStream bs = BitStream("golomb.bin", BitStream::w);
    Golomb gol = Golomb(4, &bs);
    for(int i=a; i<b; i+=c) {
      gol.encode(i);
    }
  } else {
    BitStream bs = BitStream("golomb.bin", BitStream::r);
    Golomb gol = Golomb(4, &bs);
    for(int i=a; i<b; i+=c) {
      printf("%d -> %d\n", i, gol.decode());
    }
  }
  return 0;
}
