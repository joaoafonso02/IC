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
    std::cerr << "               outputfile\n";
}

int main(int argc, char *argv[]) {
  bool compressfilenamegiven = false;
  bool decompressfilenamegiven = false;
  char compressfilename[80];
  char decompressfilename[80];

  char outfilename[80];

  int blockSize = 1024;
  double dctFrac = 0.2;

  int quantFrac = 11;
  int quantMant = 2;// 64-32-1-7;

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
    SndfileHandle outfile2 { "wavs/sample01.out.wav", SFM_WRITE, 65538 , 2, 44100 };

	  int nFrames = in.frames();
	  int nChannels = in.channels();
	  int totalSamples = nFrames * nChannels;

    short samples[blockSize*nChannels];
    
    int readSamples;
    double x[blockSize];
    double max = 0, min = 0;
    fftw_plan plan_compress = fftw_plan_r2r_1d(blockSize, x, x, FFTW_REDFT10, FFTW_ESTIMATE);
    fftw_plan plan_decompress = fftw_plan_r2r_1d(blockSize, x, x, FFTW_REDFT01, FFTW_ESTIMATE);
    while( (readSamples = in.readf(samples, blockSize)) ) {
      for(int c=0; c<nChannels; c++) {
        for(int j=0; j<blockSize; j++) x[j] = samples[j*2+c];

        fftw_execute(plan_compress);
        for(int j=0; j<blockSize; j++) x[j] = x[j] / (blockSize<<1);
        

        // std::cout <<std::hex;
        // for(int j=0; j<blockSize; j++) std::cout << std::hex << (uint64_t) x[j] << " ";
        // for(int j=0; j<blockSize; j++) printf(" %04.x", (uint64_t) x[j]);
        // std::cout <<std::dec;
        std::cout << "\n\n\n";
        printf("\n\nafter plan\n ");
        for(int j=0; j<10; j++) {
          union DoubleBitManipulation n;
          n.value = x[j];
          printf(" %016lx", n.bits);
        }
        printf("\n");
        for(int j=0; j<10; j++) std::cout << "     " << x[j];
        printf("\n\n");
        Wav_quant::reduce_quantization(x, blockSize, quantFrac, quantMant); // each double only occupies 10 bits now
        // Wav_quant::inverse_reduce_quantization(x, blockSize, 7, 10);
        printf("after quantization\n ");
        for(int j=0; j<10; j++) {
          union DoubleBitManipulation n;
          n.value = x[j];
          printf(" %016lx", n.bits);
        }
        printf("\n");
        for(int j=0; j<10; j++) std::cout << "     " << x[j];
        printf("\n");
        union DoubleBitManipulation n;
        for(int j=0; j<blockSize*dctFrac; j++) {
          n.value = x[j];
          outfile.writeBits64(n.bits, quantFrac+quantMant+1);
        }

        Wav_quant::inverse_reduce_quantization(x, blockSize, quantFrac, quantMant);
        printf("\n\nafter inverse reduce quantization\n ");
        for(int j=0; j<10; j++) {
          union DoubleBitManipulation n;
          n.value = x[j];
          printf(" %016lx", n.bits);
        }
        printf("\n");
        for(int j=0; j<10; j++) std::cout << "     " << x[j];
        printf("\n");
        fftw_execute(plan_decompress);
        for(int j=0; j<blockSize; j++) samples[j*2+c] = x[j];




        // for(int j=0; j<blockSize; j++) {
        //   if(x[j]>max) max = x[j];
        //   if(x[j]<min) min = x[j];
        // }
        // return 0;
      }
      // outfile2.writef(samples, blockSize);
    }
    // std::cout << "min: " << min << " max: " << max << "\n";
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
    while( loops++ < 1000 ) {
      for(int c=0; c<nChannels; c++) {
        for(int j=0; j<blockSize; j++) {
          if( j<blockSize*dctFrac ) {
            infile.readBits64(&n.bits, quantFrac+quantMant+1);
            x[j] = n.value;
          } else {
            x[j] = 0;
          }
        }
        // Wav_quant::inverse_reduce_quantization(x, blockSize, 5, 4);
        // for(int j=0; j<blockSize; j++) std::cout << std::hex << (uint64_t) x[j] << " ";
        Wav_quant::inverse_reduce_quantization(x, blockSize, quantFrac, quantMant);
        printf("\n\nbefore input inverse quantization\n ");
        for(int j=0; j<10; j++) {
          union DoubleBitManipulation n;
          n.value = x[j];
          printf(" %016lx", n.bits);
        }
        printf("\n");
        for(int j=0; j<10; j++) std::cout << "     " << x[j];
        printf("\n");
        std::cout << "\n\n\n";

        fftw_execute(plan_decompress);
        
        for(int j=0; j<blockSize; j++) samples[j*2+c] = x[j];
        // return 0;
      }
      outfile.writef(samples, blockSize);
    }
	}
}


