#include <iostream>
#include <fstream>

class BitStream {
private:
  std::ifstream is;
  std::ofstream os;
  std::streambuf* filebuf;
  long fileSize;
public:
  // Constructor
  BitStream(std::string fileName) {
    is.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
    filebuf = is.rdbuf();

    // get file size
    is.seekg(0, is.end);
    fileSize = is.tellg();
    printf("FILE SIZE %ld\n", fileSize);
    is.seekg(is.beg);
  }

  char* readBits(unsigned int n, unsigned int offset) {
    if( n>64 ) throw std::invalid_argument("[BitStream->readBits] n must be less than 64");
    if( offset+n>fileSize*8 ) throw std::invalid_argument("[BitStream->readBits] offset greater than file, this file max size is " + std::to_string(fileSize));
    if( offset%8!=0 ) throw std::invalid_argument("[BitStream->readBits] offset not divisible by 8 not implemented");

    // TODO: currently only accepts offset that are divisible by 8, to change that the read needs to start in the middle of a byte

    is.seekg(offset, is.beg);

    // read bytes
    unsigned int byteCount = n/8 + (n%8!=0); 
    char* bits = (char*) malloc(byteCount);
    filebuf->sgetn(bits, byteCount);

    // remove excess bits from last byte
    if(n%8!=0) {
      unsigned char mask = -1 << (8-n%8);
      bits[byteCount-1] = bits[byteCount-1] & mask;
    }

    return bits;
  }


  char* writeBits(const char* data, unsigned int n, unsigned int offset) {
    if (n > 64) throw std::invalid_argument("[BitStream->writeBits] n must be less than 64");
    if (offset + n > fileSize * 8) throw std::invalid_argument("[BitStream->writeBits] offset greater than file, this file max size is " + std::to_string(fileSize));
    if (offset % 8 != 0) throw std::invalid_argument("[BitStream->writeBits] offset not divisible by 8 not implemented");

    os.seekp(offset / 8, os.beg);

    // write bits
    unsigned int byteCount = n / 8 + (n % 8 != 0);
    os.write(data, byteCount);

    return (char*) data;

  }


  long getFileSize() { return fileSize; }

  // Deconstructor
  ~BitStream() {
    is.close();
  }
};
