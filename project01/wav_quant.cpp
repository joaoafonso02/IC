#include <iostream>
#include <sndfile.hh>
#include <vector>

#include "Wav_quant.hh"


int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << "<infile.wav> <outfile.wav> <new bite sample size>\n";
    return 1;
  }
  int newBits = std::stoi(argv[3]);
  if( newBits<=0 || newBits>16 ) throw std::invalid_argument("newBits must be between [1,16]");

  Wav_quant::reduce_quantization(argv[1], argv[2], newBits);

  return 0;

}
