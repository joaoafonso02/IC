#include "./include/Golomb.hh"
#include "./include/BitStream.hh"


int main(int argc, char **argv) {

  printf("USAGE: ./test_Golomb <file> <0 to write | 1 to read>\n\n");

  int a=-20, b=20, c=1;
  if(*argv[2]=='0') {
    BitStream bs = BitStream(argv[1], BitStream::w);
    Golomb gol = Golomb(5, &bs);
    for(int i=a; i<b; i+=c) {
      gol.encode(i);
    }
  } else {
    BitStream bs = BitStream(argv[1], BitStream::r);
    Golomb gol = Golomb(5, &bs);
    for(int i=a; i<b; i+=c) {
      printf("%d -> %d\n", i, gol.decode());
    }
  }
  return 0;
}
