#include "include/BitStream.hh"
#include "include/Golomb.hh"
#include <cstdint>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctime>
#include "include/matplotlibcpp.h"

namespace plt = matplotlibcpp;

#define N_PREDICTORS 3

struct Image_Encoded {
    std::vector<std::vector<int16_t>> err;
    uint32_t m;
    uint32_t predictor_t;
};

int calculate_m(std::vector<uint64_t> hist, uint32_t sampleSize) {
    double mean = 1;
    double b = 0;
    for(uint i=1; i<hist.size(); i++) {
        b = double(hist[i])/sampleSize;
        mean += (b / (pow(b, .1/i) - 1)) / (hist.size()-1);
    }
    double m = ceil(-1 / log(mean));
    return m<1 ? 1 : m;
}

int code_gray_image(struct Image_Encoded *out, cv::Mat *in) {
    uint rows = in->rows;
    uint cols = in->cols;
    out->m = 1;
    out->predictor_t = 0;

    // calculate histograms
    std::vector<std::vector<uint64_t>> hist(N_PREDICTORS, std::vector<uint64_t>(512, 0)); 
    std::vector<std::vector<int16_t>> errors(N_PREDICTORS, std::vector<int16_t>(rows*cols)); 
    // std::vector<int16_t> xx(512); for(uint i=0; i<xx.size(); i++) xx[i]=i; // used for plotting

    uint y, x, i;
    uint8_t a,b,c;
    uint8_t predictor[7];
    int16_t error;
    for (y = 0; y < rows; y++) {
        for (x = 0; x < cols; x++) {
            a = x==0 ? 0 : in->at<uint8_t>(y, x-1);
            b = y==0 ? 0 : in->at<uint8_t>(y-1, x);
            c = x==0 || y==0 ? 0 : in->at<uint8_t>(y-1, x-1);

            uint8_t px = in->at<uint8_t>(y,x);

            predictor[0] = a;
            predictor[1] = b;
            predictor[2] = c;
            predictor[3] = (uint8_t) int(a)+int(b)-int(c);
            predictor[4] = (uint8_t) int(a)+(int(b)-int(c))/2;
            predictor[5] = (uint8_t) int(b)+(int(a)-int(c))/2;
            predictor[6] = (uint8_t) (int(a)+int(b))/2;

            for(i=0; i<N_PREDICTORS; i++) {
                error = int16_t(px) - int16_t(predictor[i]); // error -> [-255, 255]
                errors[i][y*cols + x] = error;
                hist[i][error>=0 ? error*2 : -error*2-1] += 1; // error becomes -> [0, 512]
            }
        }
    }

    // find m and i_predictor based on histograms
    out->m = (1LL << 32) - 1;
    uint32_t tm;
    for(i=0; i<N_PREDICTORS; i++) {
        tm = calculate_m(hist[i], rows*cols);
        printf("predictor %d: %d\n", i, tm);
        if( tm<out->m ) {
            out->m = tm;
            out->predictor_t = i;
        }
    }

    // encode image
    for (y = 0; y < rows; y++) {
        for (x = 0; x < cols; x++) {
            int error = errors[out->predictor_t][y*cols + x];
            out->err[y][x] = error;
        }
    }

    return 0;
}

int main(int argc, char** argv) 
{
    int ret;
    uint i, j;
    if (argc != 3) { 
        std::cerr << "Usage: extract_channel <in_ppm_path> <out_coded_ppm_path>\n";
        return -1; 
    } 

    cv::Mat original_image = cv::imread(argv[1]);
    if (original_image.empty()) {
        std::cerr << "Error: Could not open or read the image." << std::endl;
        return -1;
    }

    BitStream bs = BitStream(argv[2], BitStream::w);

    uint64_t height = original_image.rows;
    uint64_t width = original_image.cols;
    bs.mwriteNBits(height, 16);
    bs.mwriteNBits(width, 16);
    printf("height %ld\n", height);
    printf("width %ld\n", width);

    struct Image_Encoded data;
    data.err = std::vector<std::vector<int16_t>>(height, std::vector<int16_t>(width, 0));

    cv::Mat image(height, width, CV_8UC1);

    // Convert BGR to Grayscale
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            uint8_t r = original_image.at<cv::Vec3b>(i, j)[2]; 
            uint8_t g = original_image.at<cv::Vec3b>(i, j)[1]; 
            uint8_t b = original_image.at<cv::Vec3b>(i, j)[0]; 

            image.at<uint8_t>(i, j) = 0.299 * r + 0.587 * g + 0.114 * b; 
        }
    }


    ret = code_gray_image(&data, &image);

    // Debug: See if decoded is correct (only using predictor_t 0)
    // printf("m: %d\n", data.m);
    // printf("predictor_t: %d\n", data.predictor_t);
    // for(i=0; i<height; i++) {
    //     for(j=0; j<width; j++) {
    //         uint8_t a = i-1<0 ? 0 : frame.at<uint8_t>(i, j-1);
    //         frame.at<uint8_t>(i, j) = uint8_t(int(a) + data.err[i][j]);
    //     }
    // }
    // cv::imshow("frame", frame);
    // cv::waitKey(1);

    Golomb g = Golomb(data.m, &bs);

    // write to outfile
    bs.mwriteNBits(uint64_t(data.m), 32);
    bs.mwriteNBits(uint64_t(data.predictor_t), 8);
    for (uint y = 0; y < height; y++) {
        for (uint x = 0; x < width; x++) { 
            g.encode(data.err[y][x]);
        }
    }

    return 0; 
}
