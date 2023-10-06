#ifndef BITSTREAM
#define BITSTREAM

#include <limits>
#include <string>
#include <iostream>
#include <fstream>
#include <regex>

using namespace std;


class BitStream {
private:
    char* operation;
    char* filepath;


public:
    BitStream(char* filepath, char* op) {
        filepath = filepath;
        operation = op;
    }
    
    void read_bytes(char* filepath) {
        streampos size;
        char * memblock;

        ifstream file(filepath, ios::binary|ios::ate);
        if (file.is_open()) {
            size = file.tellg();
            cout << "size: " << size << "bytes" << endl;
            memblock = new char[size];
            file.seekg(0, ios::beg);
            file.read(memblock, size);
            file.close();
        }
        cout << "data:" << memblock << endl;
    }


    void read_string(char* filepath) {
        ifstream file(filepath );
        string text;
        file >> text;

        if (!isBinary(text)) {
            cerr << "Error: Input file doesnt contain bits";
        } 

        file.close();
    }

    //void write() {}

    bool isBinary(const std::string& str) {
        const regex pattern("^[01]+$");
        return regex_match(str, pattern);
    }
};




#endif