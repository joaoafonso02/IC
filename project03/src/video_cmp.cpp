#include "include/BitStream.hh"
#include "include/Golomb.hh"
#include <cstdint>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath> // for std::round
// #include "include/matplotlibcpp.h"

// namespace plt = matplotlibcpp;

#define N_PREDICTORS 3

struct Image_Encoded {
    std::vector<std::vector<int16_t>> err;
    uint32_t m;
    uint32_t predictor_t;
    uint32_t quantization_factor;
};

int calculate_m(std::vector<uint64_t> hist, uint32_t sampleSize) {
    double mean = 1;
    double b = 0;
    for (uint i = 1; i < hist.size(); i++) {
        b = double(hist[i]) / sampleSize;
        mean += (b / (pow(b, .1 / i) - 1)) / (hist.size() - 1);
    }
    double m = ceil(-1 / log(mean));
    return m < 1 ? 1 : m;
}

int code_gray_image(struct Image_Encoded *out, cv::Mat *in, int quantization_factor) {
    uint rows = in->rows;
    uint cols = in->cols;
    out->m = 1;
    out->predictor_t = 0;
    out->quantization_factor = quantization_factor;
    out->err.resize(rows, std::vector<int16_t>(cols, 0));

    // calculate histograms
    std::vector<std::vector<uint64_t>> hist(N_PREDICTORS, std::vector<uint64_t>(512, 0));
    std::vector<std::vector<int16_t>> errors(N_PREDICTORS, std::vector<int16_t>(rows * cols));
    
    uint y, x, i;
    uint8_t a, b, c;
    uint8_t predictor[7];
    int16_t error;
    for (y = 0; y < rows; y++) {
        for (x = 0; x < cols; x++) {
            a = x == 0 ? 0 : in->at<uint8_t>(y, x - 1);
            b = y == 0 ? 0 : in->at<uint8_t>(y - 1, x);
            c = x == 0 || y == 0 ? 0 : in->at<uint8_t>(y - 1, x - 1);

            uint8_t px = in->at<uint8_t>(y, x);

            predictor[0] = a;
            predictor[1] = b;
            predictor[2] = c;
            predictor[3] = (uint8_t)int(a) + int(b) - int(c);
            predictor[4] = (uint8_t)int(a) + (int(b) - int(c)) / 2;
            predictor[5] = (uint8_t)int(b) + (int(a) - int(c)) / 2;
            predictor[6] = (uint8_t)(int(a) + int(b)) / 2;

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
    for (i = 0; i < N_PREDICTORS; i++) {
        tm = calculate_m(hist[i], rows * cols);
        printf("predictor %d: %d\n", i, tm);
        if (tm < out->m) {
            out->m = tm;
            out->predictor_t = i;
        }
    }

    // encode image
    for (y = 0; y < rows; y++) {
        for (x = 0; x < cols; x++) {
            int error = errors[out->predictor_t][y * cols + x];
            out->err[y][x] = error / quantization_factor;  // Quantization
        }
    }

    return 0;
}


int main(int argc, char **argv) {
    int ret;
    uint i, j;
    if (argc != 4) {
        std::cerr << "Usage: extract_channel <in_y4m_path> <out_coded_y4m_path> <quantization_factor>\n";
        return -1;
    }

    cv::VideoCapture cap(argv[1]);
    printf("%d\n", static_cast<int>(cap.get(cv::CAP_PROP_FOURCC)));
    if (!cap.isOpened()) {
        std::cerr << "No video data\n";
        return -1;
    }

    BitStream bs = BitStream(argv[2], BitStream::w);
    int quantization_factor = std::stoi(argv[3]);

    uint64_t height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    uint64_t width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    uint64_t frameC = cap.get(cv::CAP_PROP_FRAME_COUNT);
    bs.mwriteNBits(height, 16);
    bs.mwriteNBits(width, 16);
    bs.mwriteNBits(frameC, 16);
    printf("height %ld\n", height);
    printf("width %ld\n", width);

    struct Image_Encoded data;
    data.err = std::vector<std::vector<int16_t>>(height, std::vector<int16_t>(width, 0));

    int frameCount = 0;
    cv::Mat original_frame, frame(height, width, CV_8UC1);
    while (cap.read(original_frame)) {
        if (original_frame.empty()) {
            break;
        }
        printf("\nFRAME: %d\n", frameCount++);

        // Convert YUV to Grayscale
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                frame.at<uint8_t>(i, j) = original_frame.at<cv::Vec3b>(i, j)[2];
            }
        }

        ret = code_gray_image(&data, &frame, quantization_factor);

        Golomb g = Golomb(data.m, &bs);

        // write to outfile
        bs.mwriteNBits(uint64_t(data.m), 32);
        bs.mwriteNBits(uint64_t(data.predictor_t), 8);
        bs.mwriteNBits(uint64_t(data.quantization_factor), 32);
        for (uint y = 0; y < height; y++) {
            for (uint x = 0; x < width; x++) {
                g.encode(data.err[y][x]);
            }
        }

        // Reconstruct the frame
        for (uint y = 0; y < height; y++) {
            for (uint x = 0; x < width; x++) {
                int error = data.err[y][x];
                frame.at<uint8_t>(y, x) = std::clamp(int(frame.at<uint8_t>(y, x)) + error, 0, 255);
            }
        }

        // Calculate PSNR
        double psnr = 0.0;
        double mse = 0.0;
        for (uint y = 0; y < height; y++) {
            for (uint x = 0; x < width; x++) {
                mse += std::pow(original_frame.at<cv::Vec3b>(y, x)[2] - frame.at<uint8_t>(y, x), 2);
            }
        }
        mse /= (height * width);
        if (mse > 0.0) {
            psnr = 10.0 * std::log10((255.0 * 255.0) / mse);
        }
        std::cout << "PSNR: " << psnr << " dB" << std::endl;
    }

    return 0;
}