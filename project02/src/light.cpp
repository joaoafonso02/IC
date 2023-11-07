#include <opencv2/opencv.hpp> 
#include <stdio.h> 

int main(int argc, char** argv) 
{ 
    if (argc != 4) { 
        printf("usage: light <in_image_path> <light intensity> <out_image_path>\n"); 
        return -1; 
    } 

    cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR); 
    if (!image.data) { 
        printf("No image data \n"); 
        return -1; 
    } 

    double intensity = std::stod(argv[2]);

    cv::Mat out {image.size(), image.type()};

    int y, x, i;
    for(y=0; y<image.rows; y++) {
        for(x=0; x<image.cols; x++){
            cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
            for(i=0; i<image.channels(); i++) pixel[i] = pixel[i]*intensity <= 255 ? pixel[i]*intensity : 255;
            out.at<cv::Vec3b>(y, x) = pixel;
        }
    }

    cv::imwrite(argv[3], out);

    return 0; 
}