// int main(int argc, char* argv[]) {
//
//   size_t blockSize { 1024 }; 
//   double dctFrac { 1 };
//   int quantization { 16 };
//
//   size_t i, j, k, c, err;
//
//   // Run with: ./wav_dct2 <compress | decompress> <target_file> <output>
//   if( argc!=4 ) {
//     cerr << "Run with: ./wav_dct2 <compress | decompress> <target_file> <output>";
//     return 1;
//   }
//
//   if( std::string(argv[1])=="compress" ) {
//     SndfileHandle inFile { argv[2] };
//     if( inFile.error() ) {
//       cerr << "Error: invalid input file\n";
//       return 1;
//     }
//
//     BitStream outFile { argv[3], BitStream::w }; 
//
//     vector<double> x(blockSize);
//     vector<double> xfile(blockSize * 2);
//     vector<short> samples(blockSize * 2);
//     fftw_plan plan_d = fftw_plan_r2r_1d(blockSize, x.data(), x.data(), FFTW_REDFT10, FFTW_ESTIMATE);
// 	  // fftw_plan plan_i = fftw_plan_r2r_1d(blockSize, x.data(), x.data(), FFTW_REDFT01, FFTW_ESTIMATE);
//     int readSamples;
//     while( (readSamples = inFile.readf(samples.data(), blockSize))!=0) {
//       for(c=0; c<2; c++) {
//         for(j=0; j<readSamples; j++) x[j] = samples[j*2+c]; // cast of short to double
//         fftw_execute(plan_d);
//         for(j=0; j<readSamples; j++) xfile[j*2+c] = x[j] / (blockSize << 1); // cast double to short
//         
//         // for(j=0; j<readSamples; j++) xfile[j*2+c] = x[j] / (blockSize << 1); // cast of short to double
//         // for(j=0; j<readSamples; j++) xfile[j*2+c] = x[j] / (blockSize << 1); // cast of double to short
//       }
//       
//       // write to file
//       for(j=0; j<readSamples; j++) samples[j] = xfile[j]; // cast of short to double
//       // Wav_quant::reduce_quantization(samples.data(), readSamples*2, quantization);
//       // printf("\n\n---------------------------------------------------------------------------\n");
//       for(j=0; j<readSamples*2 && j<readSamples*2*dctFrac; j++) {
//         // samples[j] = 0x1234;
//         // printf("%08x ", samples[j]);
//         samples[j] = (samples[j] & 0xFF00) >> 8 | (samples[j] & 0x00FF) << 8; 
//
//         outFile.writeBits((unsigned char*) &samples[j], quantization, 0);
//       }
//
//       
//       // outFile.writeBits((unsigned char*) samples.data(), (int) readSamples*2*32*dctFrac, 0);  
//     }
//
//   } else if( std::string(argv[1])=="decompress" ) {
//
//     BitStream inFile { argv[2], BitStream::r };
//     SndfileHandle outFile { argv[3], SFM_WRITE, 65538 , 2, 44100 };
//
//     vector<short> samples(blockSize*2);
//     vector<double> xfile(blockSize*2);
//     vector<double> x(blockSize);
// 	  fftw_plan plan_i = fftw_plan_r2r_1d(blockSize, x.data(), x.data(), FFTW_REDFT01, FFTW_ESTIMATE);
//
//     for(i=0; i<1000; i++) {
//       // inFile.readBits((unsigned char*) xfile.data(), (int) blockSize*2*quantization*dctFrac);
//       // printf("\n\n---------------------------------------------------------------------------\n");
//       for(j=0; j<blockSize*2*dctFrac; j++) {
//         inFile.readBits((unsigned char*) &samples[j], quantization);
//         samples[j] = (samples[j] & 0xFF00) >> 8 | (samples[j] & 0x00FF) << 8; 
//         samples[j] = (samples[j] & 0x00FF) << (16 - quantization)%8 | (samples[j]&0xFF00);
//         xfile[j] = samples[j];
//       }
//
//       for(c=0; c<2; c++) {
//         for(j=0; j<blockSize; j++) x[j] = xfile[j*2+c];
//         fftw_execute(plan_i);
//         for(j=0; j<blockSize; j++) samples[j*2+c] = static_cast<short>(round(x[j])); // cast of short to double
//       }
//
//       outFile.writef(samples.data(), blockSize);
// 	  }
//
//   } else {
//     cerr << "First argument must be compress or decompress"; 
//   }
//
//   return 0;
// }
