#include "include/BitStream.hh"
#include "include/Golomb.hh"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <sndfile.hh>

#define TRUE 1
#define FALSE 0
#define NOPREDICT 1
#define TEMPREDICT 2
#define INTCHNPREDICT 3

#define i16 32768
#define BLOCKSIZE 1024
#define OUTPUT_HISTOGRAM FALSE
#define PREDICT TEMPREDICT
// #define QUANT 12


static uint64_t wav_histogram(uint32_t *histogram, SndfileHandle *wav, uint32_t quant) {
  uint64_t i;
  int16_t samples[BLOCKSIZE*wav->channels()], prev[wav->channels()];
  uint64_t nframes, totalSamples = 0;

  // initialization
  for(i=0; i<uint32_t(wav->channels()) ;i++) prev[i] = 0;
  for(i=0; i<uint32_t(i16 * wav->channels()) >>quant ;i++) histogram[i] = 0;

  // calculate histogram (three different predictions available)
  wav->seek(0, 0);
  while( (nframes = wav->readf(samples, BLOCKSIZE)) ) {
    for(i=0; i<nframes*wav->channels(); i+=1 ) {
      if( PREDICT == NOPREDICT ) {
        // wav samples
        int j = samples[i]>=0 ? samples[i]*2 : -samples[i]*2-1;
        histogram[j]+=1;

      } else if( PREDICT == TEMPREDICT ) {
        // temporal prediction for both channels
        short j = (samples[i]>>quant)-prev[i%2];
        int jj = j>=0 ? j*2 : -j*2-1;
        histogram[jj] += 1;
        prev[i%2] = samples[i]>>quant; 

      } else if( PREDICT == INTCHNPREDICT ) {
        // temporal prediction + inter-channel for right channel
        if( i%2==0 ) { // 
          short j1 = samples[i]-prev[0]; // left channel error
          short j2 = samples[i]-samples[i+1]; // right channel error
          histogram[j1>=0 ? j1*2 : -j1*2-1] += 1;
          histogram[j2>=0 ? j2*2 : -j2*2-1] += 1;
          prev[0] = samples[i];
        }
      }
    }
    totalSamples += nframes*wav->channels();
  }

  return totalSamples;
}

static uint32_t calc_golombM(SndfileHandle *wav, uint32_t quant) {
  uint32_t i, m;
  uint32_t histogram[(i16 * wav->channels()) >>quant];
  uint64_t totalSamples = wav_histogram(histogram, wav, quant);

  double geometric = 1;
  for(i=1; i < (i16*2)>>quant; i+=1) {
    geometric += i * (double(histogram[i])/double(totalSamples));
  }
  geometric = 1/geometric;

  // printf("p: %f\n", geometric);
  m = (uint) -ceil(log2(2-geometric)/log2(1-geometric));
  if( m<1 ) m=1; // dont let m became 0
  // printf("m: %d\n", m);

  return m;
}

static uint64_t calc_bitRate(SndfileHandle *wav, uint32_t quant, uint32_t m) {
  // Calculate Bit Rate
  uint32_t i, nframes;
  int32_t error;
  uint64_t totalSamples = 0, bitCount = 0;
  Golomb gr = Golomb(m, NULL);
  int16_t samples[BLOCKSIZE*wav->channels()], prev[wav->channels()];

  for(i=0; i<uint32_t(wav->channels()); i++) prev[i] = 0;

  wav->seek(0, 0);
  while( (nframes = wav->readf(samples, BLOCKSIZE)) ) {
    for(i=0; i<nframes*wav->channels(); i++ ) {
      error = (samples[i]>>quant)-prev[i%2];
      bitCount += gr.encode_size(error);
      prev[i%2] = samples[i]>>quant;
    }
    totalSamples += nframes*wav->channels();
  }

  // Debug
  // printf("BitRate: %d %d %ld \n", m, quant, bitCount);
  // printf("BitRate: %lf\n", 44100*wav->channels()*double(bitCount)/totalSamples); // 44100 => SampleRate
  // printf("BitRate: %.0lfkbps\n", 44100*wav->channels()*double(bitCount)/totalSamples /1024); // 44100 => SampleRate
                                                                                            
  return (uint64_t) 44100*wav->channels()*double(bitCount)/totalSamples;
}

