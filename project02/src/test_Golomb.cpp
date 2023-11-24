#include "./include/Golomb.hh"
#include "./include/BitStream.hh"


int main(int argc, char **argv) {

  printf("USAGE: ./test_Golomb <file> <0 to write | 1 to read>\n\n");

  int m = 50;

  int a=0, b=100, c=1;
  if(*argv[2]=='0') {
    BitStream bs = BitStream(argv[1], BitStream::w);
    Golomb g = Golomb(m, &bs);
    for(int i=a; i<b; i+=c) {
      printf("%d ", i);
      g.encode(i);
    }
  } else {
    BitStream bs = BitStream(argv[1], BitStream::r);
    Golomb g = Golomb(m, &bs);
    int n, nerr = 0;
    for(int i=a; i<b; i+=c) {
      g.decode(&n);
      if( i!=n ) {
        printf("i: %d -> %d\n", i, n);
        nerr++;
      }
    }
    printf("nErr: %d %f\n", nerr, nerr/double(b-a));
  }

  return 0;
}
