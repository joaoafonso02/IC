#include <iostream>
#include <sndfile.hh>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << "<infile.wav> <outfile.wav> <new bite sample size>\n";
    return 1;
  }

  SndfileHandle inwav { argv[1] };
  if( (inwav.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV ) {
    std::cerr << "Error: infile not in WAV format\n";
    return 1;
  }

  SndfileHandle outwav { argv[2], SFM_WRITE, inwav.format(), inwav.channels(), inwav.samplerate() };
  if(outwav.error()) {
    std::cerr << "Error: invalid output file\n";
    return 1;
  } 

  short shiftSize = std::stoi(argv[3]);
  std::cout << "ShiftSize " << shiftSize << "\n";
  size_t nFrames;
  std::vector<short> samples(65536 * inwav.channels());
    std::cout << "asdf";
  while( (nFrames = inwav.readf(samples.data(), 65536)) ) {
    std::cout << "Bah";
    for(size_t i = 0; i<nFrames * inwav.channels(); i++) {
      samples[i] = samples[i] & (0xF000) ;
    }
    outwav.writef(samples.data(), nFrames);
  }

  return 0;

}
