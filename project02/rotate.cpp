#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc != 4) {
        printf("usage: mirror <in_image_path> <90 | 180 | 270> <out_image_path>\n");
        return -1;
    }

    cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR);
    if (!image.data) {
        printf("No image data \n");
        return -1;
    }

    if (atoi(argv[2]) % 90 != 0)  throw std::invalid_argument("2nd argument needs to be multiple of 90!\n");

    int r = (atoi(argv[2]) / 90) % 4;

    cv::Mat out;
    if (r % 2 == 0) {
        out = cv::Mat(image.size(), image.type());
    }
    else {
        out = cv::Mat(image.cols, image.rows, image.type());
    }

    int y, x, dy, dx;
    for (y = 0; y < image.rows; y++) {
        for (x = 0; x < image.cols; x++) {
            if (r == 1) {
                dy = image.cols - x - 1;
                dx = y;
            }
            else if (r == 2) {
                dy = image.rows - y - 1;
                dx = image.cols - x - 1;
            }
            else if (r == 3) {
                dy = x;
                dx = image.rows - y - 1;
            }
            else {
                dy = y;
                dx = x;
            }
            out.at<cv::Vec3b>(dy, dx) = image.at<cv::Vec3b>(y, x);
        }
    }

    imshow("Transformed Image", out);
    cv::imwrite(argv[3], out);

    cv::waitKey(0); // Wait for user input
    return 0;
}