int main(int argc, char** argv) {
  char oFile[50] = "golomb.bin";
  uint32_t i, m, quant = 0;
  uint32_t target_bitrate = 0, bitrate;
  short error;

  bool bquant = FALSE, bm = FALSE, bbitrate = FALSE;

  std::cout << "\nUsage: wavEncode <fileToEncode>\n";
  std::cout << "               [ -q quantization (def auto) ]\n";
  std::cout << "               [ -m golomb (def auto) ]\n";
  std::cout << "               [ -b target bitrate (kbps) (def auto) ]\n\n";
  std::cout << "               [ -o output file (def golomb.bin) ]\n\n";

  for(int n = 1 ; n < argc ; n++) {
    if(std::string(argv[n]) == "-q") {
      quant = atoi(argv[n+1]);
      bquant = TRUE;
      break;
    }
  }

  for(int n = 1 ; n < argc ; n++) {
    if(std::string(argv[n]) == "-m") {
      m = atoi(argv[n+1]);
      bm = TRUE;
      break;
    }
  }

  for(int n = 1 ; n < argc ; n++) {
    if(std::string(argv[n]) == "-b") {
      target_bitrate = atoi(argv[n+1]);
      bitrate = target_bitrate*1024+1;
      bbitrate = TRUE;
      break;
    }
  }

  for(int n = 1; n < argc ; n++) {
    if(std::string(argv[n]) == "-o") {
      std::strcpy(oFile, argv[n+1]);
      break;
    }
  }


  SndfileHandle wav = SndfileHandle(argv[1]);

  // write histogram to file
  if( OUTPUT_HISTOGRAM ) {
    FILE *fout = fopen("data", "w");
    uint32_t histogram[(i16 * wav.channels()) >>quant];
    wav_histogram(histogram, &wav, quant);
    for(i=0; i<uint32_t(i16 * wav.channels()) >>quant; i++) {
      fprintf(fout, "%5d %5d\n", i, histogram[i]);
    }
    fclose(fout);
  }

  if( bbitrate ) {
    quant = -1;
    bitrate = target_bitrate*2000;
    while( bitrate/1024 > target_bitrate ) {
      quant++;
      if( quant>=14 ) {
        printf("Not able to compress more!\n");
        break;
      }
      if( !bm ) m = calc_golombM(&wav, quant); 
      bitrate = calc_bitRate(&wav, quant, m);
      printf("q:%d m:%d %dkbps\n", quant, m, bitrate/1024);
    }
  } else {
    if(!bm) m = calc_golombM(&wav, quant);
  }
  

  // Debug
  printf("Encoded m: %d\n", m);
  printf("Encoded quantization: %d\n", quant);
  printf("Encoded bitrate: %ldkbps\n", calc_bitRate(&wav, quant, m)/1024);

  // Generate Encoded File 
  BitStream fs = BitStream(oFile, BitStream::w);
  fs.writeNBits(uint64_t(m)<<(64-16), 16); // first 16 bits store the golomb parameter
  fs.writeNBits(uint64_t(quant)<<(64-8), 8); // next 8 bits are used to store the used QUANTization
  Golomb g = Golomb(m, &fs);

  uint32_t nframes;
  int16_t samples[BLOCKSIZE*wav.channels()], prev[wav.channels()];
  for(i=0; i<uint32_t(wav.channels()); i++) prev[i] = 0;

  wav.seek(0, 0);
  while( (nframes = wav.readf(samples, BLOCKSIZE)) ) {
    for(i=0; i<nframes*wav.channels(); i++ ) {
      error = (samples[i]>>quant)-prev[i%2];
      g.encode(error);
      prev[i%2] = samples[i]>>quant;
    }
  }

  printf("\n\n");
  return 0;
}
