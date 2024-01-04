#include "include/BitStream.hh"
#include "include/Golomb.hh"
#include <cstdint>
#include <opencv2/opencv.hpp>
#include <iostream>
// #include "include/matplotlibcpp.h"
//
// namespace plt = matplotlibcpp;

#define N_PREDICTORS 7

int main(int argc, char** argv) 
{
    uint i, j;

    BitStream bs = BitStream("compressed", BitStream::r);
    uint64_t m, i_predictor, width, height;
    bs.readNBits(&m, 32); m >>= (64-32);
    bs.readNBits(&i_predictor, 4); i_predictor >>= (64-4);
    bs.readNBits(&height, 16); height >>= (64-16);
    bs.readNBits(&width, 16); width >>= (64-16);
    printf("\nHeader Values:\n");
    printf("M: %ld\n", m);
    printf("I_Predictor: %ld\n", i_predictor);
    printf("Height: %ld\n", height);
    printf("Width: %ld\n", width);
    
    Golomb g = Golomb(m, &bs);

    cv::Mat image(height, width, CV_8UC1); 

    int y, x;
    int8_t last = 0, error;
    uint8_t a=0,b=0,c=0;
    uint8_t predictor[7];
    for (y = 0; y < image.rows; y++) {
        for (x = 0; x < image.cols; x++) {
            a = x-1<0 ? 0 : image.at<uint8_t>(y, x-1);
            b = y-1<0 ? 0 : image.at<uint8_t>(y-1, x);
            c = x-1<0 || y-1<0 ? 0 : image.at<uint8_t>(y-1, x-1);
            
            uint8_t predict;
            switch(i_predictor) {
              case 0: predict = a; break;
              case 1: predict = b; break;
              case 2: predict = c; break;
              case 3: predict = a+b-c; break;
              case 4: predict = a+(b-c)/2; break;
              case 5: predict = b+(a-c)/2; break;
              case 6: predict = (a+b)/2; break;
              default: predict = 0; break;
            }

            int64_t error;
            g.decode(&error);
            image.at<uint8_t>(y, x) = predict + error;
        }
    }

    // plt::plot(xx, hist);
    // plt::show();
    // Sample data
    // std::vector<int8_t> xx = {1, 2, 3, 4, 5};
    // std::vector<int8_t> xx(255); for(int i=0; i<255; i++) xx[i]=i-128;

    // Plot the data
    // plt::plot(xx, hist);
    // plt::show();
    //
    cv::imshow("Extracted Gray", image);
    cv::waitKey(0);

    return 0; 
}
