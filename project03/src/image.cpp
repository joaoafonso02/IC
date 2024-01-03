#include "include/BitStream.hh"
#include "include/Golomb.hh"
#include <cstdint>
#include <opencv2/opencv.hpp>
#include <iostream>
// #include "include/matplotlibcpp.h"
//
// namespace plt = matplotlibcpp;

#define N_PREDICTORS 7

uint8_t getgray(cv::Vec3b px) {
    return (px[0]+px[1]+px[2])/3;
}

int calculate_m(std::vector<uint64_t> hist, uint32_t sampleSize) {
    double mean = 1;
    double b = 0;
    for(uint i=1; i<hist.size(); i++) {
        b = double(hist[i])/sampleSize;
        mean += (b / (pow(b, 1.0/i) - 1)) / (hist.size()-1);
    }
    double m = ceil(-1 / log(mean));
    return m<1 ? 1 : m;
}

int main(int argc, char** argv) 
{
    uint i, j;
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


    // cv::Mat outimg(image.size(), CV_8UC1); 

    std::vector<int8_t> img[image.rows * image.cols];
    std::vector<std::vector<uint64_t>> hist(N_PREDICTORS, std::vector<uint64_t>(256, 0)); 
    std::vector<std::vector<uint8_t>> errors(N_PREDICTORS, std::vector<uint8_t>(image.rows*image.cols)); 

    int y, x;
    int8_t last = 0, error;
    uint8_t a,b,c;
    uint8_t predictor[7];
    for (y = 0; y < image.rows; y++) {
        for (x = 0; x < image.cols; x++) {
            a = x-1<0 ? 0 : getgray(image.at<cv::Vec3b>(y, x-1));
            b = y-1<0 ? 0 : getgray(image.at<cv::Vec3b>(y-1, x));
            c = x-1<0 || y-1<0 ? 0 : getgray(image.at<cv::Vec3b>(y-1, x-1));

            int8_t px = getgray(image.at<cv::Vec3b>(y,x));
            
            predictor[0] = a;
            predictor[1] = b;
            predictor[2] = c;
            predictor[3] = a+b-c;
            predictor[4] = a+(b-c)/2;
            predictor[5] = b+(a-c)/2;
            predictor[6] = (a+b)/2;

            for(i=0; i<N_PREDICTORS; i++) {
                error = predictor[i]-px;
                hist[i][error>=0 ? error*2 : -error*2-1] += 1;
                errors[i][y*image.rows+x] = error;
            }
        }
    }

    uint32_t tm, m = (1LL << 32) - 1, i_predictor;
    for(i=0; i<N_PREDICTORS; i++) {
        tm = calculate_m(hist[i], image.rows*image.cols);
        printf("M %d: %d\n", i, tm);
        if( tm<m ) {
            m = tm;
            i_predictor = i;
        }
    }
    printf("CALCULATED M with predictor %d: %d\n", i_predictor, m);

    BitStream bs = BitStream("compressed", BitStream::w);
    Golomb g = Golomb(m, &bs);

    for(i=0; i<uint32_t(image.rows*image.cols); i++) g.encode(errors[i_predictor][i]);

    // plt::plot(xx, hist);
    // plt::show();
    // Sample data
    // std::vector<int8_t> xx = {1, 2, 3, 4, 5};
    // std::vector<int8_t> xx(255); for(int i=0; i<255; i++) xx[i]=i-128;

    // Plot the data
    // plt::plot(xx, hist);
    // plt::show();
    //
    // cv::imshow("Extracted Gray", outimg);
    // cv::waitKey(0);

    return 0; 
}
