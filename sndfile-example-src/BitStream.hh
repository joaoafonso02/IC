#include <iostream>
#include <fstream>

class BitStream {
private:
  std::streambuf* filebuf;

  unsigned char buffer;
  short bufferpos;

public:
  enum Flag {w, r};
  std::fstream fs;

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
    for(unsigned int i=0; i<n; i++) {
      bool bit = (data[i/8] >> (7-i%8)) & 0x01;
      printf("%b [%d, %d]: %d\n", data[i/8], i/8, i%8, bit);
      writeBit(bit);
    }
    writeBitFlush();
  }

  // Read functions
  bool readBit() {
    if (bufferpos == 0) {
      buffer = filebuf->sbumpc();
      bufferpos = 8;
    }
    bool bit = (buffer >> (bufferpos - 1)) & 0x01;
    bufferpos -= 1;
    putchar(bit + '0');
    return bit;
}

  unsigned char readBits(unsigned int n) {
    unsigned char result = 0;
    for (unsigned int i = 0; i < n; i++) {
      bool bit = readBit();
      result = (result << 1) | (bit ? 0x01 : 0x00);
      
    }
    return result;
  }
 
  // Deconstructor
  ~BitStream() {
    fs.close();
  }
};


 