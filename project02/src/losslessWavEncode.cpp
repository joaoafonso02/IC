#include "include/BitStream.hh"
#include "include/Golomb.hh"
#include <cstdint>
#include <math.h>
#include <stdio.h>
#include <sndfile.hh>

#define BLOCKSIZE 1024
#define GOLOMB_M 512;

int main(int argc, char** argv) {
  // find best M (go throw all the file and create a histogram)
  int i;

  SndfileHandle wav = SndfileHandle("./src/wabes/sample05.wav");
  printf("sr %d\n\n", wav.samplerate());

  int histogram[32768*2] = {0};
  int bs = BLOCKSIZE;

  short samples[bs*wav.channels()];
  int nframes;
  short prev[2] = {0};
  uint64_t totalSamples = 0;
  while( (nframes = wav.readf(samples, bs)) ) {
    for(i=0; i<nframes*wav.channels(); i+=wav.channels() ) { // +=channels to read only one channel
      // show the diference between using the difference between frame to the frame itself
      // int j = samples[i]>=0 ? samples[i]*2 : -samples[i]*2-1;
      // histogram[j]+=1;
      short j = samples[i]-prev[i%2];
      int jj = j>=0? j*2 : -j*2-1;
      histogram[jj]+=1;
      prev[i%2] = samples[i]; 

    }
    totalSamples += nframes;
  }

  double geometric = 0;
  for(i=1; i<32768*2; i+=1) {
    double a = (double) histogram[i]/(double) totalSamples;
    geometric += i*a;
  }
  geometric = 1-1/geometric;
  printf("p: %f\n", geometric);
  uint32_t m = (uint) -ceil(log2(1+geometric)/log2(geometric));
  printf("m: %d\n", m);


  // FILE *fout = fopen("data", "w");
  // for(i=0; i<32768*2; i++) {
  //   fprintf(fout, "%5d %5d\n", i, histogram[i]);
  // }
  // fclose(fout);
  
  wav.seek(0, 0);

  BitStream fs = BitStream("golomb.bin", BitStream::w);
  fs.writeNBits(uint64_t(m)<<32, 32);
  Golomb g = Golomb(m, &fs);

  prev[0] = 0;
  prev[1] = 0;
  short error;
  while( (nframes = wav.readf(samples, bs)) ) {
    // printf("NEW BLOCK\n");
    for(i=0; i<nframes*wav.channels(); i++ ) {
      error = samples[i]-prev[i%2];
      g.encode(error);
      prev[i%2] = samples[i];
    }
  }

  return 0;
}
