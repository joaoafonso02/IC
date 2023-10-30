#ifndef Wav_quant_HH
#define Wav_quant_HH

#include <iostream>
#include <sndfile.hh>
#include <vector>
#include <string>

class Wav_quant {
private:
  union DoubleBitManipulation {
    double value;
    uint64_t bits;
  };
public:
  static void reduce_quantization(short* samples, int nSamples, int newBits) {
    short mask = 0xFFFF;
    mask <<= 16-newBits;
    for(int i=0; i<nSamples; i++) {
      samples[i] = samples[i] & mask;
    }
  }

  static void reduce_quantization(double* samples, int nSamples, int newExpBits, int newMantissaBits) {
    union DoubleBitManipulation s;
    for(int i=0; i<nSamples; i++) {
      s.value = samples[i];
      s.bits &= ~(0LU) << (52-newMantissaBits);
      uint64_t signals = (s.bits & 3LU<<62); 
      // s.bits = (s.bits << (11 - newExpBits)) & ~(0LU)>>(2) | (s.bits & 3LU<<62);
      // s.bits = (s.bits & (3ULL<<62)) | (s.bits << (11 - newExpBits - 1));
      samples[i] = s.value;
    }
  }
  static void inverse_reduce_quantization(double* samples, int nSamples, int newExpBits, int newMantissaBits) {
    union DoubleBitManipulation s;
    for(int i=0; i<nSamples; i++) {
      s.value = samples[i];
      s.bits = (s.bits & ~(0LU)>>2) >> (11-newExpBits) | (s.bits & (3LU)<<62);
      // s.bits = (s.bits & (3ULL<<62)) | ((s.bits & ~(3ULL<<62)) << (11-newExpBits - 1));
      samples[i] = s.value;
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
      outfile.writef(samples, frames);
    }
  }
};

#endif
