#include <iostream>
#include <fstream>
#include "BitStream.hh"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <inputFileName> <outputFileName>" << std::endl;
        return 1;
    }

    std::string inputFileName = argv[1];
    std::string outputFileName = argv[2];

    // Read bitCount from a separate file
    std::ifstream bitCountFile("bitcount.txt");
    unsigned int bitCount;
    bitCountFile >> bitCount;
    bitCountFile.close();

    BitStream input(inputFileName, BitStream::r);
    BitStream output(outputFileName, BitStream::w);

    char byte;
    bool eofReached = false; // EOF
    unsigned int bitsWritten = 0; // Track the number of bits written

    while (input.fs.read(&byte, 1) && bitsWritten < bitCount) {
        if (byte == 0x00) { 
            eofReached = true;
            break;
        }

        for (int i = 7; i >= 0; i--) { // each bit in byte
            if (bitsWritten < bitCount) {
                bool bit = (byte >> i) & 0x01;
                char c = (bit ? '1' : '0');
                output.fs.put(c);
                bitsWritten++;
            } else {
                eofReached = true; // Exit the loop after writing the required number of bits
                break;
            }
        }
    }

    return 0;
}
