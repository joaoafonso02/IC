#ifndef Wav_quant_HH
#define Wav_quant_HH

#include <iostream>
#include <sndfile.hh>
#include <vector>
#include <string>

class Wav_quant {
public:
  static void reduce_quantization(short* samples, int nSamples, int newBits) {
    short mask = 0xFFFF;
    mask <<= 16-newBits;
    for(int i=0; i<=nSamples; i++) {
      samples[i] = samples[i] & mask;
    }
  }

  static void reduce_quantization(std::string inFile, std::string outFile, int newBits) {
    SndfileHandle infile { inFile };
    if( (infile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV ) {
      std::cerr << "Error: infile not in WAV format\n";
      throw;
    }

    SndfileHandle outfile { outFile , SFM_WRITE, infile.format(), infile.channels(), infile.samplerate() };
    if( outfile.error() ) {
      std::cerr << "Error: infile not in WAV format\n";
      throw;
    }

    int blocksize = 65536;
    short samples[blocksize * infile.channels()];
    size_t frames;
    while( (frames = infile.readf(samples, blocksize)) ) {
      Wav_quant::reduce_quantization(samples, blocksize * infile.channels(), newBits);
      outfile.writef(samples, blocksize);
    }
  }
};

#endif
