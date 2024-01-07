#include "include/BitStream.hh"
#include "include/Golomb.hh"
#include <cstdint>
#include <cstdio>
#include <opencv2/core/matx.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
// #include "include/matplotlibcpp.h"
//
// namespace plt = matplotlibcpp;

#define N_PREDICTORS 3

int main(int argc, char** argv) 
{
    uint i, j;

    if (argc != 3) { 
        std::cerr << "Usage: extract_channel <in_y4m_path> <out_coded_y4m_path>\n";
        return -1; 
    } 

    BitStream bs = BitStream(argv[1], BitStream::r);
    BitStream out = BitStream(argv[2], BitStream::w);

    uint64_t m, predictor_t, width, height;
    bs.mreadNBits(&height, 16);
    bs.mreadNBits(&width, 16);

    cv::Mat image(height, width, CV_8UC1); 

    bs.mreadNBits(&m, 32);
    bs.mreadNBits(&predictor_t, 8);
    printf("\nHeader Values:\n");
    printf("M: %ld\n", m);
    printf("I_Predictor: %ld\n", predictor_t);

    Golomb g = Golomb(m, &bs);


    uint y, x;
    int64_t error;
    uint8_t a, b, c;
    uint8_t predictor[7];
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            a = x==0 ? 0 : image.at<uint8_t>(y, x-1);
            b = y==0 ? 0 : image.at<uint8_t>(y-1, x);
            c = x==0 || y==0 ? 0 : image.at<uint8_t>(y-1, x-1);

            uint8_t predict;
            switch(predictor_t) {
                case 0: predict = a; break;
                case 1: predict = b; break;
                case 2: predict = c; break;
                case 3: predict = (uint8_t) int(a)+int(b)-int(c); break;
                case 4: predict = a+(b-c)/2; break;
                case 5: predict = b+(a-c)/2; break;
                case 6: predict = (a+b)/2; break;
                default: predict = 0; break;
            }

            g.decode(&error);

            uint8_t px = uint8_t(int(predict) + error);
            image.at<uint8_t>(y, x) = px;
        }
    }

    // Convert Gray to BGR
    cv::Mat bgrImage(height, width, CV_8UC3);
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            uint8_t px = image.at<uint8_t>(y,x);
            bgrImage.at<cv::Vec3b>(y,x) = cv::Vec3b(px,px,px); 
        }
    }

    cv::imwrite(argv[2], bgrImage);
        // cv::imshow("Extracted Gray", image);
        // cv::waitKey(0);

        // plt::plot(xx, hist);
        // plt::show();
        // Sample data
        // std::vector<int8_t> xx = {1, 2, 3, 4, 5};
        // std::vector<int8_t> xx(255); for(int i=0; i<255; i++) xx[i]=i-128;

        // Plot the data
        // plt::plot(xx, hist);
        // plt::show();
        //
        // cv::imshow("Extracted Gray", image);
        // cv::waitKey(0);

    return 0; 
}
