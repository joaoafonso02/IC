#include "include/BitStream.hh"
#include "include/Golomb.hh"
#include <stdio.h>
#include <sndfile.hh>

#define BLOCKSIZE 1024
#define GOLOMB_M 2048;

int main(int argc, char** argv) {
  // find best M (go throw all the file and create a histogram)
  int i;

  SndfileHandle wav = SndfileHandle("./src/wabes/sample02.wav");
  printf("sr %d\n\n", wav.samplerate());

  int histogram[32768*2] = {0};
  int bs = BLOCKSIZE;

  short samples[bs*wav.channels()];
  int nframes;
  short last = 0;
  while( (nframes = wav.readf(samples, bs)) ) {
    for(i=0; i<nframes*wav.channels(); i+=wav.channels() ) { // +=channels to read only one channel
      // show the diference between using the difference between frame to the frame itself
      int j = samples[i]>=0 ? samples[i]*2 : -samples[i]*2-1;
      histogram[j]+=1;
      // last = samples[i]; 
      // histogram[samples[i]+32768]+=1;
    }
  }

  double geometric = 0;
  int completed = 0;
  for(i=0; i<5000; i+=500) {
    double a = histogram[i];
    double b = histogram[i+2];
    printf("%f\n", b/a);
    // if( a==0 ) a=1;
    // if( a==0 ) continue;
    // if( b==0 ) continue;
    geometric += b/a;
  }
  printf("GEOMETRIC %d: %f\n", completed, geometric );

  // FILE *fout = fopen("data", "w");
  // for(i=0; i<32768*2; i++) {
  //   fprintf(fout, "%5d %5d\n", i, histogram[i]);
  // }
  // fclose(fout);
  
  // TODO: find best M

  int m = GOLOMB_M;
  wav.seek(0, 0);

  BitStream fs = BitStream("golomb.bin", BitStream::w);
  Golomb g = Golomb(m, &fs);

  short prev[2] = {0, 0};
  short error;
  while( (nframes = wav.readf(samples, bs)) ) {
    printf("NEW BLOCK\n");
    for(i=0; i<nframes*wav.channels(); i++ ) {
      error = samples[i]-prev[i%2];
      g.encode(error);
      prev[i%2] = samples[i];
    }
  }

  return 0;
}
