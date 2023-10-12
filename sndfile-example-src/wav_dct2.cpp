#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <cmath>
#include <fftw3.h>
#include "BitStream.hh"

using namespace std;

int main(int argc, char* argv[]) {

  size_t blockSize { 1024 }; 
  double dctFrac { 0.125 };

  size_t i, j, k, c, err;

  // Run with: ./wav_dct2 <compress | decompress> <target_file> <output>
  if( argc!=4 ) {
    cerr << "Run with: ./wav_dct2 <compress | decompress> <target_file> <output>";
    return 1;
  }

  if( std::string(argv[1])=="compress" ) {
    SndfileHandle inFile { argv[2] };
    if( inFile.error() ) {
      cerr << "Error: invalid input file\n";
      return 1;
    }

    BitStream outFile { argv[3], BitStream::w }; 

    vector<double> x(blockSize);
    vector<float> xfile(blockSize * 2);
    vector<short> samples(blockSize * 2);
    fftw_plan plan_d = fftw_plan_r2r_1d(blockSize, x.data(), x.data(), FFTW_REDFT10, FFTW_ESTIMATE);
	  fftw_plan plan_i = fftw_plan_r2r_1d(blockSize, x.data(), x.data(), FFTW_REDFT01, FFTW_ESTIMATE);
    k = 0;
    int readSamples;
    while( (readSamples = inFile.readf(samples.data(), blockSize))!=0) {
      for(c=0; c<2; c++) {
        for(j=0; j<readSamples; j++) x[j] = samples[j*2+c]; // cast of short to double
        fftw_execute(plan_d);
        for(j=0; j<readSamples; j++) xfile[j*2+c] = x[j] / (blockSize << 1); // cast of short to double
      }
      
      // write to file
      outFile.writeBits((unsigned char*) xfile.data(), (int) readSamples*2*32*dctFrac, 0);  
    }

  } else if( std::string(argv[1])=="decompress" ) {

    BitStream inFile { argv[2], BitStream::r };
    SndfileHandle outFile { argv[3], SFM_WRITE, 65538 , 2, 44100 };

    vector<short> samples(blockSize*2);
    vector<float> xfile(blockSize*2);
    vector<double> x(blockSize);
	  fftw_plan plan_i = fftw_plan_r2r_1d(blockSize, x.data(), x.data(), FFTW_REDFT01, FFTW_ESTIMATE);

    for(i=0; i<1000; i++) {
      inFile.readBits((unsigned char*) xfile.data(), (int) blockSize*2*32*dctFrac);

      for(c=0; c<2; c++) {
        for(j=0; j<blockSize*dctFrac; j++) x[j] = xfile[j*2+c]; // cast of short to double
        for(j=blockSize*dctFrac; j<blockSize; j++) x[j] = 0;
        fftw_execute(plan_i);
        for(j=0; j<blockSize; j++) samples[j*2+c] = static_cast<short>(round(x[j])); // cast of short to double
      }

      outFile.writef(samples.data(), blockSize);
	  }

  } else {
    cerr << "First argument must be compress or decompress"; 
  }

  return 0;
}
