#include <iostream>
#include <vector>
#include <cstdlib>
#include <sndfile.hh>
#include "wav_hist.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {
    bool single_echo = false, multi_echo = false, bassBoost = false, reverse = false, vibrato = false, amplitude_modulation = false; // sound effect flags
    double bassGain = 0.0;
    size_t blockSize = 256;

    if (argc < 4) {
        cerr << "Usage: " << argv[0] <<  " <input file> <output file> [-single_echo | -multi_echo | -bass <bass gain> | -reverse | -vibrato | -amplitude_modulation]\n";
        cerr << "\nSound Effects:\n";
        cerr << "  -single_echo             Enable single echo effect\n";
        cerr << "  -multi_echo              Enable multi echo effect\n";
        cerr << "  -bass <bass gain>        Enable bass boost effect with a specified gain\n";
        cerr << "  -reverse                 Enable reverse effect\n";
        cerr << "  -vibrato                 Enable vibrato effect\n";
        cerr << "  -amplitude_modulation    Enable amplitude_modulation effect\n";
        
        return 1;
    }

    SndfileHandle sfhIn { argv[1] }; // input file
    if(sfhIn.error()) {
        cerr << "Error: invalid input file\n";
        return 1;
    }

    if((sfhIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
        cerr << "Error: file is not in WAV format\n";
        return 1;
    }

    if((sfhIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
        cerr << "Error: file is not in PCM_16 format\n";
        return 1;
    }

    string effect = argv[3]; // sound effect option
    if(effect == "-single_echo") {
        single_echo = true;
    } else if (effect == "-multi_echo") {
        multi_echo = true;
    } else if (effect == "-bass") {
        if (argc == 5) { // bassGain
            try {
                bassGain = stod(argv[4]); // Convert the string argument to a double representing bass gain (stod - Convert string to double)
                bassBoost = true; // bassBoost enabled
            } catch (const invalid_argument& e) {
                cerr << "Error: Invalid bass gain argument. Please provide a valid number." << endl;
                return 1;
            }
        } else {
            cerr << "Error: Bass boost requires a bass gain value. Use -b <bass gain>." << endl;
            return 1;
        }
    } else if (effect == "-reverse") {
        reverse = true;
    } else if (effect == "-vibrato") {
        vibrato = true;
    } else if (effect == "-amplitude_modulation"){
        amplitude_modulation = true;
    } else {
        cerr << "Error: Invalid sound effect argument. Use -single_echo for single echo, -multi_echo for multiecho, -bass <bass gain> for bass boost, -reverse for reverse or -vibrato for vibrato." << endl;
        return 1;
    }

    SndfileHandle sfhOut { argv[2], SFM_WRITE, sfhIn.format(), sfhIn.channels(), sfhIn.samplerate() }; // output file
    if(sfhOut.error()) {
        cerr << "Error: invalid output file\n";
        return 1;
    }

    size_t nFrames;
    short sample_output;
    vector<short> samples(FRAMES_BUFFER_SIZE * sfhOut.channels());
    vector<short> samples_output; // output samples
    samples_output.resize(0);
    WAVHist hist { sfhOut }; // handle audio history --> output file

    vector<short> audioData; // vector to store all the audio used for REVERSE

    // Check if the wanted_effect is single_echo or multiple_echo
    float gain = 0.3;
    int delay = 5000;
    float freq = 1.0;

    while ((nFrames = sfhIn.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
        samples.resize(nFrames * sfhIn.channels());
        audioData.insert(audioData.end(), samples.begin(), samples.end()); // Store the samples in the audioData vector

        if (single_echo || multi_echo) {
            for (int i = 0; i < (int)samples.size(); i++) {
                if(i >= delay) {
                    if(single_echo) {
                        sample_output = (samples.at(i) + gain * samples.at(i - delay)) / (1 + gain);
                    } else if (multi_echo) {
                        if (i - 2 * delay >= 0) {
                            sample_output = (samples.at(i) + gain * samples.at(i - delay) + gain * samples.at(i - 2 * delay)) / (1 + gain + gain);
                        } else {
                            sample_output = (samples.at(i) + gain * samples.at(i - delay)) / (1 + gain); // Use single echo for early samples
                        }
                    } else {
                        sample_output = samples.at(i);
                    }

                    samples_output.insert(samples_output.end(), sample_output);
                }
            }  
        } else if(amplitude_modulation) {
            for (int i = 0; i < (int)samples.size(); i++) {
                sample_output = samples[i] * cos(2 * M_PI * (2* freq /sfhIn.samplerate()) * i); // m(t) = Acos(2πft)
                samples_output.insert(samples_output.end(), sample_output);
            }
        } else if(bassBoost) {
            for (int i = 0; i < (int)samples.size(); i++) {
                if (samples[i] < 0) {
                    sample_output = bassGain/1000;
                } else {
                    sample_output = samples[i];
                }
                samples_output.insert(samples_output.end(), sample_output);
               
            }
        } else if (vibrato) { 
            const double vibratoDepth = 0.9, vibratoFrequency = 1.0; 

            for (size_t i = 0; i < samples.size(); i++) {
                // Amount of vibrato 
                double vibratoAmount = vibratoDepth * sin(2.0 * M_PI * vibratoFrequency * i / sfhIn.samplerate());
                // Apply vibrato effect to the sample
                short vibratoSample = static_cast<short>(samples[i] * cos(vibratoAmount));
                samples_output.insert(samples_output.end(), vibratoSample);
            }
        }
    }

    if (reverse) {
        size_t audioDataSize = audioData.size();
        // reverse the sample entirely
        for (size_t i = 0; i < audioData.size() / 2; i++) {
            swap(audioData[i], audioData[audioDataSize - i - 1]);
        }
        samples_output = audioData;
    
    }

    sfhOut.writef(samples_output.data(), samples_output.size() / sfhIn.channels());

}