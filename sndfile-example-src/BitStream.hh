#include <iostream>
#include <fstream>

class BitStream {
private:
  std::fstream fs;
  std::streambuf* filebuf;
  long fileSize;
public:
  // Constructor
  BitStream(std::string fileName) {
    fs.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
    filebuf = fs.rdbuf();

    // get file size
    fs.seekg(0, fs.end);
    fileSize = fs.tellg();
    printf("FILE SIZE %ld\n", fileSize);
    fs.seekg(fs.beg);
  }
  // BitStream(std::string fileName) {
  //   os.open(fileName, std::ios::out | std::ios::binary);

  //   if (!os) {
  //       std::cerr << "Failed to open the file '" << fileName << "' for output" << std::endl;
  //       throw std::runtime_error("Failed to open the file for output");
  //   }

  //   // Initialize the filebuf
  //   filebuf = os.rdbuf();

  //   // Reset fileSize to 0 since you are writing to a new file
  //   fileSize = 0;
  // }


  char* readBits(unsigned int n, unsigned int offset) {
    if( n>64 ) throw std::invalid_argument("[BitStream->readBits] n must be less than 64");
    if( offset+n>fileSize*8 ) throw std::invalid_argument("[BitStream->readBits] offset greater than file, this file max size is " + std::to_string(fileSize));
    if( offset%8!=0 ) throw std::invalid_argument("[BitStream->readBits] offset not divisible by 8 not implemented");

    // TODO: currently only accepts offset that are divisible by 8, to change that the read needs to start in the middle of a byte

    fs.seekg(offset / 8, fs.beg);

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

  void writeBits(const char* data, unsigned int n, unsigned int offset) {
    fs.seekp(offset / 8, fs.beg); // Move the output stream to the correct byte position

    // Write bits
    unsigned int byteCount = n / 8 + (n % 8 != 0); // Calculate the number of bytes to write
    filebuf->sputn(data, byteCount);
     // Write the data to the file

    // Flush the stream to ensure data is written immediately
    fs.flush();
  }


  long getFileSize() { return fileSize; }

  // Deconstructor
  ~BitStream() {
    fs.close();
  }
};
