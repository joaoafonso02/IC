#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "Wav_quant.hh"
#include "wav_hist.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {

	if(argc < 2) {
		cerr << "Usage: " << argv[0] << " <input file>\n";
		return 1;
	}

	SndfileHandle sndFile { argv[1] };
	if(sndFile.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
  }

	if((sndFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	if((sndFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}

	// int channel { stoi(argv[argc-1]) };
	// if(channel >= sndFile.channels()) {
	// 	cerr << "Error: invalid channel requested\n";
	// 	return 1;
	// }

	size_t nFrames;
	vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
	WAVHist hist { sndFile };
	while((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames * sndFile.channels());
		Wav_quant::reduce_quantization(samples.data(), samples.size(), 3);
		hist.update(samples);
	}

	// hist.dump(channel);
	hist.filedump("out");
	return 0;
}

