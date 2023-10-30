#include <iostream>
#include <fstream>
#include "BitStream.hh"

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <inputFileName> <outputFileName> <bitCountFileName>" << std::endl;
        return 1;
    }

    std::string inputFileName = argv[1];
    std::string outputFileName = argv[2];
    std::string bitCountFileName = argv[3];

    BitStream input(inputFileName, BitStream::r);
    BitStream output(outputFileName, BitStream::w);

    char c;
    unsigned int bitCount = 0; // Track the number of bits read

    while (input.fs.get(c)) {
        if (c == '0' || c == '1') {
            bool bit = (c == '1');
            output.writeBit(bit);
            bitCount++;
        }
    }

    std::cout << "Total bits read: " << bitCount << std::endl;

    output.writeBitFlush();
    // Write bitCount to a separate file
    std::ofstream bitCountFile(bitCountFileName);
    bitCountFile << bitCount;
    bitCountFile.close();
    
    return 0;
}
