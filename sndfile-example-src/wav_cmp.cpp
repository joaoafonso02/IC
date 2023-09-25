#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <cmath>

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading/writing frames

int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <file 1> <file 2>\n";
		return 1;
    }

    SndfileHandle file1 { argv[1] };
    SndfileHandle file2 { argv[2] };

    if ( (file1.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
        std::cerr << "Error: file1 not in WAV format\n";
        return 1;
    }

    if ( (file2.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
        std::cerr << "Error: file2 not in WAV format\n";
        return 1;
    }

    // check if the files have the same number of channels
    if (file1.channels() != file2.channels()) {
        std::cerr << "Both files must have the same number of channels.\n";
        return 1;
    }

    // check if the files have the same number of frames
    if (file1.frames() != file2.frames()) {
        std::cerr << "Both files must have the same number of frames.\n";
        return 1;
    }

    size_t nFrames;
    std::vector<short> samples1(FRAMES_BUFFER_SIZE * file1.channels());
    std::vector<short> samples2(FRAMES_BUFFER_SIZE * file2.channels());

    double mse = 0.0;
    double maxAbsoluteError = 0.0;
    double signal = 0.0;
    double noise = 0.0;


    while( (nFrames = file1.readf(samples1.data(), FRAMES_BUFFER_SIZE)) ) {
        file2.readf(samples2.data(), FRAMES_BUFFER_SIZE);

        for (size_t i = 0; i < samples1.size(); i++) {
            double diff = samples1[i] - samples2[i];
            mse += diff*diff;
            maxAbsoluteError = std::max(maxAbsoluteError, std::abs(diff));
            signal += samples1[i]*samples1[i];
            noise += diff*diff;
        }
    }

    mse /= (file1.frames() * file1.channels());
    double snr = 10 * log10(signal/noise);

    std::cout << "Mean Squared Error (L2 norm): " << mse << std::endl;
    std::cout << "Maximum Absolute Error (L∞ norm): " << maxAbsoluteError << std::endl;
    std::cout << "Signal-to-Noise Ratio (SNR): " << snr << " dB" << std::endl;

    return 0;
}
