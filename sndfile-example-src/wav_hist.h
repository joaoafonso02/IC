#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>
#include <fstream>
#include <math.h>

class WAVHist {
private:
	std::vector<std::map<short, size_t>> counts;
	std::map<short, size_t> midChannelCounts;
  	std::map<short, size_t> sideChannelCounts;
	std::map<short, size_t> compactChannelCounts;

public:
  WAVHist(const SndfileHandle& sfh) {
		counts.resize(sfh.channels());
	}

	void update(const std::vector<short>& samples) {
		size_t n { };
		for(auto s : samples)
			counts[n++ % counts.size()][s]++;
	}

	void dump(const size_t channel) const {
		for(auto [value, counter] : counts[channel]) {
			std::cout << value << '\t' << counter;
		}
	}

	void dumpAll() {
		std::cout << "DUMPALL" << "\n"; 
		// write all channels to files
		for(size_t i=0; i<counts.size(); i++) {
			std::ofstream channel;
			channel.open("channel"+std::to_string(i)+".txt");
			for(auto [value, counter] : counts[i]) {
				channel << counter << "\t" << value << "\n"; 
			}
			channel.close();
		}

		// write COMPACT channel
		std::ofstream compact;
		compact.open("compact.txt");
		for(auto [value, counter] : compactChannelCounts) {
			compact << value*(32768*2/32) << "\t" << counter << "\n"; 
		}
		compact.close();

		// write MID channel
		std::ofstream mid;
		mid.open("mid.txt");
		for(auto [value, counter] : midChannelCounts) {
			mid << counter << "\t" << value << "\n"; 
		}
		mid.close();

		// write SIDE channel
		std::ofstream side;
		side.open("side.txt");
		for(auto [value, counter] : sideChannelCounts) {
			side << counter << "\t" << value << "\n"; 
		}
		side.close();
	}

	void update2(const std::vector<short>& samples) {
		size_t n = 0;
		for(size_t i=0; i<samples.size(); i++) {
			counts[i%2][samples[i]]++; 
			compactChannelCounts[(int) (samples[i]/(65536/32))]++;
			
			if(counts.size() == 2 && i%2==0) {
				midChannelCounts[(samples[i] + samples[i+1]) / 2]++;
				sideChannelCounts[(samples[i] - samples[i+1]) / 2]++;
			}
		}
	}

};

#endif

