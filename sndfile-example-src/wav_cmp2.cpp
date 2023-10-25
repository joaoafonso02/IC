#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <cmath>
#include <fstream>
#include "gnuplot-iostream.h" // Include the Gnuplot C++ bindings

#include "Wav_quant.hh"

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading/writing frames

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file 1> \n";
		return 1;
    }

    SndfileHandle file1 { argv[1] };

    if ( (file1.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
        std::cerr << "Error: file1 not in WAV format\n";
        return 1;
    }
    
    size_t nFrames;
    std::vector<short> samples1(FRAMES_BUFFER_SIZE * file1.channels());
    std::vector<short> samples2(FRAMES_BUFFER_SIZE * file1.channels());

    std::vector<double> mse(16);
    std::vector<double> maxAbsoluteError(16);
    std::vector<double> signal(16);
    std::vector<double> noise(16);
    std::vector<double> snr(16);


    while( (nFrames = file1.readf(samples1.data(), FRAMES_BUFFER_SIZE)) ) {
        
        for (size_t i = 0; i < 16; i++) {
            
            for (size_t j = 0; j < samples1.size(); j++) 
                samples2[j] = samples1[j];
            
            Wav_quant::reduce_quantization( (short*)(samples2.data()), FRAMES_BUFFER_SIZE * file1.channels(), 16-i);
            
            for (size_t j = 0; j < samples1.size(); j++) {
                double diff = samples1[j] - samples2[j];
                mse[i] += diff;
                maxAbsoluteError[i] = std::max(maxAbsoluteError[i], std::abs(diff));
                signal[i] += samples1[j]*samples2[j];
                noise[i] += diff*diff;
            }
        }
    }

    std::ofstream mseFile("ex2/mse.txt");  // Open file for MSE data
    std::ofstream maeFile("ex2/max_absolute_error.txt");  // Open file for Max Absolute Error data
    std::ofstream snrFile("ex2/snr.txt");  // Open file for SNR data


    for (size_t i = 0; i < 16; i++) {
        mse[i] /= (file1.frames()*file1.channels());
        snr[i] = 10*log10(signal[i]/noise[i]);

        std::cout << "MSE " << 16-i << ": " << mse[i] << std::endl;
        std::cout << "MAE " << 16-i << ": " << maxAbsoluteError[i] << std::endl;
        std::cout << "SNR " << 16-i << ": " << snr[i] << std::endl;

        mseFile << mse[i] << "\n";  // Write MSE data to file
        maeFile << maxAbsoluteError[i] << "\n";  // Write Max Absolute Error data to file
        snrFile << snr[i] << "\n";  // Write SNR data to file

        printf("\n");
    }

    mseFile.close();  // Close the MSE file
    maeFile.close();  // Close the Max Absolute Error file
    snrFile.close();  // Close the SNR file

    return 0;
}

