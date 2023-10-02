#include <iostream>
#include <fstream>

class BitStream {
private:
  std::fstream fs;
  std::streambuf* filebuf;

  unsigned char buffer;
  short bufferpos;

public:
  enum Flag {w, r};

  // Constructor
  BitStream(std::string fileName, Flag flag) {
    if( flag==r ) fs.open(fileName, std::ios::in | std::ios::binary);
    if( flag==w ) fs.open(fileName, std::ios::out | std::ios::binary);
    filebuf = fs.rdbuf();

    buffer = 0;
    bufferpos = 0;
  }

  void writeBit(bool bit) {
    unsigned char cbit = (unsigned char) bit;
    cbit <<= 7-bufferpos;
    buffer |= cbit;
    bufferpos += 1;

    printf("buffer: %x\n", buffer);
    if(bufferpos==8) {
      filebuf->sputc(buffer);
      buffer = 0;
      bufferpos = 0;
    }
  }

  void writeBitFlush() {
    filebuf->sputc(buffer);
    buffer = 0;
    bufferpos = 0;
  }

  void writeBits(unsigned char* data, unsigned int n, unsigned int offset) {
    for(int i=0; i<n; i++) {
      bool bit = (data[i/8] >> (7-i%8)) & 0x01;
      printf("%b [%d, %d]: %d\n", data[i/8], i/8, i%8, bit);
      writeBit(bit);
    }
    writeBitFlush();
  }

  // Deconstructor
  ~BitStream() {
    fs.close();
  }
};
