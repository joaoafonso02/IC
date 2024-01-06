#include "include/BitStream.hh"
#include "include/Golomb.hh"
#include <cstdint>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctime>
// #include "include/matplotlibcpp.h"
//
// namespace plt = matplotlibcpp;

#define N_PREDICTORS 7

struct Image_Encoded {
    cv::Mat mat;
    int16_t *error;
    uint32_t m;
    uint32_t i_predictor;
};

uint8_t getgray(cv::Vec3b px) {
    // uint16_t sum = uint16_t(px[0])+uint16_t(px[1])+uint16_t(px[2]);
    // return uint8_t(sum/3);
    return uint8_t(px[2]);
}

int calculate_m(std::vector<uint64_t> hist, uint32_t sampleSize) {
    double mean = 1;
    double b = 0;
    for(uint i=1; i<hist.size(); i++) {
        b = double(hist[i])/sampleSize;
        mean += (b / (pow(b, 0.1/i) - 1)) / (hist.size()-1);
    }
    double m = ceil(-1 / log(mean));
    return m<1 ? 1 : m;
}

int img_encode(struct Image_Encoded *out, cv::Mat *in) {
    int rows = in->rows;
    int cols = in->cols;
    out->mat = cv::Mat(in->size(), CV_8UC1);
    // for (int y = 0; y < rows; y++) {
    //     for (int x = 0; x < cols; x++) {
    //         out->mat.at<uint8_t>(y,x) = 0;
    //     }
    // }
    out->m = 1;
    out->i_predictor = 0;

    // calculate histograms
    std::vector<std::vector<uint64_t>> hist(N_PREDICTORS, std::vector<uint64_t>(512, 0)); 
    std::vector<std::vector<int16_t>> errors(N_PREDICTORS, std::vector<int16_t>(rows*cols)); 
    int y, x, i;
    int a,b,c;
    int predictor[7];
    int16_t error;
    for (y = 0; y < rows; y++) {
        for (x = 0; x < cols; x++) {
            a = x-1<0 ? 0 : getgray(in->at<cv::Vec3b>(y, x-1));
            b = y-1<0 ? 0 : getgray(in->at<cv::Vec3b>(y-1, x));
            c = x-1<0 || y-1<0 ? 0 : getgray(in->at<cv::Vec3b>(y-1, x-1));

            int px = getgray(in->at<cv::Vec3b>(y,x));

            // printf("%u %u %u %u\n", a,b,c, px);

            predictor[0] = a;
            predictor[1] = b;
            predictor[2] = c;
            predictor[3] = a+b-c;
            predictor[4] = a+(b-c)/2;
            predictor[5] = b+(a-c)/2;
            predictor[6] = (a+b)/2;

            for(i=0; i<N_PREDICTORS; i++) {
                error = px - int(predictor[i]); // error e [-256, 256]
                hist[i][error>=0 ? error*2 : -error*2-1] += 1;
                errors[i][y*cols + x] = error;
            }
        }
    }

    // find m and i_predictor based on histograms
    out->m = (1LL << 32) - 1;
    uint32_t tm;
    for(i=0; i<N_PREDICTORS; i++) {
        tm = calculate_m(hist[i], rows*cols);
        if( tm<out->m ) {
            out->m = tm;
            out->i_predictor = i;
        }
    }

    // encode image
    for (y = 0; y < rows; y++) {
        for (x = 0; x < cols; x++) {
            int error = errors[out->i_predictor][y*cols + x];
            // printf("E: %d\n", error);
            out->mat.at<uint8_t>(y, x) = error/4+128;
            // out->mat.at<uint8_t>(y,x) = in->at<cv::Vec3b>(y,x)[2];
            out->error[y*cols + x] = error;
        }
    }

    return 0;
}

int main(int argc, char** argv) 
{
    int ret;
    uint i, j;
    clock_t start, end;
    if (argc != 3) { 
        std::cerr << "Usage: extract_channel <in_image_path> <out_image_path>\n";
        return -1; 
    } 

    cv::VideoCapture cap(argv[1]);
    printf("%d\n", static_cast<int>(cap.get(cv::CAP_PROP_FOURCC)));
    if(!cap.isOpened()) {
        std::cerr << "No video data\n";
        return -1;
    }

    BitStream bs = BitStream(argv[2], BitStream::w);
    cv::Mat image;
    struct Image_Encoded data;
    uint64_t height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    uint64_t width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    data.error = (int16_t*) malloc(height * width * sizeof(int16_t));

    bs.mwriteNBits(height, 16);
    bs.mwriteNBits(width, 16);
    printf("height %ld\n", height);
    printf("width %ld\n", width);

    int frameCount = 0;
    while(true) {
        printf("\nFRAME: %d\n", frameCount++);
        // cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR); 
        //
        cap >> image;

        if (image.empty()) { 
            // std::cerr << "No image data\n"; 
            // return -1; 
            break;
        } 

        ret = img_encode(&data, &image);
        printf("m: %d\n", data.m);
        printf("i_predictor: %d\n", data.i_predictor);
        cv::imshow("frame", data.mat);
        cv::waitKey(1);

        Golomb g = Golomb(data.m, &bs);

        // // write to outfile
        bs.mwriteNBits(uint64_t(data.m), 32);
        bs.mwriteNBits(uint64_t(data.i_predictor), 4);
        for (uint y = 0; y < height; y++) {
            for (uint x = 0; x < width; x++) { 
                // printf("%d %d -> %d\n", y, x, data.error[y*width+x]);
                // exit(0);
                g.encode(data.error[y*width + x]);
            }
        }
    }

    free(data.error);
    return 0; 
}
