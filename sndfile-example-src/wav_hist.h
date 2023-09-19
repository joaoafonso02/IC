#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>

class WAVHist {
  private:
	std::vector<std::map<short, size_t>> counts;
    std::vector<std::map<short, size_t>> midChannelCounts;
    std::vector<std::map<short, size_t>> sideChannelCounts;

  public:
    WAVHist(const SndfileHandle& sfh) {
		counts.resize(sfh.channels());
		midChannelCounts.resize(sfh.channels());
		sideChannelCounts.resize(sfh.channels());
	}

	void update(const std::vector<short>& samples) {
		size_t n { };
		for(auto s : samples)
			counts[n++ % counts.size()][s]++;
	}

	void dump(const size_t channel) const {
		for(auto [value, counter] : counts[channel])
			std::cout << value << '\t' << counter << '\n';
	}

	void updateSamples(const std::vector<short>& samples) {
		size_t n { };
		for(auto s : samples)
			counts[n++ % counts.size()][s]++; 
		if(counts.size() == 2) {
			short mid = (samples[0] + samples[1]) / 2;  // (L + R) / 2, also known as the MID channel)
			short side = (samples[0] - samples[1]) / 2; // (L − R) / 2, also known as the SIDE channel
			midChannelCounts[0][mid]++; 
			sideChannelCounts[0][side]++; 
		}
	}

};

#endif

