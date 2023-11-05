#include <opencv2/opencv.hpp> 
#include <stdio.h> 

int main(int argc, char** argv) 
{ 
    if (argc != 4) { 
        printf("usage: extract_channel <in_image_path> <channel> <out_image_path>\n"); 
        return -1; 
    } 

    cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR); 
    if (!image.data) { 
        printf("No image data \n"); 
        return -1; 
    } 

    cv::Mat out {image.size(), CV_8UC1};

    int y, x;
    for(y=0; y<image.rows; y++) {
        for(x=0; x<image.cols; x++){
            image.at<unsigned char>(y,x) = image.at<cv::Vec3b>(y, x)[std::stoi(argv[2])];
        }
    }

    cv::imwrite(argv[3], out);

    return 0; 
}

