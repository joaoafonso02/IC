#ifndef Wav_quant_HH
#define Wav_quant_HH

#include <string>

class Wav_quant {
public:
  static void reduce_quantization(short* samples, int nSamples, int newBits);
  static void reduce_quantization(std::string inFile, std::string outFile, int newBits);
};

#endif
