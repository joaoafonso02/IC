#include <iostream>
#include <string>
#include "bitstream.h"

using namespace std;

int main(int argc, char *argv[]) {

    //string op = argv[2];
    BitStream bitFile {argv[1], argv[2]};


    bitFile.read_bytes(argv[1]);

    return 0;
}