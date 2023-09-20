#include <iostream>
#include <vector>
#include <sndfile.hh>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <original wav> <modified wav>\n";
		return 1;
    }

    // try {

    SndfileHandle originalFile { argv[1] };
    SndfileHandle modifiedFile { argv[2] };

    if (originalFile.format() & SF_FORMAT_TYPEMASK != SF_FORMAT_WAV) {
        std::cert << "Error: file1 not in WAV format\n"
        return 1;
    }

    if (modifiedFile.format() & SF_FORMAT_TYPEMASK != SF_FORMAT_WAV) {
        std::cert << "Error: file2 not in WAV format\n"
        return 1;
    }

    if (originalFile.channels() != modifiedFile.channels()) {
        std::cerr << "Both files must have the same number of channels." << std::endl;
        return 1;
    }


    // } catch {}



    return 0;
}