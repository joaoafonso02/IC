#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <cmath>
#include <fftw3.h>
#include <stdio.h>

#include "BitStream.hh"
#include "Wav_quant.hh"

union DoubleBitManipulation {
  double value;
  uint64_t bits;
};

void showhelp() {
    std::cerr << "Usage: wav_dct [ -c file to compress ]\n";
    std::cerr << "               [ -d file to decompress ]\n";
    std::cerr << "               [ -bs blockSize (def 1024) ]\n";
    std::cerr << "               [ -frac dctFraction (def 0.2) ]\n";
    std::cerr << "               [ -qe quantizationExponent (!do not change!) (def 11) ]\n";
    std::cerr << "               [ -qm quantizationMantissa (!do not change!) (def 11) ]\n";
    std::cerr << "               outputfile\n";
}

int main(int argc, char *argv[]) {
  bool compressfilenamegiven = false;
  bool decompressfilenamegiven = false;
  char compressfilename[80];
  char decompressfilename[80];

  char outfilename[80];

  int blockSize = 1024;
  double dctFrac = 1;

  int quantFrac = 11; // do not change (not implemented), keep at 11
  int quantMant = 10;

	if(argc < 3) {
	  showhelp();
		return 1;
	}

	for(int n=1; n<argc; n++) {
	  if(std::string(argv[n]) == "-c") {
      std::strcpy(compressfilename, argv[++n]);
      compressfilenamegiven = true;
	  }

	  if(std::string(argv[n]) == "-d") {
      std::strcpy(decompressfilename, argv[++n]);
      decompressfilenamegiven = true;
	  }

	  if(std::string(argv[n]) == "-bs") {
      blockSize = atoi(argv[++n]);
	  }

	  if(std::string(argv[n]) == "-frac") {
      blockSize = atof(argv[++n]);
	  }

	  if(std::string(argv[n]) == "-qf") {
      // quantFrac = atof(argv[++n]);
	  }

	  if(std::string(argv[n]) == "-qm") {
      quantMant = atof(argv[++n]);
	  }
	}

  std::strcpy(outfilename, argv[argc-1]);

  if(!compressfilenamegiven && !decompressfilenamegiven) {
    showhelp();
    std::cerr << "\nYou need to either compress or decompress a file.\n";
    return 2;
  }

	if(compressfilenamegiven) {
	  SndfileHandle in { compressfilename };
	  if(in.error()) {
		  std::cerr << "Error: invalid input file\n";
		  return 1;
    }

	  if((in.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		  std::cerr << "Error: file is not in WAV format\n";
		  return 1;
	  }

	  if((in.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		  std::cerr << "Error: file is not in PCM_16 format\n";
		  return 1;
	  }

	  BitStream outfile {outfilename, BitStream::w};

	  int nFrames = in.frames();
	  int nChannels = in.channels();
	  int totalSamples = nFrames * nChannels;

    short samples[blockSize*nChannels];
    
    int readSamples;
    double x[blockSize];
    fftw_plan plan_compress = fftw_plan_r2r_1d(blockSize, x, x, FFTW_REDFT10, FFTW_ESTIMATE);
    while( (readSamples = in.readf(samples, blockSize)) ) {
      for(int c=0; c<nChannels; c++) {
        for(int j=0; j<blockSize; j++) x[j] = samples[j*2+c];

        fftw_execute(plan_compress);
        for(int j=0; j<blockSize; j++) x[j] = x[j] / (blockSize<<1);
        
        Wav_quant::reduce_quantization(x, blockSize, quantFrac, quantMant); // each double only occupies 10 bits now

        union DoubleBitManipulation n;
        for(int j=0; j<blockSize*dctFrac; j++) {
          n.value = x[j];
          outfile.writeBits64(n.bits, quantFrac+quantMant+1);
        }
      }
    }
	}

	if(decompressfilenamegiven) {
    BitStream infile {decompressfilename, BitStream::r};
    SndfileHandle outfile { outfilename, SFM_WRITE, 65538 , 2, 44100 };

    int nChannels = 2;

    short samples[blockSize*nChannels];
    double x[blockSize];

    int loops = 0;
    fftw_plan plan_decompress = fftw_plan_r2r_1d(blockSize, x, x, FFTW_REDFT01, FFTW_ESTIMATE);
    union DoubleBitManipulation n;
    while( true ) {
      for(int c=0; c<nChannels; c++) {
        for(int j=0; j<blockSize; j++) {
          if( j<blockSize*dctFrac ) {
            int err = infile.readBits64(&n.bits, quantFrac+quantMant+1);
            if( err ) return 0;
            x[j] = n.value;
          } else {
            x[j] = 0;
          }
        }
        Wav_quant::inverse_reduce_quantization(x, blockSize, quantFrac, quantMant);

        fftw_execute(plan_decompress);
        
        for(int j=0; j<blockSize; j++) samples[j*2+c] = x[j];
      }
      outfile.writef(samples, blockSize);
    }
	}
}


