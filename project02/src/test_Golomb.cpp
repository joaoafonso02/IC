#include "./include/Golomb.hh"
#include "./include/BitStream.hh"


int main(int argc, char **argv) {

  printf("USAGE: ./test_Golomb <file> <0 to write | 1 to read>\n\n");

  int m = 5;

  int a=-50000, b=50000, c=1; 
  if(*argv[2]=='0') {
    BitStream bs = BitStream(argv[1], BitStream::w);
    Golomb g = Golomb(m, &bs);
    for(int i=a; i<b; i+=c) {
      g.encode(i);
    }
  } else {
    BitStream bs = BitStream(argv[1], BitStream::r);
    Golomb g = Golomb(m, &bs);
    int n;
    for(int i=a; i<b; i+=c) {
      if(g.decode(&n)) {
        printf("ERROR");
        return 1;
      }
      if( i!=n ) {
        printf("i: %d -> %d\n", i, n);
        return 1;
      }
    }
  }

  return 0;
}
