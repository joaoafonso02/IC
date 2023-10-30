#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>
#include <fstream>
#include <math.h>
#include <stdio.h>

class WAVHist {
private:
	std::vector<std::map<short, size_t>> counts;
	std::map<short, size_t> mono;
	std::map<short, size_t> diff;

public:
	WAVHist(const SndfileHandle& sfh) {
		counts.resize(sfh.channels());
	}

	void update(const std::vector<short>& samples) {
		size_t n { };
		if( counts.size()==1 ) {
			for(auto s : samples) {
				counts[n++ % counts.size()][s]++;
			}
		} else if( counts.size()==2 ) {
			for(int i=0; i<samples.size(); i+=2) {
				short s1 = samples[i];
				short s2 = samples[i+1];
				counts[0][s1]++;
				counts[1][s2]++;
				mono[(s1+s2)/2]++;
				diff[(s1-s2)/2]++;
			}
		}
	}

	void dump(const size_t channel) {
		for(auto [value, counter] : counts[channel])
			std::cout << value << '\t' << counter << '\n';
	}

	void filedump(const std::string outfilename) {
		if( counts.size()==1 ) {
			char outname[80];
			sprintf(outname, "%sch%d.data", outfilename.c_str(), 0);
			std::ofstream outfile { outname };
			for(auto [value, counter] : counts[0]) {
				outfile << value << '\t' << counter << '\n';
			}
			outfile.close();
		} else if( counts.size()==2 ) {
			char outname[80];
			sprintf(outname, "%sch%d.data", outfilename.c_str(), 0);
			std::ofstream outfilech1 { outname };
			for(auto [value, counter] : counts[0]) {
				outfilech1 << value << '\t' << counter << '\n';
			}
			outfilech1.close();

			sprintf(outname, "%sch%d.data", outfilename.c_str(), 1);
			std::ofstream outfilech2 { outname };
			for(auto [value, counter] : counts[1]) {
				outfilech2 << value << '\t' << counter << '\n';
			}
			outfilech2.close();

			sprintf(outname, "%smono.data", outfilename.c_str());
			std::ofstream outfilemono { outname };
			for(auto [value, counter] : mono) {
				outfilemono << value << '\t' << counter << '\n';
			}
			outfilemono.close();

			sprintf(outname, "%sdiff.data", outfilename.c_str());
			std::ofstream outfilediff { outname };
			for(auto [value, counter] : diff) {
				outfilediff << value << '\t' << counter << '\n';
			}
			outfilediff.close();
		}
	}
};

#endif

