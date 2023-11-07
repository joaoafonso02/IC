#include <opencv2/opencv.hpp> 
#include <stdio.h> 
#include <string.h>

int main(int argc, char** argv) 
{ 
    if (argc != 4) { 
        printf("usage: mirror <in_image_path> <horizontal | vertical> <out_image_path>\n"); 
        return -1; 
    } 

    cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR); 
    if (!image.data) { 
        printf("No image data \n"); 
        return -1; 
    } 

    int h=1, v=1;
    if( std::string(argv[2]) == "horizontal" ) h = -1;
    else if( std::string(argv[2]) == "vertical" ) v = -1;
    else throw std::invalid_argument("2nd argument needs to be horizontal or vertical!\n");

    cv::Mat out {image.size(), image.type()};

    int y, x, dy, dx;
    for(y=0; y<image.rows; y++) {
        for(x=0; x<image.cols; x++){
            dy = v > 0 ? y : image.rows - y - 1; 
            dx = h > 0 ? x : image.cols - x - 1; 
            out.at<cv::Vec3b>(dy, dx) = image.at<cv::Vec3b>(y, x);
        }
    }

    cv::imwrite(argv[3], out);

    return 0; 
}
