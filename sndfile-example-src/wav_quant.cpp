#include <iostream>
#include <sndfile.hh>
#include <vector>

#include "Wav_quant.hh"

// void Wav_quant::reduce_quantization(short* samples, int nSamples, int newBits) {
//   short mask = 0xFFFF;
//   mask <<= 16-newBits;
//   for(int i=0; i<nSamples; i++) {
//     samples[i] = samples[i] & mask;
//   }
// }
//
// void Wav_quant::reduce_quantization(std::string inFile, std::string outFile, int newBits) {
//   SndfileHandle infile { inFile };
//   if( (infile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV ) {
//     std::cerr << "Error: infile not in WAV format\n";
//     throw;
//   }
//
//   SndfileHandle outfile { outFile , SFM_WRITE, infile.format(), infile.channels(), infile.samplerate() };
//   if( outfile.error() ) {
//     std::cerr << "Error: infile not in WAV format\n";
//     throw;
//   }
//
//   int blocksize = 65536;
//   short samples[blocksize * infile.channels()];
//   size_t frames;
//   while( (frames = infile.readf(samples, blocksize)) ) {
//     Wav_quant::reduce_quantization(samples, blocksize * infile.channels(), newBits);
//     outfile.writef(samples, blocksize);
//   }
// }

// int main(int argc, char *argv[]) {
//   if (argc != 4) {
//     std::cerr << "Usage: " << argv[0] << "<infile.wav> <outfile.wav> <new bite sample size>\n";
//     return 1;
//   }
//   int newBits = std::stoi(argv[3]);
//   if( newBits<=0 || newBits>16 ) throw std::invalid_argument("newBits must be between [1,16]");
//
//   Wav_quant::reduce_quantization(argv[1], argv[2], newBits);
//
//   return 0;
//
// }
