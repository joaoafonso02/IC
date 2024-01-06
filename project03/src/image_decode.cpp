#include "include/BitStream.hh"
#include "include/Golomb.hh"
#include <cstdint>
#include <cstdio>
#include <opencv2/core/matx.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

#define N_PREDICTORS 7

int main(int argc, char** argv) {
    uint i, j;
    BitStream bs = BitStream("file.gol", BitStream::r);
    BitStream out = BitStream("file.y4m", BitStream::w);

    uint64_t m, i_predictor, width, height;
    bs.mreadNBits(&height, 16);
    bs.mreadNBits(&width, 16);

    char header[256];
    std::snprintf(header, sizeof(header), "YUV4MPEG2 W%lu H%lu F30000:1001 Ip A1:1 Cmono", width, height);
    printf("%s\n", header);
    for(i=0; header[i]!='\0'; i++) {
        out.mwriteNBits(header[i], 8);
    }
    out.mwriteNBits(0x0A, 8);
    char str_frame[] = {"FRAME "};
    str_frame[5] = 0x0A;

    cv::Mat image(height, width, CV_8UC1); 

    for(i=0; i<300; i++) {
        bs.mreadNBits(&m, 32);
        bs.mreadNBits(&i_predictor, 4);
        printf("\nHeader Values:\n");
        printf("M: %ld\n", m);
        printf("I_Predictor: %ld\n", i_predictor);

        Golomb g = Golomb(m, &bs);

        for(j=0; str_frame[j]!='\0'; j++) {
            out.mwriteNBits(str_frame[j], 8);
        }

        uint y, x;
        int64_t error;
        uint8_t a, b, c;
        uint8_t predictor[7];
        for (y = 0; y < height; y++) {
            for (x = 0; x < width; x++) {
                a = x-1<0 ? 0 : image.at<uint8_t>(y, x-1);
                b = y-1<0 ? 0 : image.at<uint8_t>(y-1, x);
                c = x-1<0 || y-1<0 ? 0 : image.at<uint8_t>(y-1, x-1);

                int64_t predict;
                switch(i_predictor) {
                    case 0: predict = a; break;
                    case 1: predict = b; break;
                    case 2: predict = int64_t(c); break;
                    case 3: predict = a+b-c; break;
                    case 4: predict = a+(b-c)/2; break;
                    case 5: predict = b+(a-c)/2; break;
                    case 6: predict = (a+b)/2; break;
                    default: predict = 0; break;
                }

                g.decode(&error);

                image.at<uint8_t>(y, x) = predict + error;
                out.mwriteNBits(uint64_t(image.at<uint8_t>(y, x)), 8);
            }
        }
        cv::imshow("Extracted Gray", image);
        cv::waitKey(1);
    }

    return 0; 
}
