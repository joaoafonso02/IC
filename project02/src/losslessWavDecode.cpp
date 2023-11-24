#include "include/BitStream.hh"
#include "include/Golomb.hh"
#include <stdio.h>
#include <sndfile.hh>
#include <sys/types.h>

#define BLOCKSIZE 1024
#define GOLOMB_M 2048

int main(int argc, char** argv) {
  BitStream fs = BitStream("golomb.bin", BitStream::r);
  Golomb g = Golomb(GOLOMB_M, &fs);

  SndfileHandle wav = SndfileHandle("out.wav", SFM_WRITE, 65538, 2, 44100);

  uint bs = BLOCKSIZE;

  int err, n;
  short prev[2] = {0, 0}, buffer[bs*2];
  uint i = 0;
  int bsi = 0;
  while(1) {
    err = g.decode(&n);
    if(i>=bs*2 || err) {
      wav.writef(buffer, i/2);
      i = 0;
      bsi++;
    }
    prev[i%2] = prev[i%2]+n;
    buffer[i] = prev[i%2];
    i++;
    if(err) break;
  }
  printf("Written %d blocks\n", bsi);

  return 0;
}
