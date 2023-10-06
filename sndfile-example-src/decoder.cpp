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

    char byte;
    bool eofReached = false; // EOF

    while (input.fs.read(&byte, 1)) { // while !EOF
        if (byte == 0x00) { 
            eofReached = true;
            break;
        }

        for (int i = 7; i >= 0; i--) { // each bit in byte
            bool bit = (byte >> i) & 0x01; 
            char c = (bit ? '1' : '0');
            output.fs.put(c);
        }
    }

    return 0;
}
