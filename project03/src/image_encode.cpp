#include "include/BitStream.hh"
#include "include/Golomb.hh"
#include <cstdint>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctime>

#define N_PREDICTORS 7

struct Image_Encoded {
    cv::Mat mat;
    int16_t *error;
    uint32_t m;
    uint32_t i_predictor;
};

struct FrameData {
    cv::Mat current;
    cv::Mat previous;
};

uint8_t getgray(cv::Vec3b px) {
    return uint8_t(px[2]);
}

int calculate_m(std::vector<uint64_t> hist, uint32_t sampleSize) {
    double mean = 1;
    double b = 0;
    for (uint i = 1; i < hist.size(); i++) {
        b = double(hist[i]) / sampleSize;
        mean += (b / (pow(b, 0.1 / i) - 1)) / (hist.size() - 1);
    }
    double m = ceil(-1 / log(mean));
    return m < 1 ? 1 : m;
}
int img_encode(struct Image_Encoded *out, FrameData *frames, bool isKeyFrame, int blockSize, int searchArea) {
    cv::Mat *in = &frames->current;
    cv::Mat *prev = &frames->previous;
    int rows = in->rows;
    int cols = in->cols;
    out->mat = cv::Mat(in->size(), CV_8UC1);
    out->m = 1;
    out->i_predictor = 0;

    std::vector<std::vector<uint64_t>> hist(N_PREDICTORS, std::vector<uint64_t>(512, 0));
    std::vector<std::vector<int16_t>> errors(N_PREDICTORS, std::vector<int16_t>(rows * cols));
    int y, x, i;

    for (y = 0; y < rows; y += blockSize) {
        for (x = 0; x < cols; x += blockSize) {
            int blockMotionCompensation = 0;

            for (int by = 0; by < blockSize; by++) {
                for (int bx = 0; bx < blockSize; bx++) {
                    int currentX = x + bx;
                    int currentY = y + by;

                    if (currentX < cols && currentY < rows) {
                        int px = getgray(in->at<cv::Vec3b>(currentY, currentX));

                        if (!isKeyFrame) {
                            // Motion compensation for inter-frames (P frames)
                            int motionCompensation = 0;

                            // Consider a search area around the current block
                            for (int sy = -searchArea; sy <= searchArea; sy++) {
                                for (int sx = -searchArea; sx <= searchArea; sx++) {
                                    int searchX = currentX + sx;
                                    int searchY = currentY + sy;

                                    if (searchX >= 0 && searchX < cols && searchY >= 0 && searchY < rows) {
                                        motionCompensation += getgray(prev->at<cv::Vec3b>(searchY, searchX));
                                    }
                                }
                            }

                            blockMotionCompensation += motionCompensation;
                        }

                        int a = currentX - 1 < 0 ? 0 : getgray(in->at<cv::Vec3b>(currentY, currentX - 1));
                        int b = currentY - 1 < 0 ? 0 : getgray(in->at<cv::Vec3b>(currentY - 1, currentX));
                        int c = currentX - 1 < 0 || currentY - 1 < 0 ? 0 : getgray(in->at<cv::Vec3b>(currentY - 1, currentX - 1));

                        int predictor[7];
                        int16_t error;

                        predictor[0] = a;
                        predictor[1] = b;
                        predictor[2] = c;
                        predictor[3] = a + b - c;
                        predictor[4] = a + (b - c) / 2;
                        predictor[5] = b + (a - c) / 2;
                        predictor[6] = (a + b) / 2;

                        for (i = 0; i < N_PREDICTORS; i++) {
                            error = px - int(predictor[i]);
                            hist[i][error >= 0 ? error * 2 : -error * 2 - 1] += 1;
                            errors[i][(currentY / blockSize) * (cols / blockSize) + (currentX / blockSize)] = error;
                        }
                    }
                }
            }

            if (!isKeyFrame) {
                // block motion compensation value
                for (int by = 0; by < blockSize; by++) {
                    for (int bx = 0; bx < blockSize; bx++) {
                        int currentX = x + bx;
                        int currentY = y + by;

                        if (currentX < cols && currentY < rows) {
                            unsigned char &px = in->at<cv::Vec3b>(currentY, currentX)[2];
                            px += blockMotionCompensation;
                        }
                    }
                }
            }
        }
    }

    out->m = (1LL << 32) - 1;
    uint32_t tm;
    for (i = 0; i < N_PREDICTORS; i++) {
        tm = calculate_m(hist[i], rows * cols);
        if (tm < out->m) {
            out->m = tm;
            out->i_predictor = i;
        }
    }

    for (y = 0; y < rows; y++) {
        for (x = 0; x < cols; x++) {
            int error = errors[out->i_predictor][y / blockSize * (cols / blockSize) + x / blockSize];
            out->mat.at<uint8_t>(y, x) = error / 4 + 128;
            out->error[y * cols + x] = error;
        }
    }

    return 0;
}


int main(int argc, char **argv) {
    int ret;
    uint i, j;
    clock_t start, end;
    if (argc != 6) {
        std::cerr << "Usage: <bin> <in_image_path> <out_image_path> <key_frame_period> <block_size> <search_area>\n";
        return -1;
    }

    int keyFramePeriod = std::stoi(argv[3]);
    int blockSize = std::stoi(argv[4]);
    int searchArea = std::stoi(argv[5]);

    if (keyFramePeriod <= 0 || blockSize <= 0 || searchArea <= 0) {
        std::cerr << "Invalid parameters\n";
        return -1;
    }

    cv::VideoCapture cap(argv[1]);
    if (!cap.isOpened()) {
        std::cerr << "No video data\n";
        return -1;
    }

    BitStream bs = BitStream(argv[2], BitStream::w);
    cv::Mat image;
    struct Image_Encoded data;
    uint64_t height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    uint64_t width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    data.error = (int16_t *)malloc(height * width * sizeof(int16_t));

    bs.mwriteNBits(height, 16);
    bs.mwriteNBits(width, 16);

    FrameData frames;
    frames.current = cv::Mat(height, width, CV_8UC3);
    frames.previous = cv::Mat(height, width, CV_8UC3);

    int frameCount = 0;
    while (true) {
        printf("\nFRAME: %d\n", frameCount++);
        cap >> frames.current;

        if (frames.current.empty()) {
            break;
        }

        bool isKeyFrame = (frameCount % keyFramePeriod == 0);
        ret = img_encode(&data, &frames, isKeyFrame, blockSize, searchArea);
        printf("m: %d\n", data.m);
        printf("i_predictor: %d\n", data.i_predictor);
        cv::imshow("frame", data.mat);
        cv::waitKey(1);

        Golomb g = Golomb(data.m, &bs);

        bs.mwriteNBits(uint64_t(data.m), 32);
        bs.mwriteNBits(uint64_t(data.i_predictor), 4);
        for (uint y = 0; y < height; y++) {
            for (uint x = 0; x < width; x++) {
                g.encode(data.error[y * width + x]);
            }
        }

        // Store current frame as previous for the next iteration
        frames.previous = frames.current.clone();
    }

    free(data.error);
    return 0;
}
