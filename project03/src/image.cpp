#include "include/BitStream.hh"
#include "include/Golomb.hh"
#include <cstdint>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "include/matplotlibcpp.h"

namespace plt = matplotlibcpp;

uint8_t getgray(cv::Vec3b px) {
    return (px[0]+px[1]+px[2])/3;
}

int main(int argc, char** argv) 
{ 
    if (argc != 4) { 
        std::cerr << "Usage: extract_channel <in_image_path> <channel> <out_image_path>\n";
        return -1; 
    } 

    cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR); 
    if (image.empty()) { 
        std::cerr << "No image data\n"; 
        return -1; 
    } 

    int channel = std::stoi(argv[2]);
    if (channel < 0 || channel >= image.channels()) {
        std::cerr << "Invalid channel index\n";
        return -1;
    }


    cv::Mat outimg(image.size(), CV_8UC1); 

    std::vector<int8_t> img[image.rows * image.cols];
    std::vector<int64_t> hist(255); for(int i=0; i<255; i++) hist[i] = 0;
    std::vector<uint8_t> errors(image.rows * image.cols);

    int y, x;
    int8_t last = 0;
    uint8_t a,b,c;
    for (y = 0; y < image.rows; y++) {
        for (x = 0; x < image.cols; x++) {
            a = x-1<0 ? 0 : getgray(image.at<cv::Vec3b>(y, x-1));
            b = y-1<0 ? 0 : getgray(image.at<cv::Vec3b>(y-1, x));
            c = x-1<0 || y-1<0 ? 0 : getgray(image.at<cv::Vec3b>(y-1, x-1));

            int8_t px = getgray(image.at<cv::Vec3b>(y,x));
            
            // uint8_t predictor = a;
            // uint8_t predictor = b;
            // uint8_t predictor = c;
            // uint8_t predictor = a+b-c;
            // uint8_t predictor = a+(b-c)/2;
            // uint8_t predictor = b+(a-c)/2;
            uint8_t predictor = (a+b)/2;
            int8_t error = predictor-px;

            outimg.at<uint8_t>(y, x) = error>=0 ? error*2 : -error*2-1;
            hist[error>=0 ? error*2 : -error*2-1] += 1;
            errors[y*image.rows+x] = error;
        }
    }
    int i;
    double geometric = 0;
    for(i=1; i < 255; i+=1) {
        geometric += i * (double(hist[i])/double(image.rows*image.cols));
    }
    geometric = 1/geometric;

    // printf("p: %f\n", geometric);
    int m = (uint) -ceil(log2(1-geometric)/log2(-geometric));
    printf("CALCULATED M: %d\n", m);

    BitStream bs = BitStream("compressed", BitStream::w);
    Golomb g = Golomb(32, &bs);

    for(i=0; i<image.rows*image.cols; i++) g.encode(errors[i]);

    // plt::plot(xx, hist);
    // plt::show();
    // Sample data
    // std::vector<int8_t> xx = {1, 2, 3, 4, 5};
    std::vector<int8_t> xx(255); for(int i=0; i<255; i++) xx[i]=i-128;

    // Plot the data
    plt::plot(xx, hist);
    plt::show();

    cv::imshow("Extracted Gray", outimg);
    cv::waitKey(0);

    return 0; 
}
