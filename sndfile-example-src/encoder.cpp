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

    BitStream input(inputFileName, BitStream::r);
    BitStream output(outputFileName, BitStream::w);

    char c;
    while (input.fs.get(c)) { // Read bits one by one.
        bool bit = (c == '1'); // checks if bit - 0 or 1
        output.writeBit(bit); // (true if '1', false if '0')
    }

    output.writeBitFlush();
    return 0;
}
